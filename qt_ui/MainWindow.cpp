#include "MainWindow.h"
#include "AdminWindow.h"
#include "StudentWindow.h"

#include "../manager/system.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <string>

MainWindow::MainWindow(SystemManager& system, AuthManager& auth, QWidget* parent)
    : QMainWindow(parent)
    , m_system(system)
    , m_auth(auth)
    , m_logoutRequested(false)
{
    setWindowTitle("Hệ thống Đăng ký Tín chỉ - PBL2");
    resize(1240, 820);
    setMinimumSize(960, 640);

    // ---- Menu bar ----
    QMenu* fileMenu = menuBar()->addMenu("&Hệ thống");

    QAction* logoutAction = new QAction("Đăng xuất", this);
    logoutAction->setShortcut(QKeySequence("Ctrl+L"));
    fileMenu->addAction(logoutAction);
    fileMenu->addSeparator();

    QAction* exitAction = new QAction("Thoát", this);
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);

    connect(logoutAction, &QAction::triggered, this, &MainWindow::performLogout);
    connect(exitAction,   &QAction::triggered, this, &MainWindow::close);

    statusBar()->showMessage("Sẵn sàng");

    // Light, clean chrome stylesheet (no dark/black background)
    setStyleSheet(R"(
        QMainWindow {
            background-color: #F3F6FB;
            color: #1E293B;
        }
        QMenuBar {
            background-color: #1E40AF;
            color: #FFFFFF;
            font-size: 13px;
            padding: 2px 6px;
        }
        QMenuBar::item {
            background: transparent;
            padding: 4px 10px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background-color: #2563EB;
        }
        QMenu {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
        }
        QMenu::item {
            padding: 6px 20px;
        }
        QMenu::item:selected {
            background-color: #EFF6FF;
            color: #1D4ED8;
        }
        QStatusBar {
            background-color: #EFF6FF;
            color: #475569;
            border-top: 1px solid #DBEAFE;
            font-size: 12px;
        }
    )");
}

void MainWindow::showForRole(UserRole role, const QString& studentID)
{
    m_logoutRequested = false;
    if (role == UserRole::Administrator) {
        auto* adminWin = new AdminWindow(m_system, this);
        connect(adminWin, &AdminWindow::logout, this, &MainWindow::performLogout);
        connect(adminWin, &AdminWindow::semesterChanged, this, [this](int sem) {
            statusBar()->showMessage(
                QString("Đăng nhập với quyền: Quản trị viên (Admin)  |  Học kỳ hiện tại: %1")
                    .arg(sem));
        });
        setCentralWidget(adminWin);
        statusBar()->showMessage(
            QString("Đăng nhập với quyền: Quản trị viên (Admin)  |  Học kỳ hiện tại: %1")
                .arg(m_system.getCurrentSemester()));
    } else {
        auto* studentWin = new StudentWindow(m_system, studentID, this);
        connect(studentWin, &StudentWindow::logout, this, &MainWindow::performLogout);
        setCentralWidget(studentWin);
        statusBar()->showMessage(
            QString("Đăng nhập với quyền: Sinh viên [%1]  |  Học kỳ hiện tại: %2")
                .arg(studentID)
                .arg(m_system.getCurrentSemester()));
    }
}

bool MainWindow::isLogoutRequested() const
{
    return m_logoutRequested;
}

void MainWindow::performLogout()
{
    std::string err;
    m_system.saveData(err);
    m_auth.logout();
    m_logoutRequested = true;
    close();
}
