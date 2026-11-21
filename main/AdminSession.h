#ifndef ADMIN_SESSION_H
#define ADMIN_SESSION_H

#include <iostream>
#include <string>
#include <stdexcept>

#include "../manager/authmanager.h"
#include "../manager/system.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/student.h"
#include "../ui/ConsoleView.h"
#include "SessionHelper.h"

class AdminSession {
public:
    static void run(SystemManager& system, AuthManager& auth) {
        using namespace std;
        while (true) {
            ConsoleView::showAdminMenu(system.getCurrentSemester());
            int choice;
            if (!ConsoleView::readChoice(choice)) {
                if (cin.eof()) break;
                cout << "Lua chon khong hop le.\n";
                continue;
            }

            if (choice == 0) {
                cout << "\n>>> Da dang xuat khoi quyen quan tri vien.\n";
                string err;
                system.saveData(err);
                break;
            }

            if (choice == 1) {
                Student sv;
                cin >> sv;
                const string studentID = sv.getMaSV();
                if (auth.hasAccount(studentID)) {
                    cout << ">> Them that bai: Ma sinh vien da ton tai.\n";
                } else if (system.addStudent(sv)) {
                    if (!auth.addAccount(studentID, studentID, UserRole::Student,
                                         studentID)) {
                        system.deleteStudent(studentID);
                        cout << ">> Them that bai: Khong the tao tai khoan sinh vien.\n";
                        continue;
                    }
                    cout << ">> Them sinh vien moi thanh cong!\n";
                    cout << ">> Tai khoan va mat khau khoi tao: " << studentID << '\n';
                } else {
                    cout << ">> Them that bai: Ma sinh vien da ton tai hoac du lieu khong hop le.\n";
                }
            } else if (choice == 2) {
                cout << "Nhap ma sinh vien can xoa: ";
                string svID;
                getline(cin, svID);
                DoubleLinkedList<string> waitlistNotices;
                if (system.deleteStudent(svID, &waitlistNotices)) {
                    ConsoleView::printNotices(waitlistNotices);
                    auth.removeAccount(svID);
                    cout << ">> Da xoa sinh vien " << svID << " va tat ca cac dang ky lien quan.\n";
                } else {
                    cout << ">> Khong tim thay sinh vien voi ma: " << svID << "\n";
                }
            } else if (choice == 3) {
                cout << "Nhap ma sinh vien can sua: ";
                string svID;
                getline(cin, svID);
                Student* sv = system.findStudent(svID);
                if (sv == nullptr) {
                    cout << ">> Khong tim thay sinh vien voi ma: " << svID << "\n";
                } else {
                    ConsoleView::editStudentInteractive(*sv);
                    cout << ">> Cap nhat thong tin sinh vien thanh cong!\n";
                }
            } else if (choice == 4) {
                cout << "Nhap tu khoa tim kiem sinh vien (ma SV, ho ten, lop, khoa): ";
                string kw;
                getline(cin, kw);
                DoubleLinkedList<Student> results;
                system.searchStudents(kw, results);
                ConsoleView::displayStudentSearchResults(kw, results);
            } else if (choice == 5) {
                DoubleLinkedList<Student> students;
                system.getAllStudents(students);
                ConsoleView::displayStudents(students);
            } else if (choice == 6) {
                Course c;
                cin >> c;
                if (system.addCourse(c)) {
                    cout << ">> Them mon hoc moi thanh cong!\n";
                } else {
                    cout << ">> Them that bai: Ma mon da ton tai hoac so tin chi khong hop le.\n";
                }
            } else if (choice == 7) {
                cout << "Nhap ma mon hoc can xoa: ";
                string courseID;
                getline(cin, courseID);
                string errMsg;
                if (system.deleteCourse(courseID, &errMsg)) {
                    cout << ">> Xoa mon hoc thanh cong!\n";
                } else {
                    if (!errMsg.empty()) cout << errMsg << '\n';
                    cout << ">> Xoa mon hoc that bai.\n";
                }
            } else if (choice == 8) {
                cout << "Nhap ma mon hoc can sua: ";
                string courseID;
                getline(cin, courseID);
                Course* c = system.findCourse(courseID);
                if (c == nullptr) {
                    cout << ">> Khong tim thay mon hoc voi ma: " << courseID << "\n";
                } else {
                    ConsoleView::editCourseInteractive(*c);
                    string err;
                    system.saveData(err);
                    cout << ">> Cap nhat mon hoc thanh cong!\n";
                }
            } else if (choice == 9) {
                cout << "Nhap tu khoa tim kiem mon hoc (ma mon, ten mon, khoa, nganh): ";
                string kw;
                getline(cin, kw);
                DoubleLinkedList<Course> results;
                system.searchCourses(kw, results);
                ConsoleView::displayCourseSearchResults(kw, results);
            } else if (choice == 10) {
                DoubleLinkedList<Course> courses;
                system.getAllCourses(courses);
                ConsoleView::displayCourses(courses);
            } else if (choice == 11) {
                SessionHelper::searchCoursesSpecialized(system);
            } else if (choice == 12) {
                ClassSection sec;
                cin >> sec;
                if (system.addClassSection(sec)) {
                    cout << ">> Them lop hoc phan thanh cong!\n";
                } else {
                    cout << ">> Them that bai: Ma lop HP da ton tai hoac ma mon khong ton tai trong he thong.\n";
                }
            } else if (choice == 13) {
                cout << "Nhap ma lop hoc phan can xoa: ";
                string secID;
                getline(cin, secID);
                if (system.deleteClassSection(secID)) {
                    cout << ">> Xoa lop hoc phan thanh cong (da huy cac dang ky lien quan).\n";
                } else {
                    cout << ">> Xoa lop hoc phan that bai.\n";
                }
            } else if (choice == 14) {
                cout << "Nhap ma lop hoc phan can sua: ";
                string secID;
                getline(cin, secID);
                ClassSection* sec = system.findClassSection(secID);
                if (sec == nullptr) {
                    cout << ">> Khong tim thay lop hoc phan voi ma: " << secID << "\n";
                } else {
                    ConsoleView::editClassSectionInteractive(*sec);
                    cout << ">> Cap nhat thong tin lop hoc phan thanh cong!\n";
                }
            } else if (choice == 15) {
                cout << "Nhap tu khoa tim kiem (Ma lop HP, ma mon, nhom, giang vien): ";
                string kw;
                getline(cin, kw);
                DoubleLinkedList<ClassSection> results;
                system.searchSections(kw, results);
                ConsoleView::displaySectionSearchResults(kw, results);
            } else if (choice == 16) {
                cout << "1. Xem theo tung NHOM lop hoc phan (theo hoc ky " << system.getCurrentSemester() << ")\n";
                cout << "2. Xem tat ca cac lop hoc phan\n";
                cout << "Chon (1-2): ";
                int subChoice = 1;
                ConsoleView::readChoice(subChoice);
                if (subChoice == 1) {
                    SessionHelper::showSectionsByGroupView(system);
                } else {
                    DoubleLinkedList<ClassSection> sections;
                    system.getAllSections(sections);
                    ConsoleView::displaySections(sections);
                }
            } else if (choice == 17) {
                cout << "Nhap ma lop hoc phan can xem danh sach sinh vien: ";
                string secID;
                getline(cin, secID);
                ClassSection sec;
                DoubleLinkedList<SectionStudentInfo> students;
                if (!system.getStudentsInSection(secID, sec, students)) {
                    cout << ">> Khong tim thay lop hoc phan: " << secID << "\n";
                } else {
                    ConsoleView::displayStudentsInSection(sec, students);

                    // Kiem tra dieu kien: Lop du so luong VA co sinh vien trong danh sach cho
                    if (system.canShowWaitlist(secID)) {
                        int count = system.getWaitlistCount(secID);
                        cout << "\n------------------------------------------------------------\n";
                        cout << "  [THONG BAO]: Lop hoc phan da DU SO LUONG (" 
                             << sec.getSoLuongDaDangKy() << "/" << sec.getSiSoToiDa()
                             << ") va dang co " << count << " sinh vien trong danh sach cho (Waitlist).\n";
                        cout << "------------------------------------------------------------\n";
                        cout << "  Chuc nang mo rong:\n";
                        cout << "  1. Show danh sach cho (Waitlist)\n";
                        cout << "  0. Bo qua (Quay lai menu)\n";
                        cout << "  Nhap lua chon: ";
                        int subChoice = 0;
                        if (ConsoleView::readChoice(subChoice) && subChoice == 1) {
                            DoubleLinkedList<Student> waitlistStudents;
                            system.getWaitlistStudents(secID, waitlistStudents);
                            ConsoleView::displayWaitlistStudents(secID, waitlistStudents);
                        }
                    }
                }
            } else if (choice == 18) {
                string svID, secID;
                cout << "Ma sinh vien: ";
                getline(cin, svID);
                cout << "Ma lop hoc phan: ";
                getline(cin, secID);
                SessionHelper::tryRegisterWithFeedback(system, svID, secID);
            } else if (choice == 19) {
                string svID, secID;
                cout << "Ma sinh vien: ";
                getline(cin, svID);
                cout << "Ma lop hoc phan: ";
                getline(cin, secID);
                DoubleLinkedList<string> waitlistNotices;
                bool success = system.cancelRegistration(svID, secID, &waitlistNotices);
                ConsoleView::printNotices(waitlistNotices);
                cout << (success ? ">> Huy dang ky lop hoc phan thanh cong!\n" : ">> Huy dang ky that bai!\n");
            } else if (choice == 20) {
                DoubleLinkedList<Registration> registrations;
                system.getAllRegistrations(registrations);
                ConsoleView::displayRegistrations(registrations);
            } else if (choice == 21) {
                cout << "\n--- THIET LAP KI HOC HE THONG ---\n";
                cout << "Ki hoc hien tai: Ki " << system.getCurrentSemester() << '\n';
                cout << "Nhap ki hoc muon ap dung: ";
                string semStr;
                getline(cin, semStr);
                int newSem = 0;
                try {
                    newSem = stoi(semStr);
                } catch (const invalid_argument&) {
                    cout << ">> Gia tri hoc ky nhap vao khong hop le.\n";
                } catch (const out_of_range&) {
                    cout << ">> Gia tri hoc ky nhap vao vuot gioi han.\n";
                }
                if (system.setCurrentSemester(newSem)) {
                    cout << ">> Da thiet lap thanh cong: He thong hien dang o Hoc ky " << newSem << ".\n";
                } else {
                    cout << ">> Thiet lap that bai: Ki hoc chi co the la 1 hoac 2.\n";
                }
            } else {
                cout << "Lua chon khong hop le.\n";
            }
        }
    }
};

#endif // ADMIN_SESSION_H
