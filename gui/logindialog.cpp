#include "logindialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

LoginDialog::LoginDialog(SystemManager& sys, QWidget* parent)
    : QDialog(parent), system(sys), adminRole(false), loggedStudentID("") {

    setWindowTitle("Hệ Thống Đăng Ký Học Phần - Đăng Nhập");
    setFixedSize(420, 280);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 20, 25, 20);

    QLabel* lblTitle = new QLabel("CỔNG THÔNG TIN ĐÀO TẠO", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    QFont titleFont = lblTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    lblTitle->setFont(titleFont);
    lblTitle->setStyleSheet("color: #1a73e8;");
    mainLayout->addWidget(lblTitle);

    // Lua chon vai tro
    QHBoxLayout* roleLayout = new QHBoxLayout();
    radStudent = new QRadioButton("Sinh viên", this);
    radAdmin = new QRadioButton("Quản trị viên", this);
    radStudent->setChecked(true);
    roleLayout->addStretch();
    roleLayout->addWidget(radStudent);
    roleLayout->addSpacing(20);
    roleLayout->addWidget(radAdmin);
    roleLayout->addStretch();
    mainLayout->addLayout(roleLayout);

    // Form dang nhap
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);

    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Nhập mã sinh viên (Ví dụ: 10222001)");
    txtUsername->setMinimumHeight(30);

    txtPassword = new QLineEdit(this);
    txtPassword->setEchoMode(QLineEdit::Password);
    txtPassword->setPlaceholderText("Mật khẩu (mặc định để trống với SV)");
    txtPassword->setMinimumHeight(30);

    formLayout->addRow("Tài khoản / Mã SV:", txtUsername);
    formLayout->addRow("Mật khẩu:", txtPassword);
    mainLayout->addLayout(formLayout);

    lblStatus = new QLabel("", this);
    lblStatus->setStyleSheet("color: #d93025;");
    lblStatus->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblStatus);

    // Cac nut bam
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLogin = new QPushButton("Đăng nhập", this);
    btnLogin->setMinimumHeight(34);
    btnLogin->setStyleSheet("background-color: #1a73e8; color: white; font-weight: bold; border-radius: 4px;");

    btnExit = new QPushButton("Thoát", this);
    btnExit->setMinimumHeight(34);

    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnExit);
    mainLayout->addLayout(btnLayout);

    connect(radStudent, &QRadioButton::toggled, this, &LoginDialog::onRoleChanged);
    connect(radAdmin, &QRadioButton::toggled, this, &LoginDialog::onRoleChanged);
    connect(btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(btnExit, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginDialog::onRoleChanged() {
    lblStatus->clear();
    if (radStudent->isChecked()) {
        txtUsername->setPlaceholderText("Nhập mã sinh viên (Ví dụ: 10222001)");
        txtPassword->setPlaceholderText("Mật khẩu (mặc định để trống với SV)");
    } else {
        txtUsername->setPlaceholderText("Tài khoản admin (mặc định: admin)");
        txtPassword->setPlaceholderText("Mật khẩu admin (mặc định: admin)");
    }
}

void LoginDialog::onLoginClicked() {
    std::string user = txtUsername->text().trimmed().toStdString();
    std::string pass = txtPassword->text().trimmed().toStdString();

    if (user.empty()) {
        lblStatus->setText("Vui lòng nhập tài khoản hoặc mã sinh viên!");
        return;
    }

    if (radAdmin->isChecked()) {
        if (user == "admin" && (pass == "admin" || pass.empty())) {
            adminRole = true;
            loggedStudentID = "";
            accept();
        } else {
            lblStatus->setText("Sai tài khoản hoặc mật khẩu quản trị viên!");
        }
    } else {
        // Kiem tra quy uoc Ma sinh vien (8 chu so, 3 so dau 100-103)
        if (!Student::isValidMaSV(user)) {
            lblStatus->setText("Mã sinh viên phải gồm 8 chữ số (bắt đầu bằng 100 -> 103)!");
            return;
        }

        Student* sv = system.findStudent(user);
        if (sv == nullptr) {
            // Neu sinh vien chua co trong he thong, tao sinh vien mau de tien demo
            string tenKhoa = "";
            string prefix = user.substr(0, 3);
            if (prefix == "100") tenKhoa = "Khoa Co ban / Dai cuong";
            else if (prefix == "101") tenKhoa = "Khoa Co khi";
            else if (prefix == "102") tenKhoa = "Khoa Cong nghe Thong tin";
            else if (prefix == "103") tenKhoa = "Khoa Dien - Dien tu";

            Student newSV(user, "Sinh viên " + user, "0900000000", user + "@dut.udn.vn", "22T_" + prefix, tenKhoa);
            system.addStudent(newSV);
        }

        adminRole = false;
        loggedStudentID = user;
        accept();
    }
}

