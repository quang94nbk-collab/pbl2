#include "AdminWindow.h"

#include "StudentManagementWidget.h"
#include "CourseManagementWidget.h"
#include "SectionManagementWidget.h"

#include "../manager/system.h"
#include "../model/classsection.h"
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

AdminWindow::AdminWindow(SystemManager& system, QWidget* parent)
    : QWidget(parent)
    , m_system(system)
    , m_tabWidget(nullptr)
    , m_studentWidget(nullptr)
    , m_courseWidget(nullptr)
    , m_sectionWidget(nullptr)
    , m_registrationsTable(nullptr)
    , m_regTitle(nullptr)
    , m_semesterSpinBox(nullptr)
    , m_semesterBadge(nullptr)
{
    setupUi();

    // Soft light-blue & white stylesheet (no dark/black background)
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

    // ---- Tab 3: Lớp học phần ----
    m_sectionWidget = new SectionManagementWidget(m_system, this);
    m_tabWidget->addTab(m_sectionWidget, "Lớp học phần");

    // ---- Tab 4: Đăng ký (admin view - loc theo hoc ky hien tai) ----
    auto* regTab = new QWidget;
    regTab->setStyleSheet("background-color: #FFFFFF;");
    auto* regLayout = new QVBoxLayout(regTab);
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

    setupRegistrationsTab();
    regLayout->addWidget(m_registrationsTable);
    m_tabWidget->addTab(regTab, "Đăng ký");

    connect(refreshBtn, &QPushButton::clicked, this, &AdminWindow::refreshRegistrationsTab);

    // ---- Tab 5: Cài đặt ----
    auto* settingsTab = new QWidget;
    settingsTab->setStyleSheet("background-color: #FFFFFF;");
    auto* settingsLayout = new QVBoxLayout(settingsTab);
    settingsLayout->setContentsMargins(20, 20, 20, 20);
    settingsLayout->setAlignment(Qt::AlignTop);

    auto* semGroup = new QGroupBox("Cập nhật học kỳ hiện tại", settingsTab);
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
    m_semesterSpinBox->setRange(1, 2);
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

    m_tabWidget->addTab(settingsTab, "Cài đặt");

    mainLayout->addWidget(m_tabWidget);
    outerLayout->addWidget(contentWidget, 1);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &AdminWindow::onTabChanged);

    // Load data into registrations tab on startup
    refreshRegistrationsTab();
}

void AdminWindow::setupRegistrationsTab()
{
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

    // Set column widths
    m_registrationsTable->setColumnWidth(0, 180);
    m_registrationsTable->setColumnWidth(1, 160);
    m_registrationsTable->setColumnWidth(2, 160);
}

void AdminWindow::setupSettingsTab()
{
    // Widget creation is handled inline in setupUi().
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
    if (index == 2 && m_sectionWidget) {
        m_sectionWidget->refreshTable();
    }
    if (index == 3) {
        refreshRegistrationsTab();
    }
    if (index == 4 && m_semesterSpinBox) {
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
