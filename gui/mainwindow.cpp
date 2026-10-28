#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(SystemManager& sys, FileManager& fm, const std::string& studentID,
                       bool adminMode, QWidget* parent)
    : QMainWindow(parent), system(sys), fileManager(fm),
      currentStudentID(studentID), isAdmin(adminMode) {

    setWindowTitle("Hệ Thống Quản Lý Đào Tạo & Đăng Ký Học Phần");
    resize(1050, 680);

    setupUI();
    loadUserInfo();
    loadCurriculumTable();
    loadAvailableSectionsTable();
    loadRegisteredSectionsTable();

    if (isAdmin) {
        loadAdminStudentTable();
    }
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(15, 15, 15, 15);
    rootLayout->setSpacing(10);

    // --- TOP BAR: Thong tin nguoi dung & Chuc nang he thong ---
    QHBoxLayout* topBar = new QHBoxLayout();
    lblUserInfo = new QLabel(this);
    QFont userFont = lblUserInfo->font();
    userFont.setPointSize(11);
    userFont.setBold(true);
    lblUserInfo->setFont(userFont);
    lblUserInfo->setStyleSheet("color: #0d47a1; padding: 6px; background: #e3f2fd; border-radius: 4px;");

    btnSaveData = new QPushButton("Lưu dữ liệu", this);
    btnSaveData->setMinimumHeight(32);
    btnSaveData->setStyleSheet("background-color: #2e7d32; color: white; font-weight: bold; border-radius: 4px; padding: 0 12px;");

    btnLogout = new QPushButton("Thoát", this);
    btnLogout->setMinimumHeight(32);
    btnLogout->setStyleSheet("padding: 0 12px;");

    topBar->addWidget(lblUserInfo, 1);
    topBar->addWidget(btnSaveData);
    topBar->addWidget(btnLogout);
    rootLayout->addLayout(topBar);

    // --- TAB WIDGET ---
    tabWidget = new QTabWidget(this);
    rootLayout->addWidget(tabWidget);

    // ==========================================
    // TAB 1: CHUONG TRINH DAO TAO (CTDT)
    // ==========================================
    QWidget* tabCurriculum = new QWidget(this);
    QVBoxLayout* curLayout = new QVBoxLayout(tabCurriculum);

    QHBoxLayout* curFilterLayout = new QHBoxLayout();
    curFilterLayout->addWidget(new QLabel("Chọn Khoa hiển thị CTĐT:", this));
    cboFacultyFilter = new QComboBox(this);
    cboFacultyFilter->addItem("Khoa 100 - Khoa Cơ bản / Đại cương", "100");
    cboFacultyFilter->addItem("Khoa 101 - Khoa Cơ khí", "101");
    cboFacultyFilter->addItem("Khoa 102 - Khoa Công nghệ Thông tin", "102");
    cboFacultyFilter->addItem("Khoa 103 - Khoa Điện - Điện tử", "103");
    cboFacultyFilter->addItem("Tất cả các môn", "ALL");
    curFilterLayout->addWidget(cboFacultyFilter, 1);
    curLayout->addLayout(curFilterLayout);

    tableCurriculum = new QTableWidget(this);
    tableCurriculum->setColumnCount(6);
    tableCurriculum->setHorizontalHeaderLabels({"Mã Môn", "Tên Môn Học", "Số Tín Chỉ", "Học Kỳ", "Loại Môn", "Khoa"});
    tableCurriculum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableCurriculum->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableCurriculum->setSelectionBehavior(QAbstractItemView::SelectRows);
    curLayout->addWidget(tableCurriculum);

    tabWidget->addTab(tabCurriculum, "Chương Trình Đào Tạo");

    // ==========================================
    // TAB 2: DANG KY HOC PHAN
    // ==========================================
    QWidget* tabReg = new QWidget(this);
    QVBoxLayout* regLayout = new QVBoxLayout(tabReg);

    QLabel* lblRegNotice = new QLabel("Danh sách lớp học phần mở (Hệ thống sẽ tự động kiểm tra CTĐT của khoa và trùng lịch):", this);
    lblRegNotice->setStyleSheet("color: #555; font-style: italic;");
    regLayout->addWidget(lblRegNotice);

    tableAvailableSections = new QTableWidget(this);
    tableAvailableSections->setColumnCount(8);
    tableAvailableSections->setHorizontalHeaderLabels({"Mã Lớp HP", "Mã Môn", "Tên Môn", "Số TC", "Giảng Viên", "Thời Gian", "Phòng", "Sĩ Số"});
    tableAvailableSections->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableAvailableSections->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableAvailableSections->setSelectionBehavior(QAbstractItemView::SelectRows);
    regLayout->addWidget(tableAvailableSections);

    QHBoxLayout* regBtnLayout = new QHBoxLayout();
    regBtnLayout->addStretch();
    btnRegister = new QPushButton("Đăng Ký Lớp Đang Chọn", this);
    btnRegister->setMinimumHeight(36);
    btnRegister->setStyleSheet("background-color: #1976d2; color: white; font-weight: bold; border-radius: 4px; padding: 0 20px;");
    regBtnLayout->addWidget(btnRegister);
    regLayout->addLayout(regBtnLayout);

    tabWidget->addTab(tabReg, "Đăng Ký Học Phần");

    // ==========================================
    // TAB 3: LOP DA DANG KY (THOI KHOA BIEU)
    // ==========================================
    QWidget* tabMyClasses = new QWidget(this);
    QVBoxLayout* myLayout = new QVBoxLayout(tabMyClasses);

    tableRegisteredSections = new QTableWidget(this);
    tableRegisteredSections->setColumnCount(7);
    tableRegisteredSections->setHorizontalHeaderLabels({"Mã ĐK", "Mã Lớp HP", "Mã Môn", "Tên Môn", "Thời Gian", "Phòng", "Ngày ĐK"});
    tableRegisteredSections->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableRegisteredSections->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableRegisteredSections->setSelectionBehavior(QAbstractItemView::SelectRows);
    myLayout->addWidget(tableRegisteredSections);

    QHBoxLayout* cancelBtnLayout = new QHBoxLayout();
    cancelBtnLayout->addStretch();
    btnCancelRegistration = new QPushButton("Hủy Lớp Đang Chọn", this);
    btnCancelRegistration->setMinimumHeight(36);
    btnCancelRegistration->setStyleSheet("background-color: #c62828; color: white; font-weight: bold; border-radius: 4px; padding: 0 20px;");
    cancelBtnLayout->addWidget(btnCancelRegistration);
    myLayout->addLayout(cancelBtnLayout);

    tabWidget->addTab(tabMyClasses, "Học Phần Đã Đăng Ký");

    // ==========================================
    // TAB 4: QUAN TRI HE THONG (ADMIN)
    // ==========================================
    if (isAdmin) {
        tabAdmin = new QWidget(this);
        QVBoxLayout* adminLayout = new QVBoxLayout(tabAdmin);

        // Group 1: Them Sinh Vien Moi (Kiem tra 8 so, 100-103)
        QGroupBox* grpAddSV = new QGroupBox("Thêm Sinh Viên Mới (Quy ước: 8 chữ số, 3 số đầu 100-103)", this);
        QGridLayout* gridSV = new QGridLayout(grpAddSV);

        txtAddStudentID = new QLineEdit(this);
        txtAddStudentID->setPlaceholderText("VD: 10222002");
        txtAddStudentName = new QLineEdit(this);
        txtAddStudentPhone = new QLineEdit(this);
        txtAddStudentEmail = new QLineEdit(this);
        txtAddStudentClass = new QLineEdit(this);
        txtAddStudentMajor = new QLineEdit(this);
        btnAddStudent = new QPushButton("Thêm Sinh Viên", this);

        gridSV->addWidget(new QLabel("Mã SV:"), 0, 0);
        gridSV->addWidget(txtAddStudentID, 0, 1);
        gridSV->addWidget(new QLabel("Họ Tên:"), 0, 2);
        gridSV->addWidget(txtAddStudentName, 0, 3);

        gridSV->addWidget(new QLabel("SĐT:"), 1, 0);
        gridSV->addWidget(txtAddStudentPhone, 1, 1);
        gridSV->addWidget(new QLabel("Email:"), 1, 2);
        gridSV->addWidget(txtAddStudentEmail, 1, 3);

        gridSV->addWidget(new QLabel("Lớp:"), 2, 0);
        gridSV->addWidget(txtAddStudentClass, 2, 1);
        gridSV->addWidget(new QLabel("Ngành:"), 2, 2);
        gridSV->addWidget(txtAddStudentMajor, 2, 3);

        gridSV->addWidget(btnAddStudent, 3, 3);
        adminLayout->addWidget(grpAddSV);

        // Group 2: Them mon vao CTDT
        QGroupBox* grpAddCur = new QGroupBox("Gán Môn Vào Chương Trình Đào Tạo Của Khoa", this);
        QHBoxLayout* hCur = new QHBoxLayout(grpAddCur);

        cboAdminFaculty = new QComboBox(this);
        cboAdminFaculty->addItem("Khoa 100 - Đại cương", "100");
        cboAdminFaculty->addItem("Khoa 101 - Cơ khí", "101");
        cboAdminFaculty->addItem("Khoa 102 - CNTT", "102");
        cboAdminFaculty->addItem("Khoa 103 - Điện tử", "103");

        txtAdminCourseID = new QLineEdit(this);
        txtAdminCourseID->setPlaceholderText("Mã môn (VD: CS101)");

        spnAdminSemester = new QSpinBox(this);
        spnAdminSemester->setRange(1, 10);
        spnAdminSemester->setValue(1);

        chkAdminCompulsory = new QCheckBox("Bắt buộc", this);
        chkAdminCompulsory->setChecked(true);

        btnAddCurriculumItem = new QPushButton("Thêm Vào CTĐT", this);

        hCur->addWidget(new QLabel("Khoa:"));
        hCur->addWidget(cboAdminFaculty);
        hCur->addWidget(new QLabel("Mã Môn:"));
        hCur->addWidget(txtAdminCourseID);
        hCur->addWidget(new QLabel("Kỳ:"));
        hCur->addWidget(spnAdminSemester);
        hCur->addWidget(chkAdminCompulsory);
        hCur->addWidget(btnAddCurriculumItem);

        adminLayout->addWidget(grpAddCur);

        // Danh sach sinh vien toan truong
        tableAllStudents = new QTableWidget(this);
        tableAllStudents->setColumnCount(6);
        tableAllStudents->setHorizontalHeaderLabels({"Mã SV", "Họ Tên", "Khoa Đào Tạo", "Lớp", "SĐT", "Email"});
        tableAllStudents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableAllStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
        adminLayout->addWidget(tableAllStudents);

        tabWidget->addTab(tabAdmin, "Quản Trị Hệ Thống");

        connect(btnAddStudent, &QPushButton::clicked, this, &MainWindow::onAddStudentClicked);
        connect(btnAddCurriculumItem, &QPushButton::clicked, this, &MainWindow::onAddCurriculumItemClicked);
    }

    // Connect signals
    connect(cboFacultyFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCurriculumFacultyChanged);
    connect(btnRegister, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(btnCancelRegistration, &QPushButton::clicked, this, &MainWindow::onCancelRegistrationClicked);
    connect(btnSaveData, &QPushButton::clicked, this, &MainWindow::onSaveDataClicked);
    connect(btnLogout, &QPushButton::clicked, this, &QMainWindow::close);
}

void MainWindow::loadUserInfo() {
    if (isAdmin) {
        lblUserInfo->setText("Tài khoản: Quản Trị Viên (Toàn quyền quản lý hệ thống)");
    } else {
        Student* sv = system.findStudent(currentStudentID);
        if (sv != nullptr) {
            QString info = QString("Sinh viên: %1 | Mã SV: %2 | Khoa: %3 | Lớp: %4")
                .arg(QString::fromStdString(sv->getHoTen()))
                .arg(QString::fromStdString(sv->getMaSV()))
                .arg(QString::fromStdString(sv->getTenKhoa()))
                .arg(QString::fromStdString(sv->getLop()));
            lblUserInfo->setText(info);

            // Chuyen bo loc CTDT ve khoa cua SV
            string mk = sv->getMaKhoa();
            for (int i = 0; i < cboFacultyFilter->count(); ++i) {
                if (cboFacultyFilter->itemData(i).toString().toStdString() == mk) {
                    cboFacultyFilter->setCurrentIndex(i);
                    break;
                }
            }
        }
    }
}

void MainWindow::loadCurriculumTable() {
    tableCurriculum->setRowCount(0);
    string selectedFaculty = cboFacultyFilter->currentData().toString().toStdString();

    auto current = system.getCurriculums().getHead();
    int row = 0;
    while (current != nullptr) {
        const CurriculumItem& item = current->data;
        if (selectedFaculty == "ALL" || item.getMaKhoa() == selectedFaculty || item.getMaKhoa() == "100") {
            tableCurriculum->insertRow(row);

            Course* c = system.findCourse(item.getMaMon());
            string tenMon = (c != nullptr) ? c->getTenMon() : "N/A";
            int soTC = (c != nullptr) ? c->getSoTinChi() : 0;

            tableCurriculum->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(item.getMaMon())));
            tableCurriculum->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(tenMon)));
            tableCurriculum->setItem(row, 2, new QTableWidgetItem(QString::number(soTC)));
            tableCurriculum->setItem(row, 3, new QTableWidgetItem("Kỳ " + QString::number(item.getHocKy())));
            tableCurriculum->setItem(row, 4, new QTableWidgetItem(item.isBatBuoc() ? "Bắt buộc" : "Tự chọn"));
            tableCurriculum->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(item.getTenKhoa())));
            row++;
        }
        current = current->next;
    }
}

void MainWindow::onCurriculumFacultyChanged(int) {
    loadCurriculumTable();
}

void MainWindow::loadAvailableSectionsTable() {
    tableAvailableSections->setRowCount(0);
    auto current = system.getSections().getHead();
    int row = 0;
    while (current != nullptr) {
        const ClassSection& sec = current->data;
        tableAvailableSections->insertRow(row);

        Course* c = system.findCourse(sec.getMaMon());
        string tenMon = (c != nullptr) ? c->getTenMon() : "N/A";
        int soTC = (c != nullptr) ? c->getSoTinChi() : 0;

        QString thoiGian = QString("%1 (Tiết %2-%3)")
            .arg(QString::fromStdString(sec.getThu()))
            .arg(sec.getTietBatDau())
            .arg(sec.getTietKetThuc());

        QString siSo = QString("%1 / %2")
            .arg(sec.getSoLuongDaDangKy())
            .arg(sec.getSiSoToiDa());

        tableAvailableSections->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(sec.getMaLopHP())));
        tableAvailableSections->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(sec.getMaMon())));
        tableAvailableSections->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(tenMon)));
        tableAvailableSections->setItem(row, 3, new QTableWidgetItem(QString::number(soTC)));
        tableAvailableSections->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(sec.getGiangVien())));
        tableAvailableSections->setItem(row, 5, new QTableWidgetItem(thoiGian));
        tableAvailableSections->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(sec.getPhong())));
        tableAvailableSections->setItem(row, 7, new QTableWidgetItem(siSo));

        row++;
        current = current->next;
    }
}

void MainWindow::loadRegisteredSectionsTable() {
    tableRegisteredSections->setRowCount(0);
    if (currentStudentID.empty()) return;

    auto current = system.getRegistrations().getHead();
    int row = 0;
    while (current != nullptr) {
        const Registration& reg = current->data;
        if (reg.getMaSV() == currentStudentID) {
            ClassSection* sec = system.findClassSection(reg.getMaLopHP());
            string maMon = sec ? sec->getMaMon() : "";
            Course* c = sec ? system.findCourse(sec->getMaMon()) : nullptr;
            string tenMon = c ? c->getTenMon() : "";

            QString thoiGian = sec ? QString("%1 (Tiết %2-%3)").arg(QString::fromStdString(sec->getThu())).arg(sec->getTietBatDau()).arg(sec->getTietKetThuc()) : "";
            QString phong = sec ? QString::fromStdString(sec->getPhong()) : "";

            tableRegisteredSections->insertRow(row);
            tableRegisteredSections->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reg.getMaDangKy())));
            tableRegisteredSections->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(reg.getMaLopHP())));
            tableRegisteredSections->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(maMon)));
            tableRegisteredSections->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(tenMon)));
            tableRegisteredSections->setItem(row, 4, new QTableWidgetItem(thoiGian));
            tableRegisteredSections->setItem(row, 5, new QTableWidgetItem(phong));
            tableRegisteredSections->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(reg.getNgayDangKy())));

            row++;
        }
        current = current->next;
    }
}

void MainWindow::onRegisterClicked() {
    if (isAdmin) {
        QMessageBox::information(this, "Thông báo", "Tài khoản quản trị viên không thể đăng ký học phần!");
        return;
    }

    int selectedRow = tableAvailableSections->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một lớp học phần trong bảng để đăng ký!");
        return;
    }

    string sectionID = tableAvailableSections->item(selectedRow, 0)->text().toStdString();
    string courseID = tableAvailableSections->item(selectedRow, 1)->text().toStdString();

    // KIEM TRA CHUONG TRINH DAO TAO KHOA
    if (!system.isCourseAllowedForStudent(currentStudentID, courseID)) {
        Student* sv = system.findStudent(currentStudentID);
        QString msg = QString("Môn học [%1] không nằm trong chương trình đào tạo của %2!\nSinh viên không được phép đăng ký môn này.")
            .arg(QString::fromStdString(courseID))
            .arg(sv ? QString::fromStdString(sv->getTenKhoa()) : "khoa của bạn");
        QMessageBox::critical(this, "Lỗi đăng ký - Sai chương trình đào tạo", msg);
        return;
    }

    // Goi ham dang ky
    if (system.registerClass(currentStudentID, sectionID)) {
        QMessageBox::information(this, "Thành công", "Đăng ký học phần thành công!");
        loadAvailableSectionsTable();
        loadRegisteredSectionsTable();
    } else {
        if (system.isTimeConflict(currentStudentID, sectionID)) {
            QMessageBox::warning(this, "Lỗi trùng lịch", "Lớp học phần này bị trùng lịch với lớp bạn đã đăng ký!");
        } else {
            QMessageBox::warning(this, "Không thể đăng ký", "Lớp học phần đã đầy (đã được đưa vào hàng đợi) hoặc bạn đã đăng ký lớp này rồi!");
            loadAvailableSectionsTable();
        }
    }
}

void MainWindow::onCancelRegistrationClicked() {
    int selectedRow = tableRegisteredSections->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn lớp học phần cần hủy!");
        return;
    }

    string sectionID = tableRegisteredSections->item(selectedRow, 1)->text().toStdString();
    if (system.cancelRegistration(currentStudentID, sectionID)) {
        QMessageBox::information(this, "Thành công", "Hủy đăng ký học phần thành công!");
        loadAvailableSectionsTable();
        loadRegisteredSectionsTable();
    } else {
        QMessageBox::warning(this, "Thất bại", "Không thể hủy đăng ký lớp này!");
    }
}

void MainWindow::onSaveDataClicked() {
    string error;
    if (fileManager.saveAllData(system, error)) {
        QMessageBox::information(this, "Lưu dữ liệu", "Đã lưu toàn bộ dữ liệu vào tệp tin thành công!");
    } else {
        QMessageBox::critical(this, "Lỗi lưu tệp", QString::fromStdString(error));
    }
}

void MainWindow::loadAdminStudentTable() {
    if (!tableAllStudents) return;
    tableAllStudents->setRowCount(0);

    auto current = system.getStudents().getHead();
    int row = 0;
    while (current != nullptr) {
        const Student& sv = current->data;
        tableAllStudents->insertRow(row);
        tableAllStudents->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(sv.getMaSV())));
        tableAllStudents->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(sv.getHoTen())));
        tableAllStudents->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(sv.getTenKhoa())));
        tableAllStudents->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(sv.getLop())));
        tableAllStudents->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(sv.getSoDienThoai())));
        tableAllStudents->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(sv.getEmail())));

        row++;
        current = current->next;
    }
}

void MainWindow::onAddStudentClicked() {
    string id = txtAddStudentID->text().trimmed().toStdString();
    string name = txtAddStudentName->text().trimmed().toStdString();
    string phone = txtAddStudentPhone->text().trimmed().toStdString();
    string email = txtAddStudentEmail->text().trimmed().toStdString();
    string lop = txtAddStudentClass->text().trimmed().toStdString();
    string major = txtAddStudentMajor->text().trimmed().toStdString();

    if (!Student::isValidMaSV(id)) {
        QMessageBox::critical(this, "Lỗi nhập liệu", "Mã sinh viên không hợp lệ!\nQuy ước: Phải gồm 8 chữ số và 3 số đầu từ 100 đến 103.");
        return;
    }

    if (name.empty()) {
        QMessageBox::warning(this, "Lỗi nhập liệu", "Vui lòng nhập họ tên sinh viên!");
        return;
    }

    Student newSV(id, name, phone, email, lop, major);
    if (system.addStudent(newSV)) {
        QMessageBox::information(this, "Thành công", QString("Đã thêm sinh viên thuộc %1 thành công!").arg(QString::fromStdString(newSV.getTenKhoa())));
        loadAdminStudentTable();
        txtAddStudentID->clear();
        txtAddStudentName->clear();
        txtAddStudentPhone->clear();
        txtAddStudentEmail->clear();
        txtAddStudentClass->clear();
        txtAddStudentMajor->clear();
    } else {
        QMessageBox::warning(this, "Thất bại", "Mã sinh viên này đã tồn tại trong hệ thống!");
    }
}

void MainWindow::onAddCurriculumItemClicked() {
    string faculty = cboAdminFaculty->currentData().toString().toStdString();
    string courseID = txtAdminCourseID->text().trimmed().toStdString();
    int semester = spnAdminSemester->value();
    bool compulsory = chkAdminCompulsory->isChecked();

    if (courseID.empty() || system.findCourse(courseID) == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Mã môn học không tồn tại trong danh mục môn học!");
        return;
    }

    CurriculumItem item(faculty, courseID, semester, compulsory);
    if (system.addCurriculumItem(item)) {
        QMessageBox::information(this, "Thành công", "Đã thêm môn học vào chương trình đào tạo của khoa!");
        loadCurriculumTable();
        txtAdminCourseID->clear();
    } else {
        QMessageBox::warning(this, "Thất bại", "Môn học này đã có trong CTĐT của khoa này rồi!");
    }
}

