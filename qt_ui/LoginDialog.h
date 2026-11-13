#pragma once

#include <QDialog>
#include <QString>

#include "../manager/authmanager.h"

class QLineEdit;
class QPushButton;
class QLabel;
class QButtonGroup;

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(AuthManager& auth, QWidget* parent = nullptr);

    UserRole getRole() const;
    QString  getStudentID() const;

private slots:
    void onRoleChanged();
    void onLoginClicked();
    void onExitClicked();

private:
    void setupUi();
    void applyStyles();

    AuthManager&  m_auth;
    UserRole      m_role;
    UserRole      m_selectedRole;
    QString       m_studentID;

    QPushButton*  m_studentRoleBtn;
    QPushButton*  m_adminRoleBtn;
    QButtonGroup* m_roleGroup;
    QLabel*       m_hintLabel;
    QLineEdit*    m_usernameEdit;
    QLineEdit*    m_passwordEdit;
    QPushButton*  m_loginBtn;
    QPushButton*  m_exitBtn;
    QLabel*       m_errorLabel;
};
