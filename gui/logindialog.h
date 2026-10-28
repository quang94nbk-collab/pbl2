#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <string>

#include "../manager/system.h"

class LoginDialog : public QDialog {
    Q_OBJECT

private:
    SystemManager& system;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QRadioButton* radStudent;
    QRadioButton* radAdmin;
    QPushButton* btnLogin;
    QPushButton* btnExit;
    QLabel* lblStatus;

    bool adminRole;
    std::string loggedStudentID;

private slots:
    void onRoleChanged();
    void onLoginClicked();

public:
    explicit LoginDialog(SystemManager& sys, QWidget* parent = nullptr);

    bool isAdministrator() const { return adminRole; }
    std::string getLoggedInStudentID() const { return loggedStudentID; }
};

#endif

