#include "LoginDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QFrame>
#include <QApplication>
#include <QScreen>

LoginDialog::LoginDialog(AuthManager& auth, QWidget* parent)
    : QDialog(parent)
    , m_auth(auth)
    , m_role(UserRole::Student)
    , m_selectedRole(UserRole::Student)
    , m_studentRoleBtn(nullptr)
    , m_adminRoleBtn(nullptr)
    , m_roleGroup(nullptr)
    , m_hintLabel(nullptr)
{
    setWindowTitle("Đăng nhập - Hệ thống Đăng ký Tín chỉ PBL2");
    setFixedSize(460, 580);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setupUi();
    applyStyles();
    onRoleChanged();

    // Center on screen
    if (QScreen* screen = QApplication::primaryScreen()) {
        QRect sg = screen->availableGeometry();
        move(sg.center() - rect().center());
    }
}

void LoginDialog::setupUi()
{
    // ---- Outer layout ----
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // ---- Header banner ----
    auto* headerFrame = new QFrame;
    headerFrame->setObjectName("headerFrame");
    headerFrame->setFixedHeight(115);
    auto* headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 12, 20, 12);
    headerLayout->setAlignment(Qt::AlignCenter);

    auto* titleLabel = new QLabel("Hệ thống Đăng ký Tín chỉ");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto* subTitleLabel = new QLabel("PBL2 – Đại học Bách Khoa Đà Nẵng");
    subTitleLabel->setObjectName("subTitleLabel");
    subTitleLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(subTitleLabel);
    outerLayout->addWidget(headerFrame);

    // ---- Card frame ----
    auto* cardFrame = new QFrame;
    cardFrame->setObjectName("cardFrame");
    auto* cardLayout = new QVBoxLayout(cardFrame);
    cardLayout->setContentsMargins(32, 24, 32, 24);
    cardLayout->setSpacing(14);

    auto* loginLabel = new QLabel("Chọn vai trò & Đăng nhập");
    loginLabel->setObjectName("loginLabel");
    loginLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(loginLabel);

    // ---- Role Selector ----
    auto* roleRow = new QHBoxLayout;
    roleRow->setSpacing(8);

    m_studentRoleBtn = new QPushButton("Sinh viên");
    m_studentRoleBtn->setObjectName("roleBtn");
    m_studentRoleBtn->setCheckable(true);
    m_studentRoleBtn->setChecked(true);
    m_studentRoleBtn->setMinimumHeight(40);
    m_studentRoleBtn->setCursor(Qt::PointingHandCursor);

    m_adminRoleBtn = new QPushButton("Quản trị viên (Admin)");
    m_adminRoleBtn->setObjectName("roleBtn");
    m_adminRoleBtn->setCheckable(true);
    m_adminRoleBtn->setMinimumHeight(40);
    m_adminRoleBtn->setCursor(Qt::PointingHandCursor);

    m_roleGroup = new QButtonGroup(this);
    m_roleGroup->setExclusive(true);
    m_roleGroup->addButton(m_studentRoleBtn, 0);
    m_roleGroup->addButton(m_adminRoleBtn, 1);

    roleRow->addWidget(m_studentRoleBtn, 1);
    roleRow->addWidget(m_adminRoleBtn, 1);
    cardLayout->addLayout(roleRow);

    m_hintLabel = new QLabel;
    m_hintLabel->setObjectName("hintLabel");
    m_hintLabel->setAlignment(Qt::AlignCenter);
    m_hintLabel->setWordWrap(true);
    cardLayout->addWidget(m_hintLabel);

    // ---- Form ----
    auto* formLayout = new QFormLayout;
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setObjectName("inputField");
    m_usernameEdit->setMinimumHeight(40);

    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setObjectName("inputField");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Nhập mật khẩu...");
    m_passwordEdit->setMinimumHeight(40);

    formLayout->addRow("Tài khoản:", m_usernameEdit);
    formLayout->addRow("Mật khẩu:", m_passwordEdit);
    cardLayout->addLayout(formLayout);

    // Error label
    m_errorLabel = new QLabel;
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();
    cardLayout->addWidget(m_errorLabel);

    // Buttons
    m_loginBtn = new QPushButton("Đăng nhập");
    m_loginBtn->setObjectName("loginBtn");
    m_loginBtn->setMinimumHeight(42);
    m_loginBtn->setCursor(Qt::PointingHandCursor);
    m_loginBtn->setDefault(true);

    m_exitBtn = new QPushButton("Thoát chương trình");
    m_exitBtn->setObjectName("exitBtn");
    m_exitBtn->setMinimumHeight(40);
    m_exitBtn->setCursor(Qt::PointingHandCursor);

    cardLayout->addWidget(m_loginBtn);
    cardLayout->addWidget(m_exitBtn);

    outerLayout->addWidget(cardFrame, 1);

    // ---- Footer ----
    auto* footerLabel = new QLabel("© PBL2 – Hệ thống Quản lý & Đăng ký Tín chỉ");
    footerLabel->setObjectName("footerLabel");
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setContentsMargins(0, 4, 0, 10);
    outerLayout->addWidget(footerLabel);

    // ---- Connections ----
    connect(m_studentRoleBtn, &QPushButton::clicked, this, &LoginDialog::onRoleChanged);
    connect(m_adminRoleBtn,   &QPushButton::clicked, this, &LoginDialog::onRoleChanged);
    connect(m_loginBtn,       &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_exitBtn,        &QPushButton::clicked, this, &LoginDialog::onExitClicked);
    connect(m_passwordEdit,   &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(m_usernameEdit,   &QLineEdit::returnPressed, m_passwordEdit, [this]{ m_passwordEdit->setFocus(); });
}

void LoginDialog::applyStyles()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #F3F6FB;
        }
        #headerFrame {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #2563EB, stop:1 #1D4ED8);
            border-bottom: 3px solid #1E40AF;
        }
        #titleLabel {
            color: #FFFFFF;
            font-size: 19px;
            font-weight: bold;
            font-family: 'Segoe UI', Arial, sans-serif;
            background: transparent;
        }
        #subTitleLabel {
            color: #DBEAFE;
            font-size: 12px;
            font-family: 'Segoe UI', Arial, sans-serif;
            background: transparent;
        }
        #cardFrame {
            background-color: #FFFFFF;
            margin: 18px;
            border-radius: 12px;
            border: 1px solid #E2E8F0;
        }
        #loginLabel {
            font-size: 16px;
            font-weight: bold;
            color: #1E3A8A;
            background: transparent;
        }
        #roleBtn {
            background-color: #EFF6FF;
            color: #1E40AF;
            border: 1.5px solid #BFDBFE;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 600;
            padding: 6px 10px;
        }
        #roleBtn:hover:!checked {
            background-color: #DBEAFE;
            border-color: #93C5FD;
        }
        #roleBtn:checked {
            background-color: #2563EB;
            color: #FFFFFF;
            border: 1.5px solid #1D4ED8;
        }
        #hintLabel {
            color: #475569;
            background-color: #F8FAFC;
            border: 1px solid #E2E8F0;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 11px;
        }
        QLabel {
            font-size: 13px;
            color: #334155;
            font-family: 'Segoe UI', Arial, sans-serif;
            background: transparent;
        }
        #inputField {
            border: 1.5px solid #CBD5E1;
            border-radius: 7px;
            padding: 6px 12px;
            font-size: 13px;
            background-color: #FFFFFF;
            color: #1E293B;
        }
        #inputField:focus {
            border: 2px solid #2563EB;
            background-color: #FFFFFF;
        }
        #loginBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #3B82F6, stop:1 #2563EB);
            color: #FFFFFF;
            border: none;
            border-radius: 7px;
            font-size: 14px;
            font-weight: bold;
        }
        #loginBtn:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #60A5FA, stop:1 #3B82F6);
        }
        #loginBtn:pressed {
            background: #1D4ED8;
        }
        #exitBtn {
            background-color: #F8FAFC;
            color: #475569;
            border: 1px solid #CBD5E1;
            border-radius: 7px;
            font-size: 13px;
        }
        #exitBtn:hover {
            background-color: #FEF2F2;
            color: #DC2626;
            border-color: #FCA5A5;
        }
        #exitBtn:pressed {
            background-color: #FEE2E2;
        }
        #errorLabel {
            color: #B91C1C;
            font-size: 12px;
            background-color: #FEF2F2;
            border: 1px solid #FECACA;
            border-radius: 6px;
            padding: 7px;
        }
        #footerLabel {
            color: #64748B;
            font-size: 11px;
            background: transparent;
        }
    )");
}

void LoginDialog::onRoleChanged()
{
    m_errorLabel->hide();
    if (m_adminRoleBtn->isChecked()) {
        m_selectedRole = UserRole::Administrator;
        m_usernameEdit->setPlaceholderText("Nhập tài khoản Admin (VD: admin)...");
        m_hintLabel->setText("Đang chọn quyền Quản trị viên • Mặc định: admin / admin");
    } else {
        m_selectedRole = UserRole::Student;
        m_usernameEdit->setPlaceholderText("Nhập mã sinh viên (VD: SV001)...");
        m_hintLabel->setText("Đang chọn quyền Sinh viên • Tài khoản & mật khẩu mặc định là Mã SV");
    }
    m_usernameEdit->setFocus();
}

void LoginDialog::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Vui lòng nhập đầy đủ tài khoản và mật khẩu.");
        m_errorLabel->show();
        return;
    }

    bool ok = m_auth.login(username.toStdString(), password.toStdString());
    if (!ok) {
        m_errorLabel->setText("Tài khoản hoặc mật khẩu không đúng. Vui lòng thử lại.");
        m_errorLabel->show();
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
        return;
    }

    const UserAccount* user = m_auth.currentUser();
    if (!user || user->role != m_selectedRole) {
        m_auth.logout();
        if (m_selectedRole == UserRole::Student) {
            m_errorLabel->setText("Bạn đang chọn quyền Sinh viên.\nVui lòng chuyển sang 'Quản trị viên (Admin)' để đăng nhập tài khoản này.");
        } else {
            m_errorLabel->setText("Bạn đang chọn quyền Quản trị viên.\nTài khoản này thuộc quyền Sinh viên.");
        }
        m_errorLabel->show();
        return;
    }

    m_role      = user->role;
    m_studentID = QString::fromStdString(user->studentID);
    m_errorLabel->hide();
    accept();
}

void LoginDialog::onExitClicked()
{
    reject();
}

UserRole LoginDialog::getRole() const
{
    return m_role;
}

QString LoginDialog::getStudentID() const
{
    return m_studentID;
}
