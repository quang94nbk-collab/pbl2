#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../manager/authmanager.h"
#include "../manager/exception.h"
#include "../manager/system.h"
#include "../ui/ConsoleView.h"

using namespace std;

namespace {

bool saveAndExit(const SystemManager& system, string& error) {
    if (!system.saveData(error)) {
        cerr << "Khong the luu du lieu: " << error << '\n';
        return false;
    }
    cout << "\n>> Da luu toan bo du lieu. Tam biet!\n";
    return true;
}

void showSectionsByGroupView(const SystemManager& system) {
    cout << "  DANH SACH NHOM LOP HOC PHAN - HOC KY " << system.getCurrentSemester() << "\n";
    const int MAX_GROUPS = 100;
    string groupNames[MAX_GROUPS];
    int groupCount = system.getGroupsForCurrentSemester(groupNames, MAX_GROUPS);

    if (groupCount == 0) {
        cout << "(Khong co nhom lop hoc phan nao trong hoc ky " << system.getCurrentSemester() << ")\n";
    } else {
        for (int g = 0; g < groupCount; g++) {
            cout << "\n>>> NHOM: " << groupNames[g] << "\n";
            DoubleLinkedList<SectionDetailInfo> list;
            system.getSectionsByGroupInCurrentSemester(groupNames[g], list);
            ConsoleView::displayGroupSectionList(list, true);
        }
    }

    DoubleLinkedList<SectionDetailInfo> ungrouped;
    system.getUngroupedSectionsInCurrentSemester(ungrouped);
    if (!ungrouped.empty()) {
        cout << "\n>>> CHUA GAN NHOM:\n";
        ConsoleView::displayGroupSectionList(ungrouped, false);
    }
}

void tryRegisterWithFeedback(SystemManager& system, const string& studentID, const string& sectionID) {
    try {
        system.registerClass(studentID, sectionID);
        cout << ">> Dang ky lop hoc phan thanh cong!\n";
    } catch (const ClassFullException& e) {
        cout << ">> [Lop day] " << e.what() << "\n";
    } catch (const TimeConflictException& e) {
        cout << ">> [Trung lich] " << e.what() << "\n";
    } catch (const DuplicateException& e) {
        cout << ">> [Trung lap] " << e.what() << "\n";
    } catch (const WrongSemesterException& e) {
        cout << ">> [Sai hoc ky] " << e.what() << "\n";
    } catch (const NotEligibleException& e) {
        cout << ">> [Khong du dieu kien] " << e.what() << "\n";
    } catch (const AppException& e) {
        cout << ">> Dang ky that bai: " << e.what() << "\n";
    }
}

void showSemesterStatusView(const SystemManager& system, const string& studentID) {
    SemesterProgressSummary summary;
    DoubleLinkedList<CourseProgressItem> items;
    if (system.getSemesterRegistrationStatus(studentID, summary, items)) {
        ConsoleView::displaySemesterRegistrationStatus(summary, items);
    }
}

void studentSession(SystemManager& system, const string& studentID) {
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
            showSectionsByGroupView(system);
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
            tryRegisterWithFeedback(system, studentID, targetSectionID);

            showSemesterStatusView(system, studentID);
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

            showSemesterStatusView(system, studentID);
        } else if (choice == 7) {
            showSemesterStatusView(system, studentID);
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
            showSemesterStatusView(system, studentID);
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
}

void adminSession(SystemManager& system, AuthManager& auth) {
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
            cout << "1. Xem tat ca mon hoc\n";
            cout << "2. Loc theo Khoa, Khoa hoc, Nganh, Hoc ky\n";
            cout << "Chon (1-2): ";
            int subChoice = 1;
            ConsoleView::readChoice(subChoice);
            DoubleLinkedList<Course> courses;
            if (subChoice == 2) {
                string khoa, khoaHoc, nganh;
                cout << "Khoa: ";
                getline(cin, khoa);
                cout << "Khoa hoc: ";
                getline(cin, khoaHoc);
                cout << "Nganh: ";
                getline(cin, nganh);
                cout << "Hoc ky: ";
                string hkStr;
                getline(cin, hkStr);
                int hk = 0;
                try {
                    if (!hkStr.empty()) hk = stoi(hkStr);
                } catch (const invalid_argument&) {
                    hk = 0;
                } catch (const out_of_range&) {
                    hk = 0;
                }
                system.filterCourses(khoa, khoaHoc, nganh, hk, courses);
                ConsoleView::displayFilteredCourses(khoa, khoaHoc, nganh, hk, courses);
            } else {
                system.getAllCourses(courses);
                ConsoleView::displayCourses(courses);
            }
        } else if (choice == 11) {
            ClassSection sec;
            cin >> sec;
            if (system.addClassSection(sec)) {
                cout << ">> Them lop hoc phan thanh cong!\n";
            } else {
                cout << ">> Them that bai: Ma lop HP da ton tai hoac ma mon khong ton tai trong he thong.\n";
            }
        } else if (choice == 12) {
            cout << "Nhap ma lop hoc phan can xoa: ";
            string secID;
            getline(cin, secID);
            if (system.deleteClassSection(secID)) {
                cout << ">> Xoa lop hoc phan thanh cong (da huy cac dang ky lien quan).\n";
            } else {
                cout << ">> Xoa lop hoc phan that bai.\n";
            }
        } else if (choice == 13) {
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
        } else if (choice == 14) {
            cout << "Nhap tu khoa tim kiem (Ma lop HP, ma mon, nhom, giang vien): ";
            string kw;
            getline(cin, kw);
            DoubleLinkedList<ClassSection> results;
            system.searchSections(kw, results);
            ConsoleView::displaySectionSearchResults(kw, results);
        } else if (choice == 15) {
            cout << "1. Xem theo tung NHOM lop hoc phan (theo hoc ky " << system.getCurrentSemester() << ")\n";
            cout << "2. Xem tat ca cac lop hoc phan\n";
            cout << "Chon (1-2): ";
            int subChoice = 1;
            ConsoleView::readChoice(subChoice);
            if (subChoice == 1) {
                showSectionsByGroupView(system);
            } else {
                DoubleLinkedList<ClassSection> sections;
                system.getAllSections(sections);
                ConsoleView::displaySections(sections);
            }
        } else if (choice == 16) {
            cout << "Nhap ma lop hoc phan can xem danh sach sinh vien: ";
            string secID;
            getline(cin, secID);
            ClassSection sec;
            DoubleLinkedList<SectionStudentInfo> students;
            if (!system.getStudentsInSection(secID, sec, students)) {
                cout << "Khong tim thay lop hoc phan: " << secID << "\n";
            } else {
                ConsoleView::displayStudentsInSection(sec, students);
            }
        } else if (choice == 17) {
            string svID, secID;
            cout << "Ma sinh vien: ";
            getline(cin, svID);
            cout << "Ma lop hoc phan: ";
            getline(cin, secID);
            tryRegisterWithFeedback(system, svID, secID);
        } else if (choice == 18) {
            string svID, secID;
            cout << "Ma sinh vien: ";
            getline(cin, svID);
            cout << "Ma lop hoc phan: ";
            getline(cin, secID);
            DoubleLinkedList<string> waitlistNotices;
            bool success = system.cancelRegistration(svID, secID, &waitlistNotices);
            ConsoleView::printNotices(waitlistNotices);
            cout << (success ? ">> Huy dang ky lop hoc phan thanh cong!\n" : ">> Huy dang ky that bai!\n");
        } else if (choice == 19) {
            DoubleLinkedList<Registration> registrations;
            system.getAllRegistrations(registrations);
            ConsoleView::displayRegistrations(registrations);
        } else if (choice == 20) {
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

} // namespace

int main() {
    SystemManager system;
    string error;

    if (!system.loadData(error)) {
        cerr << "Khong the nap du lieu: " << error << '\n';
        return 1;
    }

    AuthManager auth;
    if (!auth.addAccount("admin", "admin", UserRole::Administrator)) {
        cerr << "Khong the tao tai khoan quan tri mac dinh.\n";
        return 1;
    }
    DoubleLinkedList<Student> loadedStudents;
    system.getAllStudents(loadedStudents);
    auto studentNode = loadedStudents.getHead();
    while (studentNode != nullptr) {
        const string& studentID = studentNode->data.getMaSV();
        if (!auth.addAccount(studentID, studentID, UserRole::Student, studentID)) {
            cerr << "Khong the tao tai khoan cho sinh vien: " << studentID
                 << ". Ma sinh vien co the trung ten tai khoan.\n";
            return 1;
        }
        studentNode = studentNode->next;
    }

    while (true) {
        if (!auth.isLoggedIn()) {
            ConsoleView::showMainMenu(system.getCurrentSemester());
            int menuChoice;
            if (!ConsoleView::readChoice(menuChoice)) {
                if (cin.eof()) {
                    return saveAndExit(system, error) ? 0 : 1;
                }
                cout << "Lua chon khong hop le.\n";
                continue;
            }
            if (menuChoice == 0) {
                return saveAndExit(system, error) ? 0 : 1;
            }
            if (menuChoice != 1) {
                cout << "Lua chon khong hop le.\n";
                continue;
            }

            string username, password;
            cout << "\n=== DANG NHAP ===\nTai khoan: ";
            if (!getline(cin >> ws, username)) {
                return saveAndExit(system, error) ? 0 : 1;
            }
            cout << "Mat khau: ";
            if (!getline(cin, password)) {
                return saveAndExit(system, error) ? 0 : 1;
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
            adminSession(system, auth);
            auth.logout();
            continue;
        }
        studentSession(system, user->studentID);
        auth.logout();
        continue;
    }
}
