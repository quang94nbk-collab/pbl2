#include "AdminWindow.h"

#include "StudentManagementWidget.h"
#include "CourseManagementWidget.h"
#include "SectionManagementWidget.h"

#include "../manager/system.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/student.h"
#include "../model/registration.h"
#include "../DSA/DoublyLinkedList.h"

#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QMessageBox>
#include <QSizePolicy>
#include <QString>
#include <QComboBox>
#include <QLineEdit>

AdminWindow::AdminWindow(SystemManager& system, QWidget* parent)
    : QWidget(parent)
    , m_system(system)
    , m_tabWidget(nullptr)
    , m_studentWidget(nullptr)
    , m_courseWidget(nullptr)
    , m_sectionWidget(nullptr)
    , m_curriculumTab(nullptr)
    , m_currKhoaCombo(nullptr)
    , m_currKhoaHocEdit(nullptr)
    , m_currNganhEdit(nullptr)
    , m_currHocKyCombo(nullptr)
    , m_currFilterBtn(nullptr)
    , m_currResetBtn(nullptr)
    , m_curriculumTable(nullptr)
    , m_registrationsTab(nullptr)
    , m_registrationsTable(nullptr)
    , m_regTitle(nullptr)
    , m_settingsTab(nullptr)
    , m_semesterSpinBox(nullptr)
    , m_semesterBadge(nullptr)
{
    setupUi();

    // Soft light-blue & white stylesheet
    setStyleSheet(R"(
        AdminWindow {
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
            border: 1px solid #E2E8F0;
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
        QGroupBox {
            background-color: #FFFFFF;
            font-weight: bold;
            color: #1E40AF;
            border: 1px solid #CBD5E1;
            border-radius: 8px;
            margin-top: 10px;
            padding: 14px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            background-color: #FFFFFF;
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
        QPushButton:hover {
            background-color: #3B82F6;
        }
        QPushButton:pressed {
            background-color: #1D4ED8;
        }
        QLineEdit {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 13px;
        }
        QLineEdit:focus {
            border: 1.5px solid #2563EB;
        }
        QComboBox {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 5px 8px;
            font-size: 13px;
        }
    )");
}

void AdminWindow::setupUi()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // ---- Top Header Bar with Logout Button ----
    auto* topBar = new QFrame;
    topBar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #2563EB, stop:1 #1D4ED8);");
    topBar->setFixedHeight(52);
    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(16, 0, 16, 0);

    auto* titleLbl = new QLabel("Hệ thống Đăng ký Tín chỉ - PBL2  |  Quản trị viên (Admin)");
    titleLbl->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: bold; background: transparent;");

    auto* logoutBtn = new QPushButton("Đăng xuất");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton { background-color: #EF4444; color: #FFFFFF; border: none; "
        "border-radius: 6px; padding: 6px 16px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #DC2626; }"
        "QPushButton:pressed { background-color: #B91C1C; }");
    connect(logoutBtn, &QPushButton::clicked, this, &AdminWindow::onLogout);

    topLayout->addWidget(titleLbl);
    topLayout->addStretch();
    topLayout->addWidget(logoutBtn);
    outerLayout->addWidget(topBar);

    // ---- Main Content Area ----
    auto* contentWidget = new QWidget;
    contentWidget->setStyleSheet("background-color: #F3F6FB;");
    auto* mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(0);

    m_tabWidget = new QTabWidget(contentWidget);
    m_tabWidget->setDocumentMode(false);

    // ---- Tab 1: Sinh viên ----
    m_studentWidget = new StudentManagementWidget(m_system, this);
    m_tabWidget->addTab(m_studentWidget, "Sinh viên");

    // ---- Tab 2: Môn học ----
    m_courseWidget = new CourseManagementWidget(m_system, this);
    m_tabWidget->addTab(m_courseWidget, "Môn học");

    // ---- Tab 3: Khung chương trình đào tạo ----
    setupCurriculumTab();
    m_tabWidget->addTab(m_curriculumTab, "Khung chương trình đào tạo");

    // ---- Tab 4: Lớp học phần ----
    m_sectionWidget = new SectionManagementWidget(m_system, this);
    m_tabWidget->addTab(m_sectionWidget, "Lớp học phần");

    // ---- Tab 5: Đăng ký ----
    setupRegistrationsTab();
    m_tabWidget->addTab(m_registrationsTab, "Đăng ký");

    // ---- Tab 6: Cài đặt ----
    setupSettingsTab();
    m_tabWidget->addTab(m_settingsTab, "Cài đặt");

    mainLayout->addWidget(m_tabWidget);
    outerLayout->addWidget(contentWidget, 1);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &AdminWindow::onTabChanged);

    // Load data into registrations tab on startup
    refreshRegistrationsTab();
}

/**
 * @brief Khoi tao Tab 'Khung chuong trinh dao tao' trong AdminWindow.
 * Cung cap bo loc (Khoa, Khoa hoc, Nganh, Hoc ky) va bang hien thi chuong trinh dao tao doc lap.
 */
void AdminWindow::setupCurriculumTab()
{
    m_curriculumTab = new QWidget;
    m_curriculumTab->setStyleSheet("background-color: #FFFFFF;");
    auto* mainLayout = new QVBoxLayout(m_curriculumTab);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(10);

    auto* filterGroup = new QGroupBox("Bộ lọc Khung chương trình đào tạo", m_curriculumTab);
    filterGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; color: #1E40AF; border: 1px solid #CBD5E1; "
        "border-radius: 6px; margin-top: 6px; padding: 10px; } "
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }");

    auto* filterLayout = new QHBoxLayout(filterGroup);

    auto* lblKhoa = new QLabel("Khoa:", filterGroup);
    m_currKhoaCombo = new QComboBox(filterGroup);
    m_currKhoaCombo->addItems({"Tất cả", "CNTT", "DTVT", "CoKhi", "Dien", "Hoa", "XayDung"});

    auto* lblKhoaHoc = new QLabel("Khóa:", filterGroup);
    m_currKhoaHocEdit = new QLineEdit(filterGroup);
    m_currKhoaHocEdit->setPlaceholderText("VD: K23");
    m_currKhoaHocEdit->setMaximumWidth(100);

    auto* lblNganh = new QLabel("Ngành:", filterGroup);
    m_currNganhEdit = new QLineEdit(filterGroup);
    m_currNganhEdit->setPlaceholderText("VD: CNTT");
    m_currNganhEdit->setMaximumWidth(120);

    auto* lblHocKy = new QLabel("Học kỳ:", filterGroup);
    m_currHocKyCombo = new QComboBox(filterGroup);
    m_currHocKyCombo->addItems({"Tất cả", "Kỳ 1", "Kỳ 2", "Kỳ 3", "Kỳ 4", "Kỳ 5", "Kỳ 6", "Kỳ 7", "Kỳ 8", "Kỳ 9", "Kỳ 10"});

    m_currFilterBtn = new QPushButton("Lọc chương trình", filterGroup);
    m_currFilterBtn->setStyleSheet("QPushButton{background:#0D9488;} QPushButton:hover{background:#0F766E;}");

    m_currResetBtn = new QPushButton("Xem tất cả", filterGroup);
    m_currResetBtn->setStyleSheet("QPushButton{background:#64748B;} QPushButton:hover{background:#475569;}");

    filterLayout->addWidget(lblKhoa);
    filterLayout->addWidget(m_currKhoaCombo);
    filterLayout->addWidget(lblKhoaHoc);
    filterLayout->addWidget(m_currKhoaHocEdit);
    filterLayout->addWidget(lblNganh);
    filterLayout->addWidget(m_currNganhEdit);
    filterLayout->addWidget(lblHocKy);
    filterLayout->addWidget(m_currHocKyCombo);
    filterLayout->addWidget(m_currFilterBtn);
    filterLayout->addWidget(m_currResetBtn);
    filterLayout->addStretch();
    mainLayout->addWidget(filterGroup);

    m_curriculumTable = new QTableWidget(m_curriculumTab);
    m_curriculumTable->setColumnCount(7);
    m_curriculumTable->setHorizontalHeaderLabels({
        "Mã môn", "Tên môn", "Số TC", "Khoa", "Khoá học", "Ngành", "Học kỳ"
    });
    m_curriculumTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_curriculumTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_curriculumTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_curriculumTable->setAlternatingRowColors(true);
    m_curriculumTable->horizontalHeader()->setStretchLastSection(true);
    m_curriculumTable->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_curriculumTable, 1);

    connect(m_currFilterBtn,   &QPushButton::clicked,     this, &AdminWindow::onCurriculumFilter);
    connect(m_currResetBtn,    &QPushButton::clicked,     this, &AdminWindow::onCurriculumReset);
    connect(m_currKhoaHocEdit, &QLineEdit::returnPressed, this, &AdminWindow::onCurriculumFilter);
    connect(m_currNganhEdit,   &QLineEdit::returnPressed, this, &AdminWindow::onCurriculumFilter);

    onCurriculumFilter();
}

/**
 * @brief Thuc hien loc va nap danh sach mon hoc vao bang Khung chuong trinh dao tao theo cac tieu chi.
 */
void AdminWindow::onCurriculumFilter()
{
    if (!m_curriculumTable) return;
    QString khoa = m_currKhoaCombo ? m_currKhoaCombo->currentText() : "Tất cả";
    if (khoa == "Tất cả") khoa = "";
    QString khoaHoc = m_currKhoaHocEdit ? m_currKhoaHocEdit->text().trimmed() : "";
    QString nganh   = m_currNganhEdit ? m_currNganhEdit->text().trimmed() : "";

    int hk = 0;
    if (m_currHocKyCombo) {
        int idx = m_currHocKyCombo->currentIndex();
        if (idx >= 1 && idx <= 10) hk = idx;
    }

    DoubleLinkedList<Course> list;
    m_system.filterCourses(khoa.toStdString(), khoaHoc.toStdString(),
                           nganh.toStdString(), hk, list);

    m_curriculumTable->setRowCount(0);
    auto* node = list.getHead();
    while (node) {
        const Course& c = node->data;
        int row = m_curriculumTable->rowCount();
        m_curriculumTable->insertRow(row);

        auto setCell = [&](int col, const QString& val) {
            auto* item = new QTableWidgetItem(val);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_curriculumTable->setItem(row, col, item);
        };

        setCell(0, QString::fromStdString(c.getMaMon()));
        setCell(1, QString::fromStdString(c.getTenMon()));
        setCell(2, QString::number(c.getSoTinChi()));
        setCell(3, QString::fromStdString(c.getKhoa()));
        setCell(4, QString::fromStdString(c.getKhoaHoc()));
        setCell(5, QString::fromStdString(c.getNganhHoc()));
        setCell(6, QString("Kỳ %1").arg(c.getHocKy()));

        node = node->next;
    }
    m_curriculumTable->resizeColumnsToContents();
}

/**
 * @brief Dat lai tat ca gia tri trong bo loc Khung chuong trinh dao tao ve mac dinh (Xem tat ca).
 */
void AdminWindow::onCurriculumReset()
{
    if (m_currKhoaCombo)   m_currKhoaCombo->setCurrentIndex(0);
    if (m_currKhoaHocEdit) m_currKhoaHocEdit->clear();
    if (m_currNganhEdit)   m_currNganhEdit->clear();
    if (m_currHocKyCombo)  m_currHocKyCombo->setCurrentIndex(0);
    onCurriculumFilter();
}

void AdminWindow::setupRegistrationsTab()
{
    m_registrationsTab = new QWidget;
    m_registrationsTab->setStyleSheet("background-color: #FFFFFF;");
    auto* regLayout = new QVBoxLayout(m_registrationsTab);
    regLayout->setContentsMargins(12, 12, 12, 12);

    auto* regHeaderLayout = new QHBoxLayout;
    m_regTitle = new QLabel(
        QString("<b>Danh sách đăng ký học phần - Học kỳ %1</b>")
            .arg(m_system.getCurrentSemester()));
    m_regTitle->setStyleSheet("font-size:14px; color:#1E40AF; margin-bottom:4px; background:transparent;");
    auto* refreshBtn = new QPushButton("Làm mới");
    refreshBtn->setMaximumWidth(130);
    regHeaderLayout->addWidget(m_regTitle);
    regHeaderLayout->addStretch();
    regHeaderLayout->addWidget(refreshBtn);
    regLayout->addLayout(regHeaderLayout);

    m_registrationsTable = new QTableWidget(0, 4);
    m_registrationsTable->setHorizontalHeaderLabels(
        {"Mã đăng ký", "Mã sinh viên", "Mã lớp HP", "Ngày đăng ký"});
    m_registrationsTable->horizontalHeader()->setStretchLastSection(true);
    m_registrationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_registrationsTable->verticalHeader()->setVisible(false);
    m_registrationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_registrationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_registrationsTable->setAlternatingRowColors(true);
    m_registrationsTable->setSortingEnabled(true);

    m_registrationsTable->setColumnWidth(0, 180);
    m_registrationsTable->setColumnWidth(1, 160);
    m_registrationsTable->setColumnWidth(2, 160);

    regLayout->addWidget(m_registrationsTable);

    connect(refreshBtn, &QPushButton::clicked, this, &AdminWindow::refreshRegistrationsTab);
}

void AdminWindow::setupSettingsTab()
{
    m_settingsTab = new QWidget;
    m_settingsTab->setStyleSheet("background-color: #FFFFFF;");
    auto* settingsLayout = new QVBoxLayout(m_settingsTab);
    settingsLayout->setContentsMargins(20, 20, 20, 20);
    settingsLayout->setAlignment(Qt::AlignTop);

    auto* semGroup = new QGroupBox("Cập nhật học kỳ hiện tại", m_settingsTab);
    auto* semBoxLayout = new QVBoxLayout(semGroup);
    semBoxLayout->setSpacing(12);

    m_semesterBadge = new QLabel(
        QString("Học kỳ đang áp dụng: <b>Học kỳ %1</b>").arg(m_system.getCurrentSemester()));
    m_semesterBadge->setStyleSheet(
        "font-size:13px; color:#1E40AF; background-color:#EFF6FF; "
        "border:1px solid #BFDBFE; border-radius:6px; padding:8px 12px;");
    semBoxLayout->addWidget(m_semesterBadge);

    auto* semLayout = new QHBoxLayout;
    semLayout->setSpacing(12);

    auto* semLabel = new QLabel("Chọn học kỳ mới:");
    semLabel->setStyleSheet("font-size:13px; color:#334155; background:transparent;");

    m_semesterSpinBox = new QSpinBox;
    m_semesterSpinBox->setRange(1, 8);
    m_semesterSpinBox->setValue(m_system.getCurrentSemester());
    m_semesterSpinBox->setPrefix("Học kỳ ");
    m_semesterSpinBox->setFixedWidth(120);
    m_semesterSpinBox->setMinimumHeight(36);
    m_semesterSpinBox->setStyleSheet(
        "QSpinBox { background-color: #FFFFFF; color: #1E293B; border:1px solid #CBD5E1; "
        "border-radius:6px; padding:4px 8px; font-size:13px; font-weight:600; }"
        "QSpinBox:focus { border:2px solid #2563EB; }");

    auto* confirmBtn = new QPushButton("Xác nhận");
    confirmBtn->setCursor(Qt::PointingHandCursor);
    confirmBtn->setMinimumHeight(36);
    confirmBtn->setStyleSheet(
        "QPushButton { background-color:#16A34A; color:#FFFFFF; border:none; "
        "border-radius:6px; padding:6px 18px; font-size:13px; font-weight:bold; }"
        "QPushButton:hover { background-color:#15803D; }"
        "QPushButton:pressed { background-color:#166534; }");

    semLayout->addWidget(semLabel);
    semLayout->addWidget(m_semesterSpinBox);
    semLayout->addWidget(confirmBtn);
    semLayout->addStretch();
    semBoxLayout->addLayout(semLayout);

    connect(confirmBtn, &QPushButton::clicked, this, &AdminWindow::onApplySemester);

    settingsLayout->addWidget(semGroup);
    settingsLayout->addStretch();
}

void AdminWindow::refreshRegistrationsTab()
{
    if (!m_registrationsTable) return;

    int currentSem = m_system.getCurrentSemester();
    if (m_regTitle) {
        m_regTitle->setText(
            QString("<b>Danh sách đăng ký học phần - Học kỳ %1</b>").arg(currentSem));
    }

    DoubleLinkedList<Registration> regs;
    m_system.getAllRegistrations(regs);

    m_registrationsTable->setSortingEnabled(false);
    m_registrationsTable->setRowCount(0);

    auto* node = regs.getHead();
    while (node) {
        const Registration& r = node->data;
        const ClassSection* sec = m_system.findClassSection(r.getMaLopHP());
        if (!sec || sec->getHocKy() != currentSem) {
            node = node->next;
            continue;
        }
        int row = m_registrationsTable->rowCount();
        m_registrationsTable->insertRow(row);

        m_registrationsTable->setItem(row, 0,
            new QTableWidgetItem(QString::fromStdString(r.getMaDangKy())));
        m_registrationsTable->setItem(row, 1,
            new QTableWidgetItem(QString::fromStdString(r.getMaSV())));
        m_registrationsTable->setItem(row, 2,
            new QTableWidgetItem(QString::fromStdString(r.getMaLopHP())));
        m_registrationsTable->setItem(row, 3,
            new QTableWidgetItem(QString::fromStdString(r.getNgayDangKy())));

        node = node->next;
    }

    m_registrationsTable->setSortingEnabled(true);
}

void AdminWindow::onLogout()
{
    auto res = QMessageBox::question(
        this, "Xác nhận đăng xuất",
        "Bạn có chắc chắn muốn đăng xuất khỏi tài khoản Quản trị viên?",
        QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit logout();
    }
}

void AdminWindow::onTabChanged(int index)
{
    QWidget* current = m_tabWidget->widget(index);
    if (current == m_sectionWidget && m_sectionWidget) {
        m_sectionWidget->refreshTable();
    }
    if (current == m_curriculumTab) {
        onCurriculumFilter();
    }
    if (current == m_registrationsTab) {
        refreshRegistrationsTab();
    }
    if (current == m_settingsTab && m_semesterSpinBox) {
        m_semesterSpinBox->setValue(m_system.getCurrentSemester());
        if (m_semesterBadge) {
            m_semesterBadge->setText(
                QString("Học kỳ đang áp dụng: <b>Học kỳ %1</b>").arg(m_system.getCurrentSemester()));
        }
    }
}

void AdminWindow::onApplySemester()
{
    if (!m_semesterSpinBox) return;
    int sem = m_semesterSpinBox->value();
    auto confirm = QMessageBox::question(
        this, "Xác nhận cập nhật học kỳ",
        QString("Bạn có chắc chắn muốn cập nhật học kỳ hiện tại thành Học kỳ %1?").arg(sem),
        QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    if (m_system.setCurrentSemester(sem)) {
        if (m_semesterBadge) {
            m_semesterBadge->setText(
                QString("Học kỳ đang áp dụng: <b>Học kỳ %1</b>").arg(sem));
        }
        std::string err;
        m_system.saveData(err);
        refreshRegistrationsTab();
        emit semesterChanged(sem);
        QMessageBox::information(this, "Cài đặt",
            QString("Đã cập nhật học kỳ hiện tại thành: Học kỳ %1").arg(sem));
    } else {
        QMessageBox::warning(this, "Cài đặt",
            "Không thể cập nhật học kỳ. Giá trị hợp lệ là 1 hoặc 2.");
    }
}
