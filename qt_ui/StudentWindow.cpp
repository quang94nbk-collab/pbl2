// ============================================================
//  StudentWindow.cpp
//  Presentation Layer - Qt GUI - Cua so cho Sinh vien
//  Chi giao tiep voi BLL qua SystemManager.
//  KHONG co Qt code trong manager/, model/, dal/, DSA/.
// ============================================================

#include "StudentWindow.h"

#include "TimetableWidget.h"
#include "RegistrationWidget.h"

#include "../manager/exception.h"
#include "../manager/system.h"
#include "../model/course.h"
#include "../model/dtos.h"
#include "../model/student.h"
#include "../DSA/DoublyLinkedList.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTabWidget>
#include <QGroupBox>
#include <QFrame>
#include <QFont>
#include <QColor>

static QString formatThuLabelStudent(const std::string& thu) {
    if (thu == "CN" || thu == "cn") return "Chủ nhật";
    if (thu.rfind("Thu", 0) == 0 || thu.rfind("Thứ", 0) == 0)
        return QString::fromStdString(thu);
    return QString("Thứ %1").arg(QString::fromStdString(thu));
}

static void clearLayoutRecursive(QLayout* layout) {
    if (!layout) return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QLayout* subLayout = item->layout()) {
            clearLayoutRecursive(subLayout);
        }
        if (QWidget* w = item->widget()) {
            w->hide();
            w->deleteLater();
        }
        delete item;
    }
}

// ---- Constructor ----
StudentWindow::StudentWindow(SystemManager& sys,
                             const QString& studentID,
                             QWidget* parent)
    : QWidget(parent)
    , m_sys(sys)
    , m_studentID(studentID)
    , m_tabs(nullptr)
    , m_timetable(nullptr)
    , m_registration(nullptr)
    , m_registeredTab(nullptr)
    , m_progressTab(nullptr)
    , m_creditTab(nullptr)
{
    setupUi();

    setStyleSheet(R"(
        StudentWindow {
            background-color: #F3F6FB;
        }
        QTabWidget::pane {
            border: 1px solid #CBD5E1;
            background-color: #FFFFFF;
            border-radius: 0px 6px 6px 6px;
        }
        QTabBar::tab {
            background-color: #E2E8F0;
            color: #334155;
            padding: 9px 20px;
            margin-right: 3px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            font-size: 13px;
            font-weight: 600;
        }
        QTabBar::tab:selected {
            background-color: #2563EB;
            color: #FFFFFF;
        }
        QTabBar::tab:hover:!selected {
            background-color: #DBEAFE;
            color: #1D4ED8;
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
            padding: 7px 8px;
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
            padding: 7px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover { background-color: #3B82F6; }
        QPushButton:pressed { background-color: #1D4ED8; }
        QGroupBox {
            background-color: #FFFFFF;
            font-weight: bold;
            color: #1E40AF;
            border: 1px solid #CBD5E1;
            border-radius: 8px;
            margin-top: 10px;
            padding: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            background-color: #FFFFFF;
        }
    )");
}

void StudentWindow::setupUi()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // -- Thanh tieu de + nut Dang xuat --
    auto* topBar = new QFrame;
    topBar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #2563EB, stop:1 #1D4ED8);");
    topBar->setFixedHeight(52);
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(16, 0, 16, 0);

    const Student* sv = m_sys.findStudent(m_studentID.toStdString());
    QString headerText = sv
        ? QString("Hệ thống Đăng ký Tín chỉ - PBL2  |  Sinh viên: %1 (%2)")
              .arg(QString::fromStdString(sv->getHoTen()))
              .arg(m_studentID)
        : QString("Hệ thống Đăng ký Tín chỉ - PBL2  |  Sinh viên: %1").arg(m_studentID);

    auto* title = new QLabel(headerText);
    title->setStyleSheet("color:#FFFFFF; font-size:15px; font-weight:bold; background:transparent;");

    auto* logoutBtn = new QPushButton("Đăng xuất");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton { background-color:#EF4444; color:#FFFFFF; border:none; "
        "border-radius:6px; padding:6px 16px; font-size:13px; font-weight:bold; }"
        "QPushButton:hover { background-color:#DC2626; }"
        "QPushButton:pressed { background-color:#B91C1C; }");
    connect(logoutBtn, &QPushButton::clicked, this, &StudentWindow::onLogout);

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(logoutBtn);
    outerLayout->addWidget(topBar);

    // -- Content container --
    auto* contentWidget = new QWidget;
    contentWidget->setStyleSheet("background-color: #F3F6FB;");
    auto* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(10, 10, 10, 10);

    m_tabs = new QTabWidget(contentWidget);
    contentLayout->addWidget(m_tabs, 1);
    outerLayout->addWidget(contentWidget, 1);

    // Tab 1: Dang ky mon hoc (dat lam tab dau tien hoac thoi khoa bieu)
    m_timetable = new TimetableWidget(m_sys, m_studentID);
    m_tabs->addTab(m_timetable, "Thời khóa biểu");

    // Tab 2: Dang ky mon hoc theo nhom
    m_registration = new RegistrationWidget(m_sys, m_studentID);
    m_tabs->addTab(m_registration, "Đăng ký học phần");

    // Tab 3: Cac lop hoc phan da dang ky (cho phep huy dang ky)
    setupRegisteredTab();
    m_tabs->addTab(m_registeredTab, "Các lớp học phần đã đăng ký");

    // Tab 4: Tien do hoc tap
    setupProgressTab();
    m_tabs->addTab(m_progressTab, "Tiến độ học tập");

    // Tab 5: Tong ket tin chi
    setupCreditTab();
    m_tabs->addTab(m_creditTab, "Tổng kết tín chỉ");

    connect(m_tabs, &QTabWidget::currentChanged, this, &StudentWindow::onTabChanged);
    connect(m_registration, &RegistrationWidget::registrationChanged, this, [this]() {
        if (m_timetable) m_timetable->refresh();
        refreshRegisteredTab();
        refreshProgressTab();
        refreshCreditTab();
    });
}

// ================================================================
//  Tab: Cac lop hoc phan da dang ky (xem & huy dang ky)
// ================================================================
void StudentWindow::setupRegisteredTab()
{
    m_registeredTab = new QWidget;
    m_registeredTab->setStyleSheet("background-color: #FFFFFF;");
    auto* layout = new QVBoxLayout(m_registeredTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);
    refreshRegisteredTab();
}

void StudentWindow::refreshRegisteredTab()
{
    if (!m_registeredTab) return;
    clearLayoutRecursive(m_registeredTab->layout());

    auto* layout = qobject_cast<QVBoxLayout*>(m_registeredTab->layout());

    int currentSem = m_sys.getCurrentSemester();

    DoubleLinkedList<StudentRegistrationDetail> details;
    m_sys.getStudentRegistrations(m_studentID.toStdString(), details, currentSem);

    DoubleLinkedList<CreditSummaryItem> creditDetails;
    int totalCredits = m_sys.calculateTotalCredits(m_studentID.toStdString(), &creditDetails, currentSem);

    int count = 0;
    auto* cntNode = details.getHead();
    while (cntNode) {
        ++count;
        cntNode = cntNode->next;
    }

    auto* hdr = new QHBoxLayout;
    auto* titleLbl = new QLabel(
        QString("<b>Các lớp học phần bạn đã đăng ký - Học kỳ %1</b>  —  "
                "<span style='color:#1D4ED8;'>Tổng số lớp: %2  |  Tổng tín chỉ: %3 TC</span>")
            .arg(currentSem)
            .arg(count)
            .arg(totalCredits));
    titleLbl->setStyleSheet("font-size:13px; color:#1E40AF; background:transparent;");

    auto* refreshBtn = new QPushButton("Làm mới");
    refreshBtn->setCursor(Qt::PointingHandCursor);
    connect(refreshBtn, &QPushButton::clicked, this, &StudentWindow::refreshRegisteredTab);

    hdr->addWidget(titleLbl);
    hdr->addStretch();
    hdr->addWidget(refreshBtn);
    layout->addLayout(hdr);

    auto* table = new QTableWidget;
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({
        "Mã ĐK", "Mã lớp HP", "Tên môn học", "Nhóm",
        "Lịch học", "Giảng viên", "Học kỳ", "Ngày ĐK", "Thao tác"
    });
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setDefaultSectionSize(34);

    auto* node = details.getHead();
    while (node) {
        const StudentRegistrationDetail& d = node->data;
        int row = table->rowCount();
        table->insertRow(row);

        QString lichHoc = QString("%1 (Tiết %2-%3)")
            .arg(formatThuLabelStudent(d.thu))
            .arg(d.tietBatDau)
            .arg(d.tietKetThuc);

        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(d.maDangKy)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(d.maLopHP)));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(d.tenMon)));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(d.nhomLopHP)));
        table->setItem(row, 4, new QTableWidgetItem(lichHoc));
        table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(d.giangVien)));
        auto* hkItem = new QTableWidgetItem(QString("Kỳ %1").arg(d.hocKy));
        hkItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 6, hkItem);
        table->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(d.ngayDangKy)));

        QString qSecID = QString::fromStdString(d.maLopHP);
        auto* cancelBtn = new QPushButton("Hủy ĐK");
        cancelBtn->setCursor(Qt::PointingHandCursor);
        cancelBtn->setStyleSheet(
            "QPushButton { background-color:#EF4444; color:#FFFFFF; border-radius:5px; padding:4px 10px; font-size:12px; }"
            "QPushButton:hover { background-color:#DC2626; }");
        connect(cancelBtn, &QPushButton::clicked, this, [this, qSecID]() {
            handleCancelSection(qSecID);
        });
        table->setCellWidget(row, 8, cancelBtn);

        node = node->next;
    }
    table->resizeColumnsToContents();
    table->setColumnWidth(8, 100);
    layout->addWidget(table, 1);

    if (count == 0) {
        auto* emptyLbl = new QLabel(
            QString("Bạn chưa đăng ký lớp học phần nào trong Học kỳ %1.").arg(currentSem));
        emptyLbl->setAlignment(Qt::AlignCenter);
        emptyLbl->setStyleSheet("font-size:13px; color:#64748B; padding:8px; background:transparent;");
        layout->addWidget(emptyLbl);
    }
}

void StudentWindow::handleCancelSection(const QString& maLopHP)
{
    auto res = QMessageBox::question(this, "Xác nhận hủy đăng ký",
        QString("Bạn có chắc chắn muốn hủy đăng ký lớp học phần '%1'?").arg(maLopHP),
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    try {
        DoubleLinkedList<std::string> waitlistNotices;
        bool ok = m_sys.cancelRegistration(
            m_studentID.toStdString(), maLopHP.toStdString(), &waitlistNotices);

        auto* wNode = waitlistNotices.getHead();
        while (wNode) {
            QMessageBox::information(this, "Thông báo hàng chờ",
                QString::fromStdString(wNode->data));
            wNode = wNode->next;
        }

        if (ok) {
            QMessageBox::information(this, "Thành công",
                QString("Đã hủy đăng ký lớp '%1'.").arg(maLopHP));
            refreshRegisteredTab();
            if (m_registration) m_registration->refreshTable();
            if (m_timetable)    m_timetable->refresh();
            refreshProgressTab();
            refreshCreditTab();
        } else {
            QMessageBox::warning(this, "Lỗi",
                "Hủy đăng ký thất bại. Bạn có thể chưa đăng ký lớp này.");
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

// ================================================================
//  Tab: Tien do hoc tap
// ================================================================
void StudentWindow::setupProgressTab()
{
    m_progressTab = new QWidget;
    m_progressTab->setStyleSheet("background-color: #FFFFFF;");
    auto* layout = new QVBoxLayout(m_progressTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);
    refreshProgressTab();
}

void StudentWindow::refreshProgressTab()
{
    if (!m_progressTab) return;

    clearLayoutRecursive(m_progressTab->layout());

    auto* layout = qobject_cast<QVBoxLayout*>(m_progressTab->layout());

    SemesterProgressSummary summary;
    DoubleLinkedList<CourseProgressItem> items;
    bool ok = m_sys.getSemesterRegistrationStatus(m_studentID.toStdString(), summary, items);

    if (!ok) {
        layout->addWidget(new QLabel("Không thể tải thông tin tiến độ học tập."));
        return;
    }

    // -- Thong tin tong quan --
    auto* infoGroup = new QGroupBox("Thông tin tổng quan tiến độ");
    auto* infoLayout = new QVBoxLayout(infoGroup);
    infoLayout->setSpacing(8);

    auto* svInfo = new QLabel(
        QString("<b>Sinh viên:</b> %1 (%2)  |  <b>Lớp:</b> %3  |  <b>Học kỳ:</b> %4")
            .arg(QString::fromStdString(summary.hoTen))
            .arg(QString::fromStdString(summary.maSV))
            .arg(QString::fromStdString(summary.lop))
            .arg(summary.currentSemester)
    );
    svInfo->setStyleSheet("font-size:13px; color:#1E293B; background:transparent;");
    infoLayout->addWidget(svInfo);

    // Progress bar cho mon hoc
    auto* monHocRow = new QHBoxLayout;
    auto* monLbl = new QLabel("Môn học:");
    monLbl->setFixedWidth(70);
    monLbl->setStyleSheet("color:#334155; background:transparent;");
    monHocRow->addWidget(monLbl);
    auto* monBar = new QProgressBar;
    monBar->setRange(0, qMax(summary.totalRequiredCourses, 1));
    monBar->setValue(summary.registeredCourses);
    monBar->setFormat(QString("%1 / %2 môn").arg(summary.registeredCourses).arg(summary.totalRequiredCourses));
    monBar->setStyleSheet(
        "QProgressBar{border:1px solid #CBD5E1; border-radius:6px; background:#F1F5F9; "
        "color:#1E293B; text-align:center; font-size:12px; font-weight:bold; min-height:22px;}"
        "QProgressBar::chunk{background:#2563EB; border-radius:5px;}");
    monHocRow->addWidget(monBar, 1);
    infoLayout->addLayout(monHocRow);

    // Progress bar cho tin chi
    auto* tcRow = new QHBoxLayout;
    auto* tcLbl = new QLabel("Tín chỉ:");
    tcLbl->setFixedWidth(70);
    tcLbl->setStyleSheet("color:#334155; background:transparent;");
    tcRow->addWidget(tcLbl);
    auto* tcBar = new QProgressBar;
    tcBar->setRange(0, qMax(summary.totalCredits, 1));
    tcBar->setValue(summary.registeredCredits);
    tcBar->setFormat(QString("%1 / %2 TC").arg(summary.registeredCredits).arg(summary.totalCredits));
    tcBar->setStyleSheet(
        "QProgressBar{border:1px solid #CBD5E1; border-radius:6px; background:#F1F5F9; "
        "color:#1E293B; text-align:center; font-size:12px; font-weight:bold; min-height:22px;}"
        "QProgressBar::chunk{background:#16A34A; border-radius:5px;}");
    tcRow->addWidget(tcBar, 1);
    infoLayout->addLayout(tcRow);

    layout->addWidget(infoGroup);

    // -- Bang chi tiet --
    auto* table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Mã môn", "Tên môn học", "Số TC", "Trạng thái", "Mã lớp HP"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    auto* node = items.getHead();
    while (node) {
        const CourseProgressItem& item = node->data;
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(item.maMon)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(item.tenMon)));
        auto* tcItem = new QTableWidgetItem(QString::number(item.soTinChi));
        tcItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 2, tcItem);

        auto* statusItem = new QTableWidgetItem(item.daDangKy ? "Đã đăng ký" : "Chưa đăng ký");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setForeground(item.daDangKy ? QColor("#15803D") : QColor("#B91C1C"));
        QFont f = statusItem->font();
        f.setBold(true);
        statusItem->setFont(f);
        statusItem->setBackground(item.daDangKy ? QColor("#DCFCE7") : QColor("#FEE2E2"));
        table->setItem(row, 3, statusItem);

        table->setItem(row, 4, new QTableWidgetItem(
            item.daDangKy ? QString::fromStdString(item.maLopHP) : "-"));

        node = node->next;
    }
    table->resizeColumnsToContents();
    layout->addWidget(table, 1);

    int missing = summary.totalRequiredCourses - summary.registeredCourses;
    if (missing > 0) {
        auto* warn = new QLabel(
            QString("CẢNH BÁO: Bạn đang thiếu %1 môn học trong học kỳ này! "
                    "Vui lòng đăng ký thêm để không bị chậm tiến độ.").arg(missing));
        warn->setStyleSheet(
            "color:#B91C1C; background-color:#FEF2F2; border:1px solid #FECACA;"
            "border-radius:6px; padding:10px; font-size:13px; font-weight:bold;");
        warn->setWordWrap(true);
        layout->addWidget(warn);
    }
}

// ================================================================
//  Tab: Tong ket tin chi
// ================================================================
void StudentWindow::setupCreditTab()
{
    m_creditTab = new QWidget;
    m_creditTab->setStyleSheet("background-color: #FFFFFF;");
    auto* layout = new QVBoxLayout(m_creditTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);
    refreshCreditTab();
}

void StudentWindow::refreshCreditTab()
{
    if (!m_creditTab) return;

    clearLayoutRecursive(m_creditTab->layout());

    auto* layout = qobject_cast<QVBoxLayout*>(m_creditTab->layout());

    int currentSem = m_sys.getCurrentSemester();

    DoubleLinkedList<CreditSummaryItem> credits;
    int totalTC = m_sys.calculateTotalCredits(m_studentID.toStdString(), &credits, currentSem);

    auto* hdrWidget = new QWidget;
    hdrWidget->setStyleSheet("background:transparent;");
    auto* hdr = new QHBoxLayout(hdrWidget);
    hdr->setContentsMargins(0, 0, 0, 0);

    auto* lbl = new QLabel(
        QString("<b>Tổng kết tín chỉ đã đăng ký - Học kỳ %1</b>").arg(currentSem));
    lbl->setStyleSheet("font-size:14px; color:#1E40AF; background:transparent;");

    auto* totalLbl = new QLabel(
        QString("<b>Tổng cộng: %1 tín chỉ</b>").arg(totalTC));
    totalLbl->setStyleSheet(
        "font-size:13px; color:#15803D; background-color:#DCFCE7; "
        "border:1px solid #86EFAC; border-radius:6px; padding:5px 12px;");

    hdr->addWidget(lbl);
    hdr->addStretch();
    hdr->addWidget(totalLbl);
    layout->addWidget(hdrWidget);

    auto* table = new QTableWidget;
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Mã lớp HP", "Tên môn học", "Số tín chỉ"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    auto* node = credits.getHead();
    while (node) {
        const CreditSummaryItem& c = node->data;
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c.maLopHP)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c.tenMon)));
        auto* tcItem = new QTableWidgetItem(QString::number(c.soTinChi));
        tcItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 2, tcItem);
        node = node->next;
    }
    table->resizeColumnsToContents();
    layout->addWidget(table, 1);

    if (credits.getHead() == nullptr) {
        auto* emptyLbl = new QLabel(
            QString("Chưa có môn học nào được đăng ký trong Học kỳ %1.").arg(currentSem));
        emptyLbl->setAlignment(Qt::AlignCenter);
        emptyLbl->setStyleSheet("font-size:13px; color:#64748B; padding:8px; background:transparent;");
        layout->addWidget(emptyLbl);
    }

    auto* footer = new QLabel(
        QString("Tổng số tín chỉ đã đăng ký (Học kỳ hiện tại: %1): <b>%2 TC</b>")
            .arg(m_sys.getCurrentSemester())
            .arg(totalTC));
    footer->setStyleSheet(
        "font-size:14px; color:#1E40AF; background-color:#EFF6FF;"
        "border:1px solid #BFDBFE; border-radius:6px; padding:10px;");
    footer->setAlignment(Qt::AlignCenter);
    layout->addWidget(footer);
}

// ---- Slots ----
void StudentWindow::onLogout()
{
    std::string missingIDs[50];
    int missingCount = m_sys.getMissingCoursesCount(m_studentID.toStdString(), missingIDs, 50);

    QString msg = "Bạn có chắc chắn muốn đăng xuất?";
    if (missingCount > 0) {
        QString missingText;
        for (int i = 0; i < missingCount; ++i) {
            const Course* c = m_sys.findCourse(missingIDs[i]);
            missingText += QString("\n  • [%1] %2 (%3 TC)")
                .arg(QString::fromStdString(missingIDs[i]))
                .arg(QString::fromStdString(c ? c->getTenMon() : missingIDs[i]))
                .arg(c ? c->getSoTinChi() : 0);
        }
        msg = QString("CẢNH BÁO TIẾN ĐỘ HỌC TẬP (Học kỳ %1):\n"
                      "Bạn hiện đang đăng ký thiếu %2 môn:%3\n\n"
                      "Bạn có chắc chắn vẫn muốn đăng xuất?")
                  .arg(m_sys.getCurrentSemester())
                  .arg(missingCount)
                  .arg(missingText);
    }

    auto res = QMessageBox::question(this, "Xác nhận đăng xuất", msg,
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit logout();
    }
}

void StudentWindow::onTabChanged(int index)
{
    QWidget* current = m_tabs->widget(index);
    if (current == m_timetable)      m_timetable->refresh();
    if (current == m_registration)   m_registration->refreshTable();
    if (current == m_registeredTab)  refreshRegisteredTab();
    if (current == m_progressTab)    refreshProgressTab();
    if (current == m_creditTab)      refreshCreditTab();
}
