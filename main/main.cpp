#include <iostream>
#include <limits>
#include <string>

#include "../manager/authmanager.h"
#include "../manager/filemanager.h"

using namespace std;

namespace {

void showAdminMenu() {
    cout << "\n=== MENU QUAN TRI ===\n"
              << "1. Them sinh vien\n"
              << "2. Them mon hoc\n"
              << "3. Them lop hoc phan\n"
              << "4. Dang ky lop hoc phan cho sinh vien\n"
              << "5. Huy dang ky\n"
              << "6. Xem sinh vien\n"
              << "7. Xem mon hoc\n"
              << "8. Xem lop hoc phan\n"
              << "9. Xem tat ca dang ky\n"
              << "10. Dang xuat\n"
              << "0. Luu va thoat\n"
              << "Chon: ";
}

void showStudentMenu() {
    cout << "\n=== MENU SINH VIEN ===\n"
              << "1. Xem mon hoc\n"
              << "2. Xem lop hoc phan\n"
              << "3. Dang ky lop hoc phan\n"
              << "4. Huy dang ky cua toi\n"
              << "5. Xem dang ky cua toi\n"
              << "6. Xem thong tin ca nhan\n"
              << "7. Dang xuat\n"
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

void showStudentRegistrations(const SystemManager& system,
                              const string& studentID) {
    auto current = system.getRegistrations().getHead();
    bool found = false;
    while (current != nullptr) {
        if (current->data.getMaSV() == studentID) {
            current->data.xuat();
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Ban chua co dang ky nao.\n";
    }
}

bool readRegistrationIDs(string& studentID, string& sectionID,
                         bool includeStudentID) {
    if (includeStudentID) {
        cout << "Ma sinh vien: ";
        if (!getline(cin, studentID)) {
            return false;
        }
    }
    cout << "Ma lop hoc phan: ";
    return static_cast<bool>(getline(cin, sectionID));
}

bool saveAndExit(const FileManager& files, const SystemManager& system,
                 string& error) {
    if (!files.saveAllData(system, error)) {
        cerr << "Khong the luu du lieu: " << error << '\n';
        return false;
    }
    cout << "Da luu du lieu.\n";
    return true;
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

    AuthManager auth;
    if (!auth.addAccount("admin", "admin", UserRole::Administrator)) {
        cerr << "Khong the tao tai khoan quan tri mac dinh.\n";
        return 1;
    }
    auto student = system.getStudents().getHead();
    while (student != nullptr) {
        const string& studentID = student->data.getMaSV();
        if (!auth.addAccount(studentID, studentID, UserRole::Student, studentID)) {
            cerr << "Khong the tao tai khoan cho sinh vien: " << studentID
                 << ". Ma sinh vien co the trung ten tai khoan.\n";
            return 1;
        }
        student = student->next;
    }

    while (true) {
        if (!auth.isLoggedIn()) {
            string username, password;
            cout << "\n=== DANG NHAP ===\nTai khoan: ";
            if (!getline(cin >> ws, username)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            cout << "Mat khau: ";
            if (!getline(cin, password)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            if (!auth.login(username, password)) {
                cout << "Tai khoan hoac mat khau khong dung.\n";
                continue;
            }
            cout << "Dang nhap thanh cong.\n";
        }

        const UserAccount* user = auth.currentUser();
        if (user == nullptr) {
            cerr << "Phien dang nhap khong hop le.\n";
            return 1;
        }
        const bool isAdministrator = user->role == UserRole::Administrator;
        if (isAdministrator) {
            showAdminMenu();
        } else {
            showStudentMenu();
        }

        int choice;
        if (!readChoice(choice)) {
            if (!cin.eof()) {
                cout << "Lua chon khong hop le.\n";
                continue;
            }
            return saveAndExit(files, system, error) ? 0 : 1;
        }

        if (choice == 0) {
            return saveAndExit(files, system, error) ? 0 : 1;
        }

        if ((isAdministrator && choice == 10) ||
            (!isAdministrator && choice == 7)) {
            auth.logout();
            continue;
        }

        bool success = false;
        if (isAdministrator && choice == 1) {
            Student newStudent;
            newStudent.nhap();
            const string studentID = newStudent.getMaSV();
            if (auth.hasAccount(studentID)) {
                cout << "Ma sinh vien trung voi ten tai khoan da ton tai.\n";
                continue;
            }
            success = system.addStudent(newStudent);
            if (success &&
                !auth.addAccount(studentID, studentID, UserRole::Student,
                                 studentID)) {
                system.deleteStudent(studentID);
                success = false;
            }
            if (success) {
                cout << "Tai khoan sinh vien: " << studentID
                     << " | Mat khau khoi tao: " << studentID << '\n';
            }
        } else if (isAdministrator && choice == 2) {
            Course course;
            course.nhap();
            success = system.addCourse(course);
        } else if (isAdministrator && choice == 3) {
            ClassSection section;
            section.nhap();
            success = system.addClassSection(section);
        } else if (isAdministrator && choice == 4) {
            string studentID, sectionID;
            if (!readRegistrationIDs(studentID, sectionID, true)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            success = system.registerClass(studentID, sectionID);
        } else if (isAdministrator && choice == 5) {
            string studentID, sectionID;
            if (!readRegistrationIDs(studentID, sectionID, true)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            success = system.cancelRegistration(studentID, sectionID);
        } else if (isAdministrator && choice == 6) {
            system.displayAllStudents();
            continue;
        } else if (isAdministrator && choice == 7) {
            system.displayAllCourses();
            continue;
        } else if (isAdministrator && choice == 8) {
            system.displayAllSections();
            continue;
        } else if (isAdministrator && choice == 9) {
            system.displayAllRegistrations();
            continue;
        } else if (!isAdministrator && choice == 1) {
            system.displayAllCourses();
            continue;
        } else if (!isAdministrator && choice == 2) {
            system.displayAllSections();
            continue;
        } else if (!isAdministrator && choice == 3) {
            string studentID, sectionID;
            if (!readRegistrationIDs(studentID, sectionID, false)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            success = system.registerClass(user->studentID, sectionID);
        } else if (!isAdministrator && choice == 4) {
            string studentID, sectionID;
            if (!readRegistrationIDs(studentID, sectionID, false)) {
                return saveAndExit(files, system, error) ? 0 : 1;
            }
            success = system.cancelRegistration(user->studentID, sectionID);
        } else if (!isAdministrator && choice == 5) {
            showStudentRegistrations(system, user->studentID);
            continue;
        } else if (!isAdministrator && choice == 6) {
            const Student* currentStudent = system.findStudent(user->studentID);
            if (currentStudent == nullptr) {
                cerr << "Khong tim thay thong tin sinh vien dang dang nhap.\n";
                return 1;
            }
            currentStudent->xuat();
            continue;
        } else {
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        cout << (success ? "Thao tac thanh cong.\n"
                              : "Thao tac that bai; hay kiem tra du lieu va ma dinh danh.\n");
    }
}
