#include <iostream>
#include <limits>
#include <string>

#include "../manager/filemanager.h"

using namespace std;

namespace {

void showMenu() {
    cout << "\n=== HE THONG DANG KY TIN CHI ===\n"
              << "1. Them sinh vien\n"
              << "2. Them mon hoc\n"
              << "3. Them lop hoc phan\n"
              << "4. Dang ky lop hoc phan\n"
              << "5. Huy dang ky\n"
              << "6. Xem sinh vien\n"
              << "7. Xem mon hoc\n"
              << "8. Xem lop hoc phan\n"
              << "9. Xem dang ky\n"
              << "0. Luu va thoat\n"
              << "Chon: ";
}

bool readChoice(int& choice) {
    if (cin >> choice) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
    if (cin.eof()) {
        return false;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

}  // namespace

int main() {
    SystemManager system;
    FileManager files;
    string error;

    if (!files.loadAllData(system, error)) {
        cerr << "Khong the nap du lieu: " << error << '\n';
        return 1;
    }

    while (true) {
        showMenu();
        int choice;
        if (!readChoice(choice)) {
            if (!cin.eof()) {
                cout << "Lua chon khong hop le.\n";
                continue;
            }
            choice = 0;
        }

        if (choice == 0) {
            if (!files.saveAllData(system, error)) {
                cerr << "Khong the luu du lieu: " << error << '\n';
                return 1;
            }
            cout << "Da luu du lieu.\n";
            return 0;
        }

        bool success = false;
        if (choice == 1) {
            Student student;
            student.nhap();
            success = system.addStudent(student);
        } else if (choice == 2) {
            Course course;
            course.nhap();
            success = system.addCourse(course);
        } else if (choice == 3) {
            ClassSection section;
            section.nhap();
            success = system.addClassSection(section);
        } else if (choice == 4) {
            string studentID, sectionID;
            cout << "Ma sinh vien: ";
            getline(cin, studentID);
            cout << "Ma lop hoc phan: ";
            getline(cin, sectionID);
            success = system.registerClass(studentID, sectionID);
        } else if (choice == 5) {
            string studentID, sectionID;
            cout << "Ma sinh vien: ";
            getline(cin, studentID);
            cout << "Ma lop hoc phan: ";
            getline(cin, sectionID);
            success = system.cancelRegistration(studentID, sectionID);
        } else if (choice == 6) {
            system.displayAllStudents();
            continue;
        } else if (choice == 7) {
            system.displayAllCourses();
            continue;
        } else if (choice == 8) {
            system.displayAllSections();
            continue;
        } else if (choice == 9) {
            system.displayAllRegistrations();
            continue;
        } else {
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        cout << (success ? "Thao tac thanh cong.\n"
                              : "Thao tac that bai; hay kiem tra du lieu va ma dinh danh.\n");
    }
}
