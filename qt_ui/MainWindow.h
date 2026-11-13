#pragma once

#include <QMainWindow>
#include <QString>

#include "../manager/authmanager.h"

class SystemManager;
class AdminWindow;
class StudentWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(SystemManager& system, AuthManager& auth,
                        QWidget* parent = nullptr);

    // Sets the central widget based on the user role.
    // Call this after the LoginDialog is accepted.
    void showForRole(UserRole role, const QString& studentID);
    bool isLogoutRequested() const;

private slots:
    void performLogout();

private:
    SystemManager& m_system;
    AuthManager&   m_auth;
    bool           m_logoutRequested;
};
