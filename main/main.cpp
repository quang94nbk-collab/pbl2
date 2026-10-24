#include <iostream>
#include <limits>
#include <string>

#include "../manager/filemanager.h"

using namespace std;

namespace {

void showMenu(int currentSemester) {
    cout << "\n=== HE THONG DANG KY TIN CHI [HOC KY HIEN TAI: " << currentSemester << "] ===\n"
         << "1. Them sinh vien\n"
         << "2. Them mon hoc (kem ki hoc 1 hoac 2)\n"
         << "3. Them lop hoc phan (kem ki hoc 1 hoac 2)\n"
         << "4. Dang ky lop hoc phan\n"
         << "5. Huy dang ky\n"
         << "6. Xem sinh vien\n"
         << "7. Xem mon hoc (tat ca)\n"
         << "8. Xem lop hoc phan\n"
         << "9. Xem dang ky\n"
         << "10. Xem mon hoc theo Khoa, Khoa hoc, Nganh & Ki hoc (Bat buoc / Tu chon)\n"
         << "11. Thiet lap ki hoc he thong (Admin quyet dinh ki 1 hoac 2)\n"
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
        showMenu(system.getCurrentSemester());
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
        } else if (choice == 10) {
            string khoa, khoaHoc, nganh;
            cout << "Nhap Khoa can xem (de trong de xem tat ca): ";
            getline(cin, khoa);
            cout << "Nhap Khoa hoc can xem (vi du: K22, de trong de xem tat ca): ";
            getline(cin, khoaHoc);
            cout << "Nhap Nganh hoc can xem (vi du: Dac thu, Tieng Nhat, de trong de xem tat ca): ";
            getline(cin, nganh);
            cout << "Nhap Ki hoc can xem (1 hoac 2, de trong hoac nhap 0 de xem tat ca): ";
            string hkStr;
            getline(cin, hkStr);
            int hk = 0;
            if (!hkStr.empty()) {
                try {
                    hk = stoi(hkStr);
                } catch (...) {
                    hk = 0;
                }
            }
            system.displayCoursesByKhoaKhoaHocAndNganh(khoa, khoaHoc, nganh, hk);
            continue;
        } else if (choice == 11) {
            cout << "\n--- THIET LAP KI HOC HE THONG (ADMIN QUYET DINH) ---\n";
            cout << "Ki hoc hien tai: Ki " << system.getCurrentSemester() << '\n';
            cout << "Nhap ki hoc muon ap dung (1 hoac 2): ";
            string semStr;
            getline(cin, semStr);
            int newSem = 0;
            try {
                newSem = stoi(semStr);
            } catch (...) {
                newSem = 0;
            }
            if (system.setCurrentSemester(newSem)) {
                cout << "Da thiet lap thanh cong: He thong hien dang o Hoc ky " << newSem << ".\n";
            } else {
                cout << "Thiet lap that bai: Ki hoc chi co the la 1 hoac 2.\n";
            }
            continue;
        } else {
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        cout << (success ? "Thao tac thanh cong.\n"
                              : "Thao tac that bai; hay kiem tra du lieu va ma dinh danh.\n");
    }
}
