#include <QApplication>
#include <QStyleFactory>
#include <string>

#include "manager/system.h"
#include "manager/filemanager.h"
#include "gui/logindialog.h"
#include "gui/mainwindow.h"

void seedSampleDataIfEmpty(SystemManager& system) {
    if (system.getCourses().getHead() != nullptr) {
        return; // Da co du lieu
    }

    // 1. Khoi tao cac mon hoc mau
    system.addCourse(Course("MATH01", "Giai tich 1", 3));
    system.addCourse(Course("CS101", "Lap trinh C++", 3));
    system.addCourse(Course("CS102", "Cau truc du lieu va giai thuat", 3));
    system.addCourse(Course("ME101", "Hinh hoa ve ky thuat", 2));
    system.addCourse(Course("EE101", "Ly thuyet mach dien", 3));

    // 2. Gan mon vao Chuong trinh dao tao (CTDT) theo Khoa
    // Khoa 100: Khoa Dai cuong (mon dung chung)
    system.addCurriculumItem(CurriculumItem("100", "MATH01", 1, true));
    // Khoa 102: Khoa CNTT
    system.addCurriculumItem(CurriculumItem("102", "CS101", 2, true));
    system.addCurriculumItem(CurriculumItem("102", "CS102", 3, true));
    // Khoa 101: Khoa Co khi
    system.addCurriculumItem(CurriculumItem("101", "ME101", 2, true));
    // Khoa 103: Khoa Dien - Dien tu
    system.addCurriculumItem(CurriculumItem("103", "EE101", 2, true));

    // 3. Khoi tao cac sinh vien mau (8 chu so, 3 so dau 100-103)
    system.addStudent(Student("10222001", "Nguyen Van An", "0901234567", "an@dut.udn.vn", "22T_DT1", "Cong nghe Thong tin"));
    system.addStudent(Student("10122001", "Tran Quoc Bao", "0907654321", "bao@dut.udn.vn", "22CK1", "Co khi Che tao"));
    system.addStudent(Student("10322001", "Le Thi Cam", "0912345678", "cam@dut.udn.vn", "22DT1", "Dien tu Vien thong"));

    // 4. Mo cac lop hoc phan
    system.addClassSection(ClassSection("LHP_MATH01", "MATH01", "TS. Nguyen Van Toan", "Thu 2", 1, 3, "A101", 60));
    system.addClassSection(ClassSection("LHP_CS101", "CS101", "ThS. Tran Minh", "Thu 3", 4, 6, "F201", 45));
    system.addClassSection(ClassSection("LHP_CS102", "CS102", "TS. Le Hoang", "Thu 4", 1, 3, "F202", 40));
    system.addClassSection(ClassSection("LHP_ME101", "ME101", "ThS. Pham Duc", "Thu 5", 7, 9, "C301", 50));
    system.addClassSection(ClassSection("LHP_EE101", "EE101", "TS. Vu Nam", "Thu 6", 1, 3, "E101", 45));
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    SystemManager system;
    FileManager fileManager;
    std::string error;

    // Nap du lieu tu cac file .txt
    fileManager.loadAllData(system, error);

    // Tu dong them du lieu mau neu chua co tap tin
    seedSampleDataIfEmpty(system);

    LoginDialog loginDialog(system);
    if (loginDialog.exec() == QDialog::Accepted) {
        MainWindow* mainWindow = new MainWindow(system, fileManager,
                                                loginDialog.getLoggedInStudentID(),
                                                loginDialog.isAdministrator());
        mainWindow->show();
        return app.exec();
    }

    return 0;
}