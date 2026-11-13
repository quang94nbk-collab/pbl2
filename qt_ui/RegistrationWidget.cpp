// ============================================================
//  RegistrationWidget.cpp
//  Presentation Layer - Qt GUI - Dang ky Tin chi
//  Chi giao tiep voi BLL qua SystemManager.
//  KHONG co Qt code trong manager/, model/, dal/, DSA/.
// ============================================================

#include "RegistrationWidget.h"

#include "../manager/exception.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/dtos.h"
#include "../model/registration.h"
#include "../model/student.h"
#include "../DSA/DoublyLinkedList.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QColor>
#include <QFont>

static const QString REG_STYLE_SHEET = R"(
    RegistrationWidget {
        background-color: #F3F6FB;
    }
    QTableWidget {
        background-color: #FFFFFF;
        color: #1E293B;
        gridline-color: #E2E8F0;
        font-size: 12px;
        selection-background-color: #DBEAFE;
        selection-color: #1E3A8A;
        alternate-background-color: #F8FAFC;
        border: 1px solid #CBD5E1;
        border-radius: 6px;
    }
    QHeaderView::section {
        background-color: #2563EB;
        color: #FFFFFF;
        padding: 6px 8px;
        font-size: 12px;
        font-weight: bold;
        border: none;
        border-right: 1px solid #3B82F6;
    }
    QPushButton {
        background-color: #2563EB;
        color: #FFFFFF;
        border: none;
        border-radius: 6px;
        padding: 6px 14px;
        font-size: 12px;
        font-weight: 600;
    }
    QPushButton:hover { background-color: #3B82F6; }
    QPushButton:pressed { background-color: #1D4ED8; }
    QPushButton:disabled {
        background-color: #CBD5E1;
        color: #64748B;
    }
    QGroupBox {
        background-color: #FFFFFF;
        font-weight: bold;
        color: #1E40AF;
        border: 1px solid #CBD5E1;
        border-radius: 8px;
        margin-top: 10px;
        padding: 12px 10px 10px 10px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 12px;
        padding: 0 6px;
        background-color: #FFFFFF;
    }
)";

static QString formatThuLabel(const std::string& thu) {
    if (thu == "CN" || thu == "cn") return "Chủ nhật";
    if (thu.rfind("Thu", 0) == 0 || thu.rfind("Thứ", 0) == 0)
        return QString::fromStdString(thu);
    return QString("Thứ %1").arg(QString::fromStdString(thu));
}

// ---- Constructor ----
RegistrationWidget::RegistrationWidget(SystemManager& sys,
                                       const QString& studentID,
                                       QWidget* parent)
    : QWidget(parent)
    , m_sys(sys)
    , m_studentID(studentID)
    , m_isAdmin(studentID.isEmpty())
    , m_adminTable(nullptr)
    , m_infoLabel(nullptr)
    , m_groupStatusLabel(nullptr)
    , m_sectionTitleLabel(nullptr)
    , m_registerGroupBtn(nullptr)
    , m_refreshBtn(nullptr)
    , m_groupTable(nullptr)
    , m_sectionTable(nullptr)
{
    setStyleSheet(REG_STYLE_SHEET);
    if (m_isAdmin) {
        setupAdminUi();
        populateAdminTable();
    } else {
        setupStudentUi();
        refreshTable();
    }
}

// ================================================================
//  ADMIN MODE
// ================================================================
void RegistrationWidget::setupAdminUi()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto* hdr = new QHBoxLayout;
    auto* lbl = new QLabel("<b>Danh sách tất cả đăng ký</b>");
    lbl->setStyleSheet("font-size:14px; color:#1E40AF; background:transparent;");
    auto* refreshBtn = new QPushButton("Làm mới");
    refreshBtn->setCursor(Qt::PointingHandCursor);
    connect(refreshBtn, &QPushButton::clicked, this, &RegistrationWidget::onRefresh);
    hdr->addWidget(lbl);
    hdr->addStretch();
    hdr->addWidget(refreshBtn);
    layout->addLayout(hdr);

    m_adminTable = new QTableWidget;
    m_adminTable->setColumnCount(4);
    m_adminTable->setHorizontalHeaderLabels({"Mã đăng ký", "Mã SV", "Mã lớp HP", "Ngày ĐK"});
    m_adminTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_adminTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_adminTable->setAlternatingRowColors(true);
    m_adminTable->horizontalHeader()->setStretchLastSection(true);
    m_adminTable->verticalHeader()->setVisible(false);
    layout->addWidget(m_adminTable, 1);
}

void RegistrationWidget::populateAdminTable()
{
    if (!m_adminTable) return;
    m_adminTable->setRowCount(0);

    DoubleLinkedList<Registration> regs;
    m_sys.getAllRegistrations(regs);

    auto* node = regs.getHead();
    int currentSem = m_sys.getCurrentSemester();
    while (node) {
        const Registration& r = node->data;
        const ClassSection* sec = m_sys.findClassSection(r.getMaLopHP());
        if (!sec || sec->getHocKy() != currentSem) {
            node = node->next;
            continue;
        }
        int row = m_adminTable->rowCount();
        m_adminTable->insertRow(row);
        m_adminTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(r.getMaDangKy())));
        m_adminTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(r.getMaSV())));
        m_adminTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(r.getMaLopHP())));
        m_adminTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(r.getNgayDangKy())));
        node = node->next;
    }
    m_adminTable->resizeColumnsToContents();
}

// ================================================================
//  STUDENT MODE - Hien thi nhom lop HP phu hop Khoa/Khoa hoc/Nganh/Hoc ky
// ================================================================
void RegistrationWidget::setupStudentUi()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    // -- Banner thong tin sinh vien --
    m_infoLabel = new QLabel;
    m_infoLabel->setStyleSheet(
        "font-size:13px; color:#1E3A8A; font-weight:600; "
        "background-color:#EFF6FF; border:1px solid #BFDBFE; "
        "border-radius:8px; padding:8px 12px;");
    m_infoLabel->setWordWrap(true);
    layout->addWidget(m_infoLabel);

    // GroupBox 1: Danh sach nhom lop HP phu hop
    auto* groupBox = new QGroupBox("1. Nhóm lớp học phần phù hợp (theo Khoa - Khóa - Ngành - Học kỳ)");
    auto* groupLayout = new QVBoxLayout(groupBox);
    groupLayout->setSpacing(6);

    auto* groupBar = new QHBoxLayout;
    m_groupStatusLabel = new QLabel;
    m_groupStatusLabel->setStyleSheet("font-size:12px; color:#475569; font-weight:normal; background:transparent;");
    m_registerGroupBtn = new QPushButton("Đăng ký cả nhóm đang chọn");
    m_registerGroupBtn->setCursor(Qt::PointingHandCursor);
    m_registerGroupBtn->setStyleSheet(
        "QPushButton { background-color:#16A34A; color:#FFFFFF; border-radius:6px; padding:6px 14px; font-weight:600; }"
        "QPushButton:hover { background-color:#22C55E; }"
        "QPushButton:disabled { background-color:#CBD5E1; color:#64748B; }");
    m_refreshBtn = new QPushButton("Làm mới");
    m_refreshBtn->setCursor(Qt::PointingHandCursor);

    groupBar->addWidget(m_groupStatusLabel, 1);
    groupBar->addWidget(m_registerGroupBtn);
    groupBar->addWidget(m_refreshBtn);
    groupLayout->addLayout(groupBar);

    m_groupTable = new QTableWidget;
    m_groupTable->setColumnCount(6);
    m_groupTable->setHorizontalHeaderLabels({
        "Tên nhóm lớp HP", "Khoa", "Khóa học", "Ngành học", "Số lớp HP", "Tổng TC"
    });
    m_groupTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_groupTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_groupTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_groupTable->setAlternatingRowColors(true);
    m_groupTable->horizontalHeader()->setStretchLastSection(true);
    m_groupTable->verticalHeader()->setVisible(false);
    m_groupTable->setMaximumHeight(160);
    groupLayout->addWidget(m_groupTable);
    layout->addWidget(groupBox);

    // GroupBox 2: Cac lop hoc phan trong nhom dang chon (chi cho phep dang ky)
    auto* secBox = new QGroupBox("2. Các lớp học phần trong nhóm đang chọn (chọn lớp để đăng ký)");
    auto* secLayout = new QVBoxLayout(secBox);
    secLayout->setSpacing(6);

    m_sectionTitleLabel = new QLabel("Vui lòng chọn một nhóm lớp học phần ở trên.");
    m_sectionTitleLabel->setStyleSheet("font-size:12px; color:#1E40AF; font-weight:600; background:transparent;");
    secLayout->addWidget(m_sectionTitleLabel);

    m_sectionTable = new QTableWidget;
    m_sectionTable->setColumnCount(10);
    m_sectionTable->setHorizontalHeaderLabels({
        "Mã lớp HP", "Mã môn", "Tên môn học", "TC", "Giảng viên",
        "Lịch học", "Phòng", "Sĩ số", "Trạng thái", "Thao tác"
    });
    m_sectionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sectionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_sectionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_sectionTable->setAlternatingRowColors(true);
    m_sectionTable->horizontalHeader()->setStretchLastSection(true);
    m_sectionTable->verticalHeader()->setVisible(false);
    m_sectionTable->verticalHeader()->setDefaultSectionSize(34);
    secLayout->addWidget(m_sectionTable);
    layout->addWidget(secBox, 1);

    // -- Ket noi --
    connect(m_refreshBtn,       &QPushButton::clicked, this, &RegistrationWidget::onRefresh);
    connect(m_registerGroupBtn, &QPushButton::clicked, this, &RegistrationWidget::onRegisterSelectedGroup);
    connect(m_groupTable,       &QTableWidget::itemSelectionChanged,
            this, &RegistrationWidget::onGroupSelectionChanged);
}

void RegistrationWidget::populateEligibleGroups()
{
    if (!m_groupTable) return;

    int prevSelectedRow = m_groupTable->currentRow();
    QString prevGroupName;
    if (prevSelectedRow >= 0 && prevSelectedRow < static_cast<int>(m_eligibleGroups.size())) {
        prevGroupName = QString::fromStdString(m_eligibleGroups[prevSelectedRow].groupName);
    }

    m_eligibleGroups.clear();
    m_groupTable->setRowCount(0);

    const Student* sv = m_sys.findStudent(m_studentID.toStdString());
    if (!sv) {
        m_groupStatusLabel->setText("Không tìm thấy thông tin sinh viên.");
        m_registerGroupBtn->setEnabled(false);
        return;
    }

    if (m_infoLabel) {
        m_infoLabel->setText(
            QString("Sinh viên: %1 (%2)  |  Khoa: %3  |  Khóa: %4  |  Ngành: %5  |  Lớp: %6  |  Học kỳ hiện tại: %7")
                .arg(QString::fromStdString(sv->getHoTen()))
                .arg(m_studentID)
                .arg(sv->getKhoa().empty() ? "Chưa cập nhật" : QString::fromStdString(sv->getKhoa()))
                .arg(sv->getKhoaHoc().empty() ? "Chưa cập nhật" : QString::fromStdString(sv->getKhoaHoc()))
                .arg(sv->getNganhHoc().empty() ? "Chưa cập nhật" : QString::fromStdString(sv->getNganhHoc()))
                .arg(QString::fromStdString(sv->getLop()))
                .arg(m_sys.getCurrentSemester())
        );
    }

    const int MAX_GROUPS = 100;
    GroupInfo rawGroups[MAX_GROUPS];
    int rawCount = m_sys.getEligibleGroupsForStudent(m_studentID.toStdString(), rawGroups, MAX_GROUPS);

    // Loc chat che theo dung Khoa, Khoa hoc, Nganh hoc va Hoc ky cua sinh vien
    for (int i = 0; i < rawCount; ++i) {
        const GroupInfo& g = rawGroups[i];
        bool allMatch = (g.totalSections > 0);
        for (int j = 0; j < g.totalSections; ++j) {
            const ClassSection* sec = m_sys.findClassSection(g.sectionIDs[j]);
            if (!sec || sec->getHocKy() != m_sys.getCurrentSemester()) {
                allMatch = false;
                break;
            }
            const Course* c = m_sys.findCourse(sec->getMaMon());
            if (c) {
                if (c->getHocKy() != 0 && c->getHocKy() != m_sys.getCurrentSemester()) {
                    allMatch = false;
                    break;
                }
                bool mK  = c->getKhoa().empty()     || c->getKhoa()     == "Chung" || (!sv->getKhoa().empty()     && sv->getKhoa()     == c->getKhoa());
                bool mKH = c->getKhoaHoc().empty()  || c->getKhoaHoc()  == "Chung" || (!sv->getKhoaHoc().empty()  && sv->getKhoaHoc()  == c->getKhoaHoc());
                bool mNg = c->getNganhHoc().empty() || c->getNganhHoc() == "Chung" || (!sv->getNganhHoc().empty() && sv->getNganhHoc() == c->getNganhHoc());
                if (!mK || !mKH || !mNg) {
                    allMatch = false;
                    break;
                }
            }
        }
        if (allMatch) {
            m_eligibleGroups.push_back(g);
        }
    }

    if (m_eligibleGroups.empty()) {
        m_groupStatusLabel->setText(
            "Hiện chưa có nhóm lớp học phần nào mở cho Khoa / Khóa / Ngành / Học kỳ của bạn.");
        m_groupStatusLabel->setStyleSheet("font-size:12px; color:#B45309; font-weight:600; background:transparent;");
        m_registerGroupBtn->setEnabled(false);
        m_sectionTable->setRowCount(0);
        m_sectionTitleLabel->setText("Không có nhóm lớp học phần phù hợp.");
        return;
    }

    m_groupStatusLabel->setText(
        QString("Tìm thấy %1 nhóm lớp học phần phù hợp với chương trình của bạn (chọn 1 nhóm để xem chi tiết):")
            .arg(m_eligibleGroups.size()));
    m_groupStatusLabel->setStyleSheet("font-size:12px; color:#1E40AF; font-weight:600; background:transparent;");
    m_registerGroupBtn->setEnabled(true);

    int rowToSelect = 0;
    for (int i = 0; i < static_cast<int>(m_eligibleGroups.size()); ++i) {
        const GroupInfo& g = m_eligibleGroups[i];
        int row = m_groupTable->rowCount();
        m_groupTable->insertRow(row);

        auto makeItem = [](const QString& txt, Qt::Alignment align = Qt::AlignCenter) {
            auto* it = new QTableWidgetItem(txt);
            it->setTextAlignment(align);
            return it;
        };

        QString gName = QString::fromStdString(g.groupName);
        if (gName == prevGroupName) rowToSelect = i;

        m_groupTable->setItem(row, 0, makeItem(gName, Qt::AlignLeft | Qt::AlignVCenter));
        m_groupTable->setItem(row, 1, makeItem(g.khoa.empty()     ? "Chung" : QString::fromStdString(g.khoa)));
        m_groupTable->setItem(row, 2, makeItem(g.khoaHoc.empty()  ? "Chung" : QString::fromStdString(g.khoaHoc)));
        m_groupTable->setItem(row, 3, makeItem(g.nganhHoc.empty() ? "Chung" : QString::fromStdString(g.nganhHoc)));
        m_groupTable->setItem(row, 4, makeItem(QString("%1 lớp").arg(g.totalSections)));
        m_groupTable->setItem(row, 5, makeItem(QString("%1 TC").arg(g.totalCredits)));
    }
    m_groupTable->resizeColumnsToContents();
    m_groupTable->selectRow(rowToSelect);
    populateSectionsForGroup(rowToSelect);
}

void RegistrationWidget::populateSectionsForGroup(int groupIndex)
{
    if (!m_sectionTable) return;
    m_sectionTable->setRowCount(0);

    if (groupIndex < 0 || groupIndex >= static_cast<int>(m_eligibleGroups.size())) {
        m_sectionTitleLabel->setText("Vui lòng chọn một nhóm lớp học phần ở trên.");
        return;
    }

    const GroupInfo& g = m_eligibleGroups[groupIndex];
    m_sectionTitleLabel->setText(
        QString("Nhóm đang chọn: %1  (%2 lớp học phần • %3 tín chỉ)")
            .arg(QString::fromStdString(g.groupName))
            .arg(g.totalSections)
            .arg(g.totalCredits));

    std::string sid = m_studentID.toStdString();

    for (int i = 0; i < g.totalSections; ++i) {
        const std::string& secID = g.sectionIDs[i];
        const ClassSection* sec = m_sys.findClassSection(secID);
        if (!sec) continue;
        const Course* c = m_sys.findCourse(sec->getMaMon());

        int row = m_sectionTable->rowCount();
        m_sectionTable->insertRow(row);

        bool daDKThis = m_sys.isSectionRegistered(sid, secID);
        std::string otherSec = m_sys.getRegisteredSectionForCourse(sid, sec->getMaMon());
        bool daDKOther = (!daDKThis && !otherSec.empty());
        bool isFull = !sec->conCho();
        bool inWaitlist = m_sys.isStudentInWaitlist(sid, secID);

        auto makeItem = [](const QString& txt, Qt::Alignment align = Qt::AlignCenter) {
            auto* it = new QTableWidgetItem(txt);
            it->setTextAlignment(align);
            return it;
        };

        QString lichHoc = QString("%1 (Tiết %2-%3)")
            .arg(formatThuLabel(sec->getThu()))
            .arg(sec->getTietBatDau())
            .arg(sec->getTietKetThuc());
        QString siSo = QString("%1 / %2")
            .arg(sec->getSoLuongDaDangKy())
            .arg(sec->getSiSoToiDa());

        m_sectionTable->setItem(row, 0, makeItem(QString::fromStdString(sec->getMaLopHP())));
        m_sectionTable->setItem(row, 1, makeItem(QString::fromStdString(sec->getMaMon())));
        m_sectionTable->setItem(row, 2, makeItem(
            QString::fromStdString(c ? c->getTenMon() : sec->getMaMon()),
            Qt::AlignLeft | Qt::AlignVCenter));
        m_sectionTable->setItem(row, 3, makeItem(QString::number(c ? c->getSoTinChi() : 0)));
        m_sectionTable->setItem(row, 4, makeItem(
            QString::fromStdString(sec->getGiangVien()),
            Qt::AlignLeft | Qt::AlignVCenter));
        m_sectionTable->setItem(row, 5, makeItem(lichHoc));
        m_sectionTable->setItem(row, 6, makeItem(QString::fromStdString(sec->getPhong())));
        m_sectionTable->setItem(row, 7, makeItem(siSo));

        // Trang thai
        QTableWidgetItem* statusItem = nullptr;
        if (daDKThis) {
            statusItem = makeItem("Đã đăng ký");
            statusItem->setForeground(QColor("#15803D"));
            statusItem->setBackground(QColor("#DCFCE7"));
        } else if (daDKOther) {
            statusItem = makeItem(QString("Đã ĐK (%1)").arg(QString::fromStdString(otherSec)));
            statusItem->setForeground(QColor("#B45309"));
            statusItem->setBackground(QColor("#FEF3C7"));
        } else if (isFull && inWaitlist) {
            statusItem = makeItem("Hết chỗ (Chờ)");
            statusItem->setForeground(QColor("#B45309"));
            statusItem->setBackground(QColor("#FEF3C7"));
        } else if (isFull) {
            statusItem = makeItem("Hết chỗ");
            statusItem->setForeground(QColor("#B91C1C"));
            statusItem->setBackground(QColor("#FEE2E2"));
        } else {
            statusItem = makeItem("Còn chỗ");
            statusItem->setForeground(QColor("#1D4ED8"));
            statusItem->setBackground(QColor("#EFF6FF"));
        }
        QFont sf = statusItem->font();
        sf.setBold(true);
        statusItem->setFont(sf);
        m_sectionTable->setItem(row, 8, statusItem);

        // Nut thao tac ngay tren dong (chi cho phep dang ky; huy DK thuc hien o tab Da dang ky)
        QString qSecID = QString::fromStdString(secID);
        auto* actionBtn = new QPushButton;
        if (daDKThis) {
            actionBtn->setText("Đã đăng ký");
            actionBtn->setEnabled(false);
        } else if (daDKOther) {
            actionBtn->setText("Đã ĐK môn");
            actionBtn->setEnabled(false);
        } else if (isFull && inWaitlist) {
            actionBtn->setText("Đang chờ");
            actionBtn->setEnabled(false);
        } else if (isFull) {
            actionBtn->setCursor(Qt::PointingHandCursor);
            actionBtn->setText("Vào hàng chờ");
            actionBtn->setStyleSheet(
                "QPushButton { background-color:#F59E0B; color:#FFFFFF; border-radius:5px; padding:4px 10px; font-size:12px; }"
                "QPushButton:hover { background-color:#D97706; }");
            connect(actionBtn, &QPushButton::clicked, this, [this, qSecID]() {
                handleRegisterSection(qSecID);
            });
        } else {
            actionBtn->setCursor(Qt::PointingHandCursor);
            actionBtn->setText("Đăng ký");
            actionBtn->setStyleSheet(
                "QPushButton { background-color:#2563EB; color:#FFFFFF; border-radius:5px; padding:4px 10px; font-size:12px; }"
                "QPushButton:hover { background-color:#1D4ED8; }");
            connect(actionBtn, &QPushButton::clicked, this, [this, qSecID]() {
                handleRegisterSection(qSecID);
            });
        }
        m_sectionTable->setCellWidget(row, 9, actionBtn);
    }

    m_sectionTable->resizeColumnsToContents();
    m_sectionTable->setColumnWidth(9, 120);
}

void RegistrationWidget::refreshTable()
{
    if (m_isAdmin) {
        populateAdminTable();
    } else {
        populateEligibleGroups();
    }
}

void RegistrationWidget::onGroupSelectionChanged()
{
    if (!m_groupTable) return;
    int row = m_groupTable->currentRow();
    populateSectionsForGroup(row);
}

void RegistrationWidget::handleRegisterSection(const QString& maLopHP)
{
    try {
        m_sys.registerClass(m_studentID.toStdString(), maLopHP.toStdString());
        QMessageBox::information(this, "Đăng ký thành công",
            QString("Đã đăng ký lớp học phần '%1' thành công!").arg(maLopHP));
        refreshTable();
        emit registrationChanged();
    } catch (const ClassFullException& e) {
        QMessageBox::information(this, "Lớp đã đầy - Hàng chờ (Waitlist)",
            QString::fromStdString(e.what()));
        refreshTable();
    } catch (const TimeConflictException& e) {
        QMessageBox::warning(this, "Trùng lịch học",
            QString::fromStdString(e.what()));
    } catch (const DuplicateException& e) {
        QMessageBox::warning(this, "Trùng đăng ký",
            QString::fromStdString(e.what()));
    } catch (const WrongSemesterException& e) {
        QMessageBox::warning(this, "Sai học kỳ",
            QString::fromStdString(e.what()));
    } catch (const NotEligibleException& e) {
        QMessageBox::warning(this, "Không đủ điều kiện",
            QString::fromStdString(e.what()));
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi đăng ký",
            QString::fromStdString(e.what()));
    }
}

void RegistrationWidget::onRegisterSelectedGroup()
{
    if (!m_groupTable) return;
    int groupIdx = m_groupTable->currentRow();
    if (groupIdx < 0 || groupIdx >= static_cast<int>(m_eligibleGroups.size())) {
        QMessageBox::information(this, "Chưa chọn nhóm",
            "Vui lòng chọn một nhóm lớp học phần trước.");
        return;
    }

    const GroupInfo& g = m_eligibleGroups[groupIdx];
    auto confirm = QMessageBox::question(this, "Đăng ký cả nhóm",
        QString("Bạn muốn đăng ký tất cả các lớp học phần trong nhóm '%1' (%2 lớp)?")
            .arg(QString::fromStdString(g.groupName))
            .arg(g.totalSections),
        QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    QStringList okList;
    QStringList failList;
    std::string sid = m_studentID.toStdString();

    for (int i = 0; i < g.totalSections; ++i) {
        QString secID = QString::fromStdString(g.sectionIDs[i]);
        if (m_sys.isSectionRegistered(sid, g.sectionIDs[i])) {
            okList << QString("• %1 (đã đăng ký trước đó)").arg(secID);
            continue;
        }
        try {
            m_sys.registerClass(sid, g.sectionIDs[i]);
            okList << QString("• %1 (thành công)").arg(secID);
        } catch (const AppException& e) {
            failList << QString("• %1: %2").arg(secID, QString::fromStdString(e.what()));
        }
    }

    QString summary;
    if (!okList.isEmpty()) {
        summary += "<b>Kết quả thành công:</b><br>" + okList.join("<br>") + "<br><br>";
    }
    if (!failList.isEmpty()) {
        summary += "<b>Không đăng ký được:</b><br>" + failList.join("<br>");
    }

    QMessageBox::information(this, "Kết quả đăng ký nhóm", summary);
    refreshTable();
    emit registrationChanged();
}

void RegistrationWidget::onRefresh()
{
    refreshTable();
}
