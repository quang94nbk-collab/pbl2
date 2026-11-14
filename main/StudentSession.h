#ifndef STUDENT_SESSION_H
#define STUDENT_SESSION_H

#include <iomanip>
#include <iostream>
#include <string>

#include "../manager/system.h"
#include "../model/course.h"
#include "../model/student.h"
#include "../ui/ConsoleView.h"
#include "SessionHelper.h"

class StudentSession {
public:
    static void run(SystemManager& system, const std::string& studentID) {
        using namespace std;
        const Student* student = system.findStudent(studentID);
        if (student == nullptr) {
            cout << "Loi: Khong tim thay thong tin sinh vien!\n";
            return;
        }

        while (true) {
            ConsoleView::showStudentMenu(system.getCurrentSemester(), *student);
            int choice;
            if (!ConsoleView::readChoice(choice)) {
                if (cin.eof()) break;
                cout << "Lua chon khong hop le.\n";
                continue;
            }

            if (choice == 0) {
                string missingIDs[50];
                int missingCount = system.getMissingCoursesCount(studentID, missingIDs, 50);
                if (missingCount > 0) {
                    cout << "\n======================================================================\n";
                    cout << "  [CANH BAO TIEN DO HOC TAP - HOC KY " << system.getCurrentSemester() << "]\n";
                    cout << "  Ban dang dang ky THIEU " << missingCount << " mon trong hoc ky nay:\n";
                    for (int m = 0; m < missingCount; m++) {
                        const Course* c = system.findCourse(missingIDs[m]);
                        cout << "    - [" << missingIDs[m] << "] "
                             << (c ? c->getTenMon() : missingIDs[m])
                             << " (" << (c ? c->getSoTinChi() : 0) << " TC)\n";
                    }
                    cout << "----------------------------------------------------------------------\n";
                    cout << "  CANH BAO: Sinh vien dang ky thieu mon se bi CHAM TIEN DO HOC TAP!\n";
                    cout << "======================================================================\n";
                    cout << "Ban co chac chan muon dang xuat?\n";
                    cout << "1. Co, toi chac chan muon dang xuat (chap nhan cham tien do)\n";
                    cout << "0. Khong, toi muon o lai de tiep tuc dang ky cho du mon\n";
                    cout << "Nhap lua chon (1 hoac 0): ";
                    int exitChoice = 0;
                    if (!ConsoleView::readChoice(exitChoice) || exitChoice != 1) {
                        cout << ">> Quay lai menu de tiep tuc dang ky mon con thieu.\n";
                        continue;
                    }
                }

                cout << "\n>>> Da dang xuat khoi tai khoan sinh vien.\n";
                string err;
                system.saveData(err);
                break;
            } else if (choice == 1) {
                cout << *student;
            } else if (choice == 2) {
                cout << "\n--- XEM DANH SACH MON HOC ---\n";
                cout << "1. Xem mon hoc theo chuong trinh cua toi (Khoa: "
                     << (student->getKhoa().empty() ? "Tat ca" : student->getKhoa())
                     << ", Khoa hoc: " << (student->getKhoaHoc().empty() ? "Tat ca" : student->getKhoaHoc())
                     << ", Nganh: " << (student->getNganhHoc().empty() ? "Tat ca" : student->getNganhHoc())
                     << ", Hoc ky " << system.getCurrentSemester() << ")\n";
                cout << "2. Xem tat ca mon hoc mo trong Hoc ky " << system.getCurrentSemester() << "\n";
                cout << "3. Xem tat ca mon hoc trong chuong trinh dao tao\n";
                cout << "Chon (1-3): ";
                int subChoice = 1;
                ConsoleView::readChoice(subChoice);
                DoubleLinkedList<Course> courses;
                if (subChoice == 1) {
                    system.filterCourses(student->getKhoa(), student->getKhoaHoc(),
                                         student->getNganhHoc(), system.getCurrentSemester(), courses);
                    ConsoleView::displayFilteredCourses(student->getKhoa(), student->getKhoaHoc(),
                                                        student->getNganhHoc(), system.getCurrentSemester(), courses);
                } else if (subChoice == 2) {
                    system.filterCourses("", "", "", system.getCurrentSemester(), courses);
                    ConsoleView::displayFilteredCourses("", "", "", system.getCurrentSemester(), courses);
                } else {
                    system.getAllCourses(courses);
                    ConsoleView::displayCourses(courses);
                }
            } else if (choice == 3) {
                cout << "Nhap tu khoa tim kiem mon hoc (ma mon, ten mon): ";
                string kw;
                getline(cin, kw);
                DoubleLinkedList<Course> results;
                system.searchCourses(kw, results);
                ConsoleView::displayCourseSearchResults(kw, results);
            } else if (choice == 4) {
                SessionHelper::showSectionsByGroupView(system);
            } else if (choice == 5) {
                const int MAX_GROUPS = 50;
                GroupInfo eligibleGroups[MAX_GROUPS];
                int groupCount = system.getEligibleGroupsForStudent(studentID, eligibleGroups, MAX_GROUPS);

                if (groupCount == 0) {
                    cout << "\n>> Hien khong co nhom lop hoc phan nao phu hop voi chuong trinh cua ban.\n";
                    continue;
                }

                cout << "  BUOC 1: CHON NHOM LOP HOC PHAN\n";
                for (int i = 0; i < groupCount; i++) {
                    cout << "[" << (i + 1) << "] Nhom: " << eligibleGroups[i].groupName
                         << " (" << eligibleGroups[i].totalSections << " lop HP | "
                         << eligibleGroups[i].totalCredits << " TC)\n";
                }
                cout << "[0] Quay lai\n";
                cout << "Chon so thu tu nhom lop hoc phan (1 - " << groupCount << ", hoac 0 de quay lai): ";
                int gIndex = 0;
                if (!ConsoleView::readChoice(gIndex) || gIndex <= 0 || gIndex > groupCount) {
                    if (gIndex != 0) cout << "Lua chon khong hop le.\n";
                    continue;
                }

                GroupInfo chosenGroup = eligibleGroups[gIndex - 1];

                cout << "\n  BUOC 2: CHON LOP HOC PHAN TRONG NHOM '" << chosenGroup.groupName << "'\n";
                for (int j = 0; j < chosenGroup.totalSections; j++) {
                    const ClassSection* sec = system.findClassSection(chosenGroup.sectionIDs[j]);
                    if (sec != nullptr) {
                        const Course* c = system.findCourse(sec->getMaMon());
                        bool daDK = system.isSectionRegistered(studentID, sec->getMaLopHP());
                        string lopKhac = system.getRegisteredSectionForCourse(studentID, sec->getMaMon());

                        ConsoleView::printSectionChoiceHeader(j + 1, *sec, c);
                        cout << setw(18) << sec->getGiangVien()
                             << " | Si so: " << sec->getSoLuongDaDangKy() << "/" << sec->getSiSoToiDa();
                        if (daDK) {
                            cout << " -> [DA DANG KY]";
                        } else if (!lopKhac.empty()) {
                            cout << " -> [DA DK O LOP KHAC: " << lopKhac << " - Can huy lop cu truoc]";
                        } else {
                            cout << " -> [CHUA DANG KY]";
                        }
                        cout << "\n";
                    }
                }
                cout << "[0] Quay lai\n";
                cout << "Nhap so thu tu lop hoc phan muon dang ky (1 - " << chosenGroup.totalSections << ", hoac 0 de quay lai): ";
                int sIndex = 0;
                if (!ConsoleView::readChoice(sIndex) || sIndex <= 0 || sIndex > chosenGroup.totalSections) {
                    if (sIndex != 0) cout << "Lua chon khong hop le.\n";
                    continue;
                }

                string targetSectionID = chosenGroup.sectionIDs[sIndex - 1];
                SessionHelper::tryRegisterWithFeedback(system, studentID, targetSectionID);

                SessionHelper::showSemesterStatusView(system, studentID);
            } else if (choice == 6) {
                const int MAX_REG = 50;
                string regSectionIDs[MAX_REG];
                int regCount = system.getStudentRegisteredSections(studentID, regSectionIDs, MAX_REG);

                if (regCount == 0) {
                    cout << "\n>> Hien tai ban chua dang ky lop hoc phan nao.\n";
                    continue;
                }

                cout << "  DANH SACH LOP HOC PHAN DA DANG KY - CHON LOP DE HUY\n";
                for (int i = 0; i < regCount; i++) {
                    const ClassSection* sec = system.findClassSection(regSectionIDs[i]);
                    if (sec != nullptr) {
                        const Course* c = system.findCourse(sec->getMaMon());
                        ConsoleView::printSectionChoiceHeader(i + 1, *sec, c);
                        cout << sec->getGiangVien() << "\n";
                    }
                }
                cout << "[0] Quay lai (Khong huy)\n";
                cout << "Nhap so thu tu lop hoc phan ban muon huy (1 - " << regCount << ", hoac 0 de quay lai): ";
                int cancelIndex = 0;
                if (!ConsoleView::readChoice(cancelIndex) || cancelIndex <= 0 || cancelIndex > regCount) {
                    if (cancelIndex != 0) cout << "Lua chon khong hop le.\n";
                    continue;
                }

                string secToCancel = regSectionIDs[cancelIndex - 1];
                const ClassSection* sec = system.findClassSection(secToCancel);
                const Course* c = (sec ? system.findCourse(sec->getMaMon()) : nullptr);

                DoubleLinkedList<string> waitlistNotices;
                bool success = system.cancelRegistration(studentID, secToCancel, &waitlistNotices);
                ConsoleView::printNotices(waitlistNotices);
                if (success) {
                    cout << ">> Da huy thanh cong lop hoc phan '" << secToCancel << "' ("
                         << (c ? c->getTenMon() : "") << ")!\n";
                } else {
                    cout << ">> Huy dang ky that bai!\n";
                }

                SessionHelper::showSemesterStatusView(system, studentID);
            } else if (choice == 7) {
                SessionHelper::showSemesterStatusView(system, studentID);
            } else if (choice == 8) {
                cout << "Nhap ma lop hoc phan can kiem tra lich: ";
                string sectionID;
                getline(cin, sectionID);
                ClassSection target;
                DoubleLinkedList<ClassSection> conflicts;
                if (!system.checkTimeConflictDetails(studentID, sectionID, target, conflicts)) {
                    cout << "Khong tim thay lop hoc phan: " << sectionID << "\n";
                } else {
                    ConsoleView::displayTimeConflictResult(target, conflicts);
                }
            } else if (choice == 9) {
                DoubleLinkedList<StudentRegistrationDetail> details;
                system.getStudentRegistrations(studentID, details, system.getCurrentSemester());
                ConsoleView::displayStudentRegistrations(studentID, details);
            } else if (choice == 10) {
                DoubleLinkedList<TimetableEntry> timetable;
                system.getStudentTimetable(studentID, timetable);
                ConsoleView::displayStudentTimetable(studentID, system.getCurrentSemester(), timetable);
            } else if (choice == 11) {
                DoubleLinkedList<CreditSummaryItem> items;
                int totalCredits = system.calculateTotalCredits(studentID, &items, system.getCurrentSemester());
                ConsoleView::displayCreditSummary(studentID, totalCredits, items);
                SessionHelper::showSemesterStatusView(system, studentID);
            } else {
                cout << "Lua chon khong hop le.\n";
            }
        }
    }
};

#endif // STUDENT_SESSION_H
