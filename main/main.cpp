#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

#include "../manager/filemanager.h"

using namespace std;

namespace {

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

void showRoleMenu(int currentSemester) {
    cout << "\n============================================================\n"
         << "           HE THONG DANG KY TIN CHI (PBL2)\n"
         << "              Hoc ky he thong: Hoc ky " << currentSemester << "\n"
         << "============================================================\n"
         << "1. Dang nhap SINH VIEN\n"
         << "2. Dang nhap QUAN TRI VIEN (ADMIN)\n"
         << "0. Luu va Thoat\n"
         << "------------------------------------------------------------\n"
         << "Nhap lua chon: ";
}

void showStudentMenu(int currentSemester, const Student& student) {
    cout << "\n============================================================\n"
         << "   MENU SINH VIEN - " << student.getHoTen() << " (MSSV: " << student.getMaSV() << ")\n"
         << "   Lop: " << (student.getLop().empty() ? "-" : student.getLop())
         << " | Khoa: " << (student.getKhoa().empty() ? "-" : student.getKhoa())
         << " | Nganh: " << (student.getNganhHoc().empty() ? "-" : student.getNganhHoc())
         << " | Hoc ky: " << currentSemester << "\n"
         << "============================================================\n"
         << " 1. Xem thong tin ca nhan\n"
         << " 2. Xem danh sach mon hoc (chuong trinh dao tao)\n"
         << " 3. Tim kiem mon hoc\n"
         << " 4. Xem danh sach lop hoc phan (theo nhom lop)\n"
         << " 5. Dang ky lop hoc phan (chon nhom -> chon lop)\n"
         << " 6. Huy dang ky lop hoc phan (chon tu danh sach da dang ky)\n"
         << " 7. Kiem tra tien do dang ky mon hoc trong hoc ky\n"
         << " 8. Kiem tra trung lich hoc\n"
         << " 9. Xem cac hoc phan da dang ky\n"
         << "10. Xem thoi khoa bieu\n"
         << "11. Tinh so tin chi da dang ky\n"
         << " 0. Dang xuat (Quay ve man hinh chinh)\n"
         << "------------------------------------------------------------\n"
         << "Nhap lua chon: ";
}

void showAdminMenu(int currentSemester) {
    cout << "\n============================================================\n"
         << "                     MENU QUAN TRI VIEN\n"
         << "                  Hoc ky he thong: Hoc ky " << currentSemester << "\n"
         << "============================================================\n"
         << "--- QUAN LY SINH VIEN ---\n"
         << " 1. Them sinh vien\n"
         << " 2. Xoa sinh vien\n"
         << " 3. Sua thong tin sinh vien\n"
         << " 4. Tim kiem sinh vien\n"
         << " 5. Xem danh sach tat ca sinh vien\n"
         << "\n--- QUAN LY MON HOC ---\n"
         << " 6. Them mon hoc\n"
         << " 7. Xoa mon hoc\n"
         << " 8. Sua thong tin mon hoc\n"
         << " 9. Tim kiem mon hoc\n"
         << "10. Xem danh sach mon hoc (tat ca / theo bo loc)\n"
         << "\n--- QUAN LY LOP HOC PHAN ---\n"
         << "11. Them lop hoc phan (co nhom lop HP)\n"
         << "12. Xoa lop hoc phan\n"
         << "13. Sua thong tin lop hoc phan\n"
         << "14. Tim kiem lop hoc phan\n"
         << "15. Xem danh sach lop hoc phan (theo nhom / tat ca)\n"
         << "16. Xem danh sach sinh vien theo lop hoc phan\n"
         << "\n--- DANG KY & HOC KY ---\n"
         << "17. Dang ky lop hoc phan cho sinh vien\n"
         << "18. Huy dang ky lop hoc phan cho sinh vien\n"
         << "19. Xem tat ca dang ky trong he thong\n"
         << "20. Thiet lap hoc ky he thong (Admin quyet dinh ki 1 hoac 2)\n"
         << "\n 0. Dang xuat (Quay ve man hinh chinh)\n"
         << "------------------------------------------------------------\n"
         << "Nhap lua chon: ";
}

void studentSession(SystemManager& system, FileManager& files, const string& studentID) {
    Student* student = system.findStudent(studentID);
    if (student == nullptr) {
        cout << "Loi: Khong tim thay thong tin sinh vien!\n";
        return;
    }

    while (true) {
        showStudentMenu(system.getCurrentSemester(), *student);
        int choice;
        if (!readChoice(choice)) {
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
                    Course* c = system.findCourse(missingIDs[m]);
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
                if (!readChoice(exitChoice) || exitChoice != 1) {
                    cout << ">> Quay lai menu de tiep tuc dang ky mon con thieu.\n";
                    continue;
                }
            }

            cout << "\n>>> Da dang xuat khoi tai khoan sinh vien.\n";
            string err;
            files.saveAllData(system, err);
            break;
        } else if (choice == 1) {
            student->xuat();
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
            readChoice(subChoice);
            if (subChoice == 1) {
                system.displayCoursesByFilter(student->getKhoa(), student->getKhoaHoc(),
                                              student->getNganhHoc(), system.getCurrentSemester());
            } else if (subChoice == 2) {
                system.displayCoursesByFilter("", "", "", system.getCurrentSemester());
            } else {
                system.displayAllCourses();
            }
        } else if (choice == 3) {
            cout << "Nhap tu khoa tim kiem mon hoc (ma mon, ten mon): ";
            string kw;
            getline(cin, kw);
            system.searchCourses(kw);
        } else if (choice == 4) {
            system.displaySectionsByGroup();
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
            if (!readChoice(gIndex) || gIndex <= 0 || gIndex > groupCount) {
                if (gIndex != 0) cout << "Lua chon khong hop le.\n";
                continue;
            }

            GroupInfo chosenGroup = eligibleGroups[gIndex - 1];

            cout << "\n  BUOC 2: CHON LOP HOC PHAN TRONG NHOM '" << chosenGroup.groupName << "'\n";
            for (int j = 0; j < chosenGroup.totalSections; j++) {
                ClassSection* sec = system.findClassSection(chosenGroup.sectionIDs[j]);
                if (sec != nullptr) {
                    Course* c = system.findCourse(sec->getMaMon());
                    bool daDK = system.isSectionRegistered(studentID, sec->getMaLopHP());
                    string lopKhac = system.getRegisteredSectionForCourse(studentID, sec->getMaMon());

                    cout << "[" << (j + 1) << "] " << setw(22) << left << sec->getMaLopHP()
                         << " | Mon: " << setw(20) << left << (c ? c->getTenMon() : sec->getMaMon())
                         << " (" << (c ? c->getSoTinChi() : 0) << " TC)\n"
                         << "      Thu " << sec->getThu()
                         << " | Tiet " << sec->getTietBatDau() << "-" << sec->getTietKetThuc()
                         << " | Phong: " << setw(6) << sec->getPhong()
                         << " | GV: " << setw(18) << sec->getGiangVien()
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
            if (!readChoice(sIndex) || sIndex <= 0 || sIndex > chosenGroup.totalSections) {
                if (sIndex != 0) cout << "Lua chon khong hop le.\n";
                continue;
            }

            string targetSectionID = chosenGroup.sectionIDs[sIndex - 1];
            bool success = system.registerClass(studentID, targetSectionID);
            cout << (success ? ">> Dang ky lop hoc phan thanh cong!\n" : ">> Dang ky that bai!\n");

            system.displaySemesterRegistrationStatus(studentID);
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
                ClassSection* sec = system.findClassSection(regSectionIDs[i]);
                if (sec != nullptr) {
                    Course* c = system.findCourse(sec->getMaMon());
                    cout << "[" << (i + 1) << "] " << setw(22) << left << sec->getMaLopHP()
                         << " | Mon: " << setw(20) << left << (c ? c->getTenMon() : sec->getMaMon())
                         << " (" << (c ? c->getSoTinChi() : 0) << " TC)\n"
                         << "      Thu " << sec->getThu()
                         << " | Tiet " << sec->getTietBatDau() << "-" << sec->getTietKetThuc()
                         << " | Phong: " << setw(6) << sec->getPhong()
                         << " | GV: " << sec->getGiangVien() << "\n";
                }
            }
            cout << "[0] Quay lai (Khong huy)\n";
            cout << "Nhap so thu tu lop hoc phan ban muon huy (1 - " << regCount << ", hoac 0 de quay lai): ";
            int cancelIndex = 0;
            if (!readChoice(cancelIndex) || cancelIndex <= 0 || cancelIndex > regCount) {
                if (cancelIndex != 0) cout << "Lua chon khong hop le.\n";
                continue;
            }

            string secToCancel = regSectionIDs[cancelIndex - 1];
            ClassSection* sec = system.findClassSection(secToCancel);
            Course* c = (sec ? system.findCourse(sec->getMaMon()) : nullptr);

            bool success = system.cancelRegistration(studentID, secToCancel);
            if (success) {
                cout << ">> Da huy thanh cong lop hoc phan '" << secToCancel << "' ("
                     << (c ? c->getTenMon() : "") << ")!\n";
            } else {
                cout << ">> Huy dang ky that bai!\n";
            }

            system.displaySemesterRegistrationStatus(studentID);
        } else if (choice == 7) {
            system.displaySemesterRegistrationStatus(studentID);
        } else if (choice == 8) {
            cout << "Nhap ma lop hoc phan can kiem tra lich: ";
            string sectionID;
            getline(cin, sectionID);
            system.checkTimeConflictInteractive(studentID, sectionID);
        } else if (choice == 9) {
            system.displayStudentRegistrations(studentID);
        } else if (choice == 10) {
            system.displayStudentTimetable(studentID);
        } else if (choice == 11) {
            system.calculateTotalCredits(studentID);
            system.displaySemesterRegistrationStatus(studentID);
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
}

void adminSession(SystemManager& system, FileManager& files) {
    while (true) {
        showAdminMenu(system.getCurrentSemester());
        int choice;
        if (!readChoice(choice)) {
            if (cin.eof()) break;
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        if (choice == 0) {
            cout << "\n>>> Da dang xuat khoi quyen quan tri vien.\n";
            string err;
            files.saveAllData(system, err);
            break;
        }

        if (choice == 1) {
            Student sv;
            sv.nhap();
            if (system.addStudent(sv)) {
                cout << ">> Them sinh vien moi thanh cong!\n";
            } else {
                cout << ">> Them that bai: Ma sinh vien da ton tai hoac du lieu khong hop le.\n";
            }
        } else if (choice == 2) {
            cout << "Nhap ma sinh vien can xoa: ";
            string svID;
            getline(cin, svID);
            if (system.deleteStudent(svID)) {
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
                cout << "\n--- THONG TIN SINH VIEN HIEN TAI ---\n";
                sv->xuat();
                cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
                string val;
                cout << "Ho ten moi [" << sv->getHoTen() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setHoTen(val);

                cout << "So dien thoai moi [" << sv->getSoDienThoai() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setSoDienThoai(val);

                cout << "Email moi [" << sv->getEmail() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setEmail(val);

                cout << "Khoa moi [" << sv->getKhoa() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setKhoa(val);

                cout << "Khoa hoc moi [" << sv->getKhoaHoc() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setKhoaHoc(val);

                cout << "Lop moi [" << sv->getLop() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setLop(val);

                cout << "Nganh hoc moi [" << sv->getNganhHoc() << "]: ";
                getline(cin, val);
                if (!val.empty()) sv->setNganhHoc(val);

                cout << ">> Cap nhat thong tin sinh vien thanh cong!\n";
            }
        } else if (choice == 4) {
            cout << "Nhap tu khoa tim kiem sinh vien (ma SV, ho ten, lop, khoa): ";
            string kw;
            getline(cin, kw);
            system.searchStudents(kw);
        } else if (choice == 5) {
            system.displayAllStudents();
        } else if (choice == 6) {
            Course c;
            c.nhap();
            if (system.addCourse(c)) {
                cout << ">> Them mon hoc moi thanh cong!\n";
            } else {
                cout << ">> Them that bai: Ma mon da ton tai hoac so tin chi khong hop le.\n";
            }
        } else if (choice == 7) {
            cout << "Nhap ma mon hoc can xoa: ";
            string courseID;
            getline(cin, courseID);
            if (system.deleteCourse(courseID)) {
                cout << ">> Xoa mon hoc thanh cong!\n";
            } else {
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
                cout << "\n--- THONG TIN MON HOC HIEN TAI ---\n";
                c->xuat();
                cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
                string val;
                cout << "Ten mon moi [" << c->getTenMon() << "]: ";
                getline(cin, val);
                if (!val.empty()) c->setTenMon(val);

                cout << "So tin chi moi [" << c->getSoTinChi() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int tc = stoi(val);
                        if (tc > 0) c->setSoTinChi(tc);
                    } catch (...) {}
                }

                cout << "Khoa quan ly moi [" << c->getKhoa() << "]: ";
                getline(cin, val);
                if (!val.empty()) c->setKhoa(val);

                cout << "Khoa hoc ap dung moi [" << c->getKhoaHoc() << "]: ";
                getline(cin, val);
                if (!val.empty()) c->setKhoaHoc(val);

                cout << "Nganh hoc ap dung moi [" << c->getNganhHoc() << "]: ";
                getline(cin, val);
                if (!val.empty()) c->setNganhHoc(val);

                cout << "Hoc ky ap dung moi  [" << c->getHocKy() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int hk = stoi(val);
                        if (hk == 1 || hk == 2) c->setHocKy(hk);
                    } catch (...) {}
                }

                cout << ">> Cap nhat mon hoc thanh cong!\n";
            }
        } else if (choice == 9) {
            cout << "Nhap tu khoa tim kiem mon hoc (ma mon, ten mon, khoa, nganh): ";
            string kw;
            getline(cin, kw);
            system.searchCourses(kw);
        } else if (choice == 10) {
            cout << "1. Xem tat ca mon hoc\n";
            cout << "2. Loc theo Khoa, Khoa hoc, Nganh, Hoc ky\n";
            cout << "Chon (1-2): ";
            int subChoice = 1;
            readChoice(subChoice);
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
                } catch (...) {
                    hk = 0;
                }
                system.displayCoursesByFilter(khoa, khoaHoc, nganh, hk);
            } else {
                system.displayAllCourses();
            }
        } else if (choice == 11) {
            ClassSection sec;
            sec.nhap();
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
                cout << "\n--- THONG TIN LOP HOC PHAN HIEN TAI ---\n";
                sec->xuat();
                cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
                string val;
                cout << "Nhom lop HP moi [" << sec->getNhomLopHP() << "]: ";
                getline(cin, val);
                if (!val.empty()) sec->setNhomLopHP(val);

                cout << "Giang vien moi [" << sec->getGiangVien() << "]: ";
                getline(cin, val);
                if (!val.empty()) sec->setGiangVien(val);

                cout << "Thu hoc moi [" << sec->getThu() << "]: ";
                getline(cin, val);
                if (!val.empty()) sec->setThu(val);

                cout << "Tiet bat dau moi [" << sec->getTietBatDau() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int t = stoi(val);
                        if (t > 0) sec->setTietBatDau(t);
                    } catch (...) {}
                }

                cout << "Tiet ket thuc moi [" << sec->getTietKetThuc() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int t = stoi(val);
                        if (t > sec->getTietBatDau()) sec->setTietKetThuc(t);
                    } catch (...) {}
                }

                cout << "Phong hoc moi [" << sec->getPhong() << "]: ";
                getline(cin, val);
                if (!val.empty()) sec->setPhong(val);

                cout << "Si so toi da moi [" << sec->getSiSoToiDa() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int ss = stoi(val);
                        if (ss > 0) sec->setSiSoToiDa(ss);
                    } catch (...) {}
                }

                cout << "Hoc ky mo lop moi [" << sec->getHocKy() << "]: ";
                getline(cin, val);
                if (!val.empty()) {
                    try {
                        int hk = stoi(val);
                        if (hk == 1 || hk == 2) sec->setHocKy(hk);
                    } catch (...) {}
                }

                cout << ">> Cap nhat thong tin lop hoc phan thanh cong!\n";
            }
        } else if (choice == 14) {
            cout << "Nhap tu khoa tim kiem (Ma lop HP, ma mon, nhom, giang vien): ";
            string kw;
            getline(cin, kw);
            system.searchSections(kw);
        } else if (choice == 15) {
            cout << "1. Xem theo tung NHOM lop hoc phan (theo hoc ky " << system.getCurrentSemester() << ")\n";
            cout << "2. Xem tat ca cac lop hoc phan\n";
            cout << "Chon (1-2): ";
            int subChoice = 1;
            readChoice(subChoice);
            if (subChoice == 1) {
                system.displaySectionsByGroup();
            } else {
                system.displayAllSections();
            }
        } else if (choice == 16) {
            cout << "Nhap ma lop hoc phan can xem danh sach sinh vien: ";
            string secID;
            getline(cin, secID);
            system.displayStudentsInSection(secID);
        }

        else if (choice == 17) {
            string svID, secID;
            cout << "Ma sinh vien: ";
            getline(cin, svID);
            cout << "Ma lop hoc phan: ";
            getline(cin, secID);
            bool success = system.registerClass(svID, secID);
            cout << (success ? ">> Dang ky lop hoc phan thanh cong!\n" : ">> Dang ky that bai!\n");
        } else if (choice == 18) {
            string svID, secID;
            cout << "Ma sinh vien: ";
            getline(cin, svID);
            cout << "Ma lop hoc phan: ";
            getline(cin, secID);
            bool success = system.cancelRegistration(svID, secID);
            cout << (success ? ">> Huy dang ky lop hoc phan thanh cong!\n" : ">> Huy dang ky that bai!\n");
        } else if (choice == 19) {
            system.displayAllRegistrations();
        } else if (choice == 20) {
            cout << "\n--- THIET LAP KI HOC HE THONG ---\n";
            cout << "Ki hoc hien tai: Ki " << system.getCurrentSemester() << '\n';
            cout << "Nhap ki hoc muon ap dung: ";
            string semStr;
            getline(cin, semStr);
            int newSem = 0;
            try {
                newSem = stoi(semStr);
            } catch (...) {
                newSem = 0;
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

int main() {
    SystemManager system;
    FileManager files;
    string error;

    if (!files.loadAllData(system, error)) {
        cerr << "Khong the nap du lieu: " << error << '\n';
        return 1;
    }

    while (true) {
        showRoleMenu(system.getCurrentSemester());
        int roleChoice;
        if (!readChoice(roleChoice)) {
            if (cin.eof()) {
                roleChoice = 0;
            } else {
                cout << "Lua chon khong hop le.\n";
                continue;
            }
        }

        if (roleChoice == 0) {
            if (!files.saveAllData(system, error)) {
                cerr << "Khong the luu du lieu: " << error << '\n';
                return 1;
            }
            cout << "\n>> Da luu toan bo du lieu. Tam biet!\n";
            return 0;
        } else if (roleChoice == 1) {
            cout << "\n--- DANG NHAP SINH VIEN ---\n";
            cout << "Nhap Ma sinh vien: ";
            string studentID;
            getline(cin, studentID);
            if (studentID.empty()) {
                continue;
            }

            Student* s = system.findStudent(studentID);
            if (s == nullptr) {
                cout << ">> Khong tim thay sinh vien co ma '" << studentID << "' trong he thong!\n";
                cout << ">> Vui long kiem tra lai hoac lien he Quan tri vien de duoc them vao he thong.\n";
                continue;
            }

            cout << "\n>> Dang nhap thanh cong! Xin chao: " << s->getHoTen()
                 << " (MSSV: " << s->getMaSV() << ")\n";
            studentSession(system, files, studentID);
        } else if (roleChoice == 2) {
            cout << "\n--- DANG NHAP QUAN TRI VIEN ---\n";
            cout << "Tai khoan (mac dinh: admin): ";
            string username;
            getline(cin, username);
            cout << "Mat khau (mac dinh: admin): ";
            string password;
            getline(cin, password);

            if ((username == "admin" || username.empty()) &&
                (password == "admin" || password.empty())) {
                cout << "\n>> Dang nhap Quan tri vien thanh cong!\n";
                adminSession(system, files);
            } else {
                cout << ">> Sai tai khoan hoac mat khau quan tri!\n";
            }
        } else {
            cout << "Lua chon khong hop le. Vui long chon lai.\n";
        }
    }
}
