#include <QApplication>
#include <QMessageBox>
#include <QPalette>
#include <QColor>
#include <QStyleFactory>
#include <QString>

#include "LoginDialog.h"
#include "MainWindow.h"
#include "AdminWindow.h"
#include "StudentWindow.h"

#include "../manager/system.h"
#include "../manager/authmanager.h"

namespace {

void applyPleasantLightTheme(QApplication& app) {
    // Ep kieu Fusion + Palette sang de khong bi nen den khi Windows o Dark Mode
    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette light;
    light.setColor(QPalette::Window,          QColor("#F3F6FB"));
    light.setColor(QPalette::WindowText,      QColor("#1E293B"));
    light.setColor(QPalette::Base,            QColor("#FFFFFF"));
    light.setColor(QPalette::AlternateBase,   QColor("#F8FAFC"));
    light.setColor(QPalette::ToolTipBase,     QColor("#FFFFFF"));
    light.setColor(QPalette::ToolTipText,     QColor("#1E293B"));
    light.setColor(QPalette::Text,            QColor("#1E293B"));
    light.setColor(QPalette::Button,          QColor("#FFFFFF"));
    light.setColor(QPalette::ButtonText,      QColor("#1E293B"));
    light.setColor(QPalette::BrightText,      QColor("#FFFFFF"));
    light.setColor(QPalette::Link,            QColor("#2563EB"));
    light.setColor(QPalette::Highlight,       QColor("#2563EB"));
    light.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    light.setColor(QPalette::PlaceholderText, QColor("#94A3B8"));

    light.setColor(QPalette::Disabled, QPalette::Text,       QColor("#94A3B8"));
    light.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#94A3B8"));
    app.setPalette(light);

    app.setStyleSheet(R"(
        QWidget {
            font-family: 'Segoe UI', Arial, sans-serif;
            color: #1E293B;
        }
        QDialog, QMessageBox {
            background-color: #F8FAFC;
            color: #1E293B;
        }
        QMessageBox QLabel {
            color: #1E293B;
        }
        QMessageBox QPushButton {
            background-color: #2563EB;
            color: #FFFFFF;
            border: none;
            border-radius: 5px;
            padding: 6px 18px;
            min-width: 72px;
            font-weight: 600;
        }
        QMessageBox QPushButton:hover {
            background-color: #3B82F6;
        }
        QComboBox {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 5px 10px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
            color: #1E293B;
            selection-background-color: #DBEAFE;
            selection-color: #1E3A8A;
        }
        QSpinBox {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 4px 8px;
        }
    )");
}

void rebuildAuthAccounts(SystemManager& system, AuthManager& auth) {
    auth = AuthManager();
    auth.addAccount("admin", "admin", UserRole::Administrator);

    try {
        DoubleLinkedList<Student> students;
        system.getAllStudents(students);
        auto node = students.getHead();
        while (node != nullptr) {
            const std::string& sid = node->data.getMaSV();
            auth.addAccount(sid, sid, UserRole::Student, sid);
            node = node->next;
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr,
            "Cảnh báo",
            QString("Không thể tải danh sách sinh viên vào AuthManager: %1")
                .arg(QString::fromStdString(e.what())));
    }
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("PBL2 - Hệ thống Đăng ký Tín chỉ");
    app.setApplicationVersion("1.0");
    applyPleasantLightTheme(app);

    // ----------------------------------------------------------------
    // 1. Khởi tạo SystemManager và load dữ liệu
    // ----------------------------------------------------------------
    SystemManager system;
    std::string loadError;
    if (!system.loadData(loadError)) {
        if (!loadError.empty()) {
            QMessageBox::warning(nullptr,
                "Cảnh báo",
                QString("Không thể tải dữ liệu: %1\nHệ thống sẽ khởi động với dữ liệu rỗng.")
                    .arg(QString::fromStdString(loadError)));
        }
    }

    AuthManager auth;

    // ----------------------------------------------------------------
    // 2. Vòng lặp đăng nhập -> cửa sổ chính -> đăng xuất
    // ----------------------------------------------------------------
    int exitCode = 0;
    while (true) {
        rebuildAuthAccounts(system, auth);

        LoginDialog loginDlg(auth);
        if (loginDlg.exec() != QDialog::Accepted) {
            break;
        }

        UserRole role     = loginDlg.getRole();
        QString studentID = loginDlg.getStudentID();

        MainWindow mainWin(system, auth);
        try {
            mainWin.showForRole(role, studentID);
        } catch (const AppException& e) {
            QMessageBox::critical(nullptr,
                "Lỗi",
                QString("Lỗi khởi động giao diện: %1")
                    .arg(QString::fromStdString(e.what())));
            exitCode = 1;
            break;
        } catch (const std::exception& e) {
            QMessageBox::critical(nullptr,
                "Lỗi",
                QString("Lỗi không xác định: %1")
                    .arg(QString::fromStdString(e.what())));
            exitCode = 1;
            break;
        }

        mainWin.show();
        exitCode = app.exec();

        if (!mainWin.isLogoutRequested()) {
            break;
        }
    }

    // ----------------------------------------------------------------
    // 3. Lưu dữ liệu trước khi thoát
    // ----------------------------------------------------------------
    std::string saveError;
    if (!system.saveData(saveError)) {
        QMessageBox::critical(nullptr,
            "Lỗi lưu dữ liệu",
            QString("Không thể lưu dữ liệu: %1")
                .arg(QString::fromStdString(saveError)));
    }

    return exitCode;
}
