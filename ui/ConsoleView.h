#ifndef CONSOLE_VIEW_H
#define CONSOLE_VIEW_H

#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "../DSA/DoublyLinkedList.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/dtos.h"
#include "../model/person.h"
#include "../model/registration.h"
#include "../model/student.h"

using namespace std;

// ============================================================
//  Nap chong toan tu nhap / xuat (thuoc tang Presentation)
//  Dung os / is chuan xac, giu tang Model doc lap voi Console
// ============================================================

inline ostream& operator<<(ostream& os, const Person& p) {
    os << "Ma: " << p.getMaNguoi() << '\n'
       << "Ho ten: " << p.getHoTen() << '\n'
       << "So dien thoai: " << p.getSoDienThoai() << '\n'
       << "Email: " << p.getEmail() << '\n';
    return os;
}

inline istream& operator>>(istream& is, Person& p) {
    string ma, ten, sdt, mail;
    cout << "Ma nguoi: ";
    getline(is >> ws, ma);
    cout << "Ho ten: ";
    getline(is, ten);
    cout << "So dien thoai: ";
    getline(is, sdt);
    cout << "Email: ";
    getline(is, mail);
    p.setMaNguoi(ma);
    p.setHoTen(ten);
    p.setSoDienThoai(sdt);
    p.setEmail(mail);
    return is;
}

inline ostream& operator<<(ostream& os, const Student& s) {
    os << "Ma sinh vien: " << s.getMaSV() << '\n'
       << "Ho ten: " << s.getHoTen() << '\n'
       << "So dien thoai: " << s.getSoDienThoai() << '\n'
       << "Email: " << s.getEmail() << '\n'
       << "Khoa: " << (s.getKhoa().empty() ? "Chua cap nhat" : s.getKhoa()) << '\n'
       << "Khoa hoc: " << (s.getKhoaHoc().empty() ? "Chua cap nhat" : s.getKhoaHoc()) << '\n'
       << "Lop: " << s.getLop() << '\n'
       << "Nganh hoc: " << s.getNganhHoc() << '\n';
    return os;
}

inline istream& operator>>(istream& is, Student& s) {
    string ma, ten, sdt, mail, khoa, khoaHoc, lop, nganh;
    cout << "Ma sinh vien: ";
    getline(is >> ws, ma);
    cout << "Ho ten: ";
    getline(is, ten);
    cout << "So dien thoai: ";
    getline(is, sdt);
    cout << "Email: ";
    getline(is, mail);
    cout << "Khoa: ";
    getline(is, khoa);
    cout << "Khoa hoc: ";
    getline(is, khoaHoc);
    cout << "Lop: ";
    getline(is, lop);
    cout << "Nganh hoc: ";
    getline(is, nganh);
    s.setMaSV(ma);
    s.setHoTen(ten);
    s.setSoDienThoai(sdt);
    s.setEmail(mail);
    s.setKhoa(khoa);
    s.setKhoaHoc(khoaHoc);
    s.setLop(lop);
    s.setNganhHoc(nganh);
    return is;
}

inline ostream& operator<<(ostream& os, const Course& c) {
    os << "Ma mon hoc: " << c.getMaMon() << '\n'
       << "Ten mon hoc: " << c.getTenMon() << '\n'
       << "So tin chi: " << c.getSoTinChi() << '\n'
       << "Khoa: " << (c.getKhoa().empty() ? "Tat ca" : c.getKhoa()) << '\n'
       << "Khoa hoc ap dung: " << (c.getKhoaHoc().empty() ? "Tat ca" : c.getKhoaHoc()) << '\n'
       << "Nganh hoc ap dung: " << (c.getNganhHoc().empty() ? "Tat ca" : c.getNganhHoc()) << '\n'
       << "Hoc ky: " << c.getHocKy() << '\n';
    return os;
}

inline istream& operator>>(istream& is, Course& c) {
    string ma, ten, khoa, khoaHoc, nganh;
    cout << "Ma mon hoc: ";
    getline(is >> ws, ma);
    cout << "Ten mon hoc: ";
    getline(is, ten);

    int tinChi = 0;
    while (true) {
        cout << "So tin chi: ";
        if (is >> tinChi && tinChi > 0) {
            is.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "So tin chi phai lon hon 0. Nhap lai.\n";
    }

    cout << "Khoa quan ly : ";
    getline(is, khoa);
    cout << "Khoa hoc ap dung : ";
    getline(is, khoaHoc);
    cout << "Nganh hoc : ";
    getline(is, nganh);

    int hk = 1;
    while (true) {
        cout << "Hoc ky ap dung : ";
        if (is >> hk && (hk == 1 || hk == 2)) {
            is.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Vui long chon 1 hoac 2.\n";
    }

    c.setMaMon(ma);
    c.setTenMon(ten);
    c.setSoTinChi(tinChi);
    c.setKhoa(khoa);
    c.setKhoaHoc(khoaHoc);
    c.setNganhHoc(nganh);
    c.setHocKy(hk);
    return is;
}

inline ostream& operator<<(ostream& os, const ClassSection& sec) {
    os << "Ma lop hoc phan: " << sec.getMaLopHP() << '\n'
       << "Ma mon hoc: " << sec.getMaMon() << '\n'
       << "Nhom lop HP: " << (sec.getNhomLopHP().empty() ? "(Chua gan nhom)" : sec.getNhomLopHP()) << '\n'
       << "Giang vien: " << sec.getGiangVien() << '\n'
       << "Thu hoc: " << sec.getThu() << '\n'
       << "Tiet: " << sec.getTietBatDau() << " - " << sec.getTietKetThuc() << '\n'
       << "Phong: " << sec.getPhong() << '\n'
       << "Si so: " << sec.getSoLuongDaDangKy() << "/" << sec.getSiSoToiDa() << '\n'
       << "Hoc ky: " << sec.getHocKy() << '\n';
    return os;
}

inline istream& operator>>(istream& is, ClassSection& sec) {
    string maLop, maMon, nhom, gv, thu, phong;
    int tietDau = 0, tietCuoi = 0, siSo = 50, hk = 1;

    cout << "Ma lop hoc phan: ";
    getline(is >> ws, maLop);
    cout << "Ma mon hoc: ";
    getline(is, maMon);
    cout << "Nhom lop hoc phan : ";
    getline(is, nhom);
    cout << "Giang vien: ";
    getline(is, gv);
    cout << "Thu hoc : ";
    getline(is, thu);

    while (true) {
        cout << "Tiet bat dau: ";
        if (is >> tietDau && tietDau > 0) break;
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tiet bat dau phai la so nguyen duong.\n";
    }
    while (true) {
        cout << "Tiet ket thuc : ";
        if (is >> tietCuoi && tietCuoi > tietDau) break;
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tiet ket thuc phai lon hon tiet bat dau.\n";
    }
    is.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Phong hoc: ";
    getline(is, phong);

    while (true) {
        cout << "Si so toi da: ";
        if (is >> siSo && siSo > 0) break;
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Si so toi da phai lon hon 0.\n";
    }
    is.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "Hoc ky mo lop: ";
        if (is >> hk && (hk == 1 || hk == 2)) {
            is.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        if (is.eof()) break;
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Vui long chon 1 hoac 2.\n";
    }

    sec.setMaLopHP(maLop);
    sec.setMaMon(maMon);
    sec.setNhomLopHP(nhom);
    sec.setGiangVien(gv);
    sec.setThu(thu);
    sec.setTietBatDau(tietDau);
    sec.setTietKetThuc(tietCuoi);
    sec.setPhong(phong);
    sec.setSiSoToiDa(siSo);
    sec.setSoLuongDaDangKy(0);
    sec.setHocKy(hk);
    return is;
}

inline ostream& operator<<(ostream& os, const Registration& reg) {
    os << "Phieu DK: " << reg.getMaDangKy()
       << " | Ma SV: " << reg.getMaSV()
       << " | Lop HP: " << reg.getMaLopHP()
       << " | Ngay: " << reg.getNgayDangKy() << '\n';
    return os;
}

// ============================================================
//  ConsoleView - Cac ham hien thi va nhap lieu giao dien Console
// ============================================================
namespace ConsoleView {

inline bool readChoice(int& choice) {
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

inline void showMainMenu(int currentSemester) {
    cout << "\n============================================================\n"
         << "           HE THONG DANG KY TIN CHI (PBL2)\n"
         << "              Hoc ky he thong: Hoc ky " << currentSemester << "\n"
         << "============================================================\n"
         << "1. Dang nhap\n"
         << "0. Luu va Thoat\n"
         << "------------------------------------------------------------\n"
         << "Nhap lua chon: ";
}

inline void showStudentMenu(int currentSemester, const Student& student) {
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

inline void showAdminMenu(int currentSemester) {
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

inline void printNotices(const DoubleLinkedList<string>& notices) {
    auto cur = notices.getHead();
    while (cur != nullptr) {
        cout << cur->data << '\n';
        cur = cur->next;
    }
}

inline void displayStudents(const DoubleLinkedList<Student>& list) {
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Danh sach sinh vien trong)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cout << "----------------------\n";
        cur = cur->next;
    }
}

inline void displayStudentSearchResults(const string& keyword, const DoubleLinkedList<Student>& list) {
    cout << "  KET QUA TIM KIEM SINH VIEN (Tu khoa: \"" << keyword << "\")\n";
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Khong tim thay sinh vien nao phu hop)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displayCourses(const DoubleLinkedList<Course>& list) {
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Danh sach mon hoc trong)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displayFilteredCourses(const string& khoa, const string& khoaHoc,
                                   const string& nganh, int hocKy,
                                   const DoubleLinkedList<Course>& list) {
    cout << "  DANH SACH MON HOC\n";
    cout << "  - Khoa:      " << (khoa.empty()     ? "TAT CA" : khoa)     << "\n";
    cout << "  - Khoa hoc:  " << (khoaHoc.empty()  ? "TAT CA" : khoaHoc)  << "\n";
    cout << "  - Nganh hoc: " << (nganh.empty()    ? "TAT CA" : nganh)    << "\n";
    cout << "  - Hoc ky:    " << (hocKy == 0       ? "TAT CA" : to_string(hocKy)) << "\n";
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Khong co mon hoc phu hop)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displayCourseSearchResults(const string& keyword, const DoubleLinkedList<Course>& list) {
    cout << "  KET QUA TIM KIEM MON HOC (Tu khoa: \"" << keyword << "\")\n";
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Khong tim thay mon hoc nao phu hop)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displaySections(const DoubleLinkedList<ClassSection>& list) {
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Danh sach lop hoc phan trong)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displaySectionSearchResults(const string& keyword, const DoubleLinkedList<ClassSection>& list) {
    cout << "  KET QUA TIM KIEM LOP HOC PHAN (Tu khoa: \"" << keyword << "\")\n";
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Khong tim thay lop hoc phan nao phu hop)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displayGroupSectionList(const DoubleLinkedList<SectionDetailInfo>& list, bool numbered) {
    int idx = 0;
    auto cur = list.getHead();
    while (cur != nullptr) {
        idx++;
        const SectionDetailInfo& s = cur->data;
        if (numbered) {
            cout << "  [" << idx << "] ";
        } else {
            cout << "  ";
        }
        cout << s.maLopHP
             << " | Mon: " << s.tenMon
             << " | GV: "  << s.giangVien
             << " | Thu "  << s.thu
             << " | Tiet " << s.tietBatDau << "-" << s.tietKetThuc;
        if (numbered) {
            cout << " | Phong: " << s.phong;
        }
        cout << " | SS: "  << s.soLuongDaDangKy << "/" << s.siSoToiDa
             << "\n";
        cur = cur->next;
    }
    if (numbered && idx == 0) {
        cout << "  (Khong co lop hoc phan nao trong nhom nay)\n";
    }
}

inline void displayStudentsInSection(const ClassSection& sec,
                                     const DoubleLinkedList<SectionStudentInfo>& students) {
    cout << "  DANH SACH SINH VIEN DANG KY LOP HOC PHAN: " << sec.getMaLopHP() << "\n";
    cout << "  Mon: " << sec.getMaMon()
         << " | Nhom: " << (sec.getNhomLopHP().empty() ? "(Chua gan nhom)" : sec.getNhomLopHP())
         << " | Thu: "  << sec.getThu()
         << " | Tiet: " << sec.getTietBatDau() << "-" << sec.getTietKetThuc() << "\n";
    cout << "  Si so hien tai: " << sec.getSoLuongDaDangKy() << "/" << sec.getSiSoToiDa() << "\n";

    int count = 0;
    auto cur = students.getHead();
    while (cur != nullptr) {
        count++;
        cout << left << setw(4) << count
             << "MSSV: " << setw(12) << cur->data.maSV
             << " | Ho ten: "  << setw(22) << cur->data.hoTen
             << " | Lop: "     << setw(10) << cur->data.lop
             << " | Ngay DK: " << cur->data.ngayDangKy << "\n";
        cur = cur->next;
    }
    if (count == 0) cout << "(Chua co sinh vien nao dang ky vao lop hoc phan nay)\n";
}

inline void displayRegistrations(const DoubleLinkedList<Registration>& list) {
    auto cur = list.getHead();
    if (cur == nullptr) {
        cout << "(Chua co phieu dang ky nao)\n";
        return;
    }
    while (cur != nullptr) {
        cout << cur->data;
        cur = cur->next;
    }
}

inline void displayStudentRegistrations(const string& maSV,
                                        const DoubleLinkedList<StudentRegistrationDetail>& details) {
    cout << "  DANH SACH LOP DA DANG KY - SV: " << maSV << "\n";
    int count = 0;
    auto cur = details.getHead();
    while (cur != nullptr) {
        count++;
        const StudentRegistrationDetail& d = cur->data;
        cout << "[" << count << "] Lop HP: " << d.maLopHP
             << " | Ngay DK: " << d.ngayDangKy << "\n";
        if (!d.tenMon.empty()) {
            cout << "    Mon: " << d.tenMon
                 << " | Nhom: " << (d.nhomLopHP.empty() ? "(Khong)" : d.nhomLopHP)
                 << " | Thu "   << d.thu
                 << " | Tiet "  << d.tietBatDau << "-" << d.tietKetThuc
                 << " | GV: "   << d.giangVien << "\n";
        }
        cur = cur->next;
    }
    if (count == 0) cout << "(Chua dang ky lop hoc phan nao)\n";
}

inline void displayCreditSummary(const string& maSV, int totalCredits,
                                 const DoubleLinkedList<CreditSummaryItem>& items) {
    cout << "  TONG KET TIN CHI DA DANG KY - MSSV: " << maSV << "\n";
    int count = 0;
    auto cur = items.getHead();
    while (cur != nullptr) {
        count++;
        cout << "[" << count << "] Lop HP: " << setw(22) << left << cur->data.maLopHP
             << " | Mon: " << setw(20) << cur->data.tenMon
             << " | So tin chi: " << cur->data.soTinChi << "\n";
        cur = cur->next;
    }
    cout << ">> TONG SO TIN CHI DA DANG KY: " << totalCredits
         << " tin chi (" << count << " lop hoc phan)\n";
}

inline void displayStudentTimetable(const string& maSV, int currentSemester,
                                    const DoubleLinkedList<TimetableEntry>& entries) {
    cout << "                   THOI KHOA BIEU SINH VIEN: " << maSV << "\n";
    cout << "                   (Hoc ky he thong: Hoc ky " << currentSemester << ")\n";
    const string days[]     = {"2", "3", "4", "5", "6", "7", "CN"};
    const string dayNames[] = {"THU HAI (Thu 2)", "THU BA (Thu 3)", "THU TU (Thu 4)",
                               "THU NAM (Thu 5)", "THU SAU (Thu 6)", "THU BAY (Thu 7)", "CHU NHAT"};
    int totalClasses = 0;
    for (int d = 0; d < 7; d++) {
        bool hasClass = false;
        auto cur = entries.getHead();
        while (cur != nullptr) {
            if (cur->data.thu == days[d]) {
                if (!hasClass) { cout << "\n>>> " << dayNames[d] << ":\n"; hasClass = true; }
                cout << "  - Tiet " << setw(2) << right << cur->data.tietBatDau
                     << " -> " << setw(2) << left << cur->data.tietKetThuc
                     << " | Lop HP: " << setw(22) << cur->data.maLopHP
                     << " | Mon: "    << setw(20) << cur->data.tenMon
                     << " | Phong: "  << setw(8)  << cur->data.phong
                     << " | GV: "     << cur->data.giangVien << "\n";
                totalClasses++;
            }
            cur = cur->next;
        }
    }
    if (totalClasses == 0)
        cout << "\n(Ban chua dang ky lop hoc phan nao trong thoi khoa bieu)\n";
}

inline void displayTimeConflictResult(const ClassSection& target,
                                      const DoubleLinkedList<ClassSection>& conflicts) {
    cout << "\n--- KIEM TRA TRUNG LICH HOC ---\n";
    cout << "Lop can kiem tra: " << target.getMaLopHP()
         << " | Thu " << target.getThu()
         << " | Tiet " << target.getTietBatDau() << "-" << target.getTietKetThuc()
         << " | Phong: " << target.getPhong() << "\n";
    if (conflicts.empty()) {
        cout << ">> [HOP LE] Lop hoc phan KHONG BI TRUNG LICH voi bat ky lop nao ban da dang ky!\n";
        return;
    }
    auto cur = conflicts.getHead();
    while (cur != nullptr) {
        cout << ">> [CANH BAO] TRUNG LICH voi lop da dang ky: "
             << cur->data.getMaLopHP()
             << " (Mon: " << cur->data.getMaMon()
             << ", Thu " << cur->data.getThu()
             << ", Tiet " << cur->data.getTietBatDau() << "-" << cur->data.getTietKetThuc()
             << ", Phong: " << cur->data.getPhong() << ")\n";
        cur = cur->next;
    }
}

inline void displaySemesterRegistrationStatus(const SemesterProgressSummary& summary,
                                              const DoubleLinkedList<CourseProgressItem>& items) {
    cout << "  RANG BUOC & TIEN DO DANG KY TAT CA MON HOC - HOC KY " << summary.currentSemester << "\n";
    cout << "  Sinh vien: " << summary.hoTen
         << " (MSSV: " << summary.maSV
         << " | Lop: "  << summary.lop << ")\n";

    if (summary.totalRequiredCourses == 0) {
        cout << "(Khong tim thay mon hoc nao duoc mo cho chuong trinh cua ban trong hoc ky "
             << summary.currentSemester << ")\n";
        return;
    }

    int idx = 0;
    auto cur = items.getHead();
    while (cur != nullptr) {
        idx++;
        const CourseProgressItem& item = cur->data;
        cout << "[" << idx << "] [" << item.maMon << "] "
             << setw(26) << left << item.tenMon << " (" << item.soTinChi << " TC) -> ";
        if (item.daDangKy) {
            cout << "[DA DANG KY: " << item.maLopHP;
            if (!item.thu.empty()) {
                cout << " | Thu " << item.thu << " Tiet " << item.tietBatDau << "-" << item.tietKetThuc;
            }
            cout << "]\n";
        } else {
            cout << "[CHUA DANG KY - CAN BO SUNG]\n";
        }
        cur = cur->next;
    }

    cout << "Tong so mon can hoc trong ky: " << summary.totalRequiredCourses << " mon ("
         << summary.totalCredits << " tin chi)\n";
    cout << "So mon da dang ky:           " << summary.registeredCourses << "/"
         << summary.totalRequiredCourses << " mon (" << summary.registeredCredits << "/"
         << summary.totalCredits << " tin chi)\n";

    if (summary.registeredCourses == summary.totalRequiredCourses) {
        cout << ">> [TIEN DO HOC TAP]: HOAN THANH! Ban da dang ky DAY DU tat ca cac mon trong Hoc ky "
             << summary.currentSemester << "!\n";
    } else {
        int missing = summary.totalRequiredCourses - summary.registeredCourses;
        cout << ">> [CANH BAO TIEN DO]: CHUA HOAN THANH! Ban con thieu " << missing
             << " mon trong Hoc ky " << summary.currentSemester << ".\n";
        cout << ">> Cac mon con thieu:\n";
        cur = items.getHead();
        while (cur != nullptr) {
            if (!cur->data.daDangKy) {
                cout << "   - [" << cur->data.maMon << "] "
                     << cur->data.tenMon
                     << " (" << cur->data.soTinChi << " TC)\n";
            }
            cur = cur->next;
        }
        cout << ">> Khuyen nghi: Ban nen dang ky bo sung day du cac mon de dam bao dung tien do chuong trinh dao tao!\n";
    }
}

inline void printSectionChoiceHeader(int index, const ClassSection& sec, const Course* course) {
    cout << "[" << index << "] " << setw(22) << left << sec.getMaLopHP()
         << " | Mon: " << setw(20) << left << (course ? course->getTenMon() : sec.getMaMon())
         << " (" << (course ? course->getSoTinChi() : 0) << " TC)\n"
         << "      Thu " << sec.getThu()
         << " | Tiet " << sec.getTietBatDau() << "-" << sec.getTietKetThuc()
         << " | Phong: " << setw(6) << sec.getPhong()
         << " | GV: ";
}

inline bool readOptionalIntField(const string& label, int currentVal, const string& fieldName, int& outVal) {
    cout << label << " [" << currentVal << "]: ";
    string val;
    getline(cin, val);
    if (val.empty()) return false;
    try {
        outVal = stoi(val);
        return true;
    } catch (const invalid_argument&) {
        cout << ">> Gia tri " << fieldName << " khong hop le, giu nguyen gia tri cu.\n";
    } catch (const out_of_range&) {
        cout << ">> Gia tri " << fieldName << " vuot gioi han, giu nguyen gia tri cu.\n";
    }
    return false;
}

inline void editStudentInteractive(Student& sv) {
    cout << "\n--- THONG TIN SINH VIEN HIEN TAI ---\n";
    cout << sv;
    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
    string val;
    cout << "Ho ten moi [" << sv.getHoTen() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setHoTen(val);

    cout << "So dien thoai moi [" << sv.getSoDienThoai() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setSoDienThoai(val);

    cout << "Email moi [" << sv.getEmail() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setEmail(val);

    cout << "Khoa moi [" << sv.getKhoa() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setKhoa(val);

    cout << "Khoa hoc moi [" << sv.getKhoaHoc() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setKhoaHoc(val);

    cout << "Lop moi [" << sv.getLop() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setLop(val);

    cout << "Nganh hoc moi [" << sv.getNganhHoc() << "]: ";
    getline(cin, val);
    if (!val.empty()) sv.setNganhHoc(val);
}

inline void editCourseInteractive(Course& c) {
    cout << "\n--- THONG TIN MON HOC HIEN TAI ---\n";
    cout << c;
    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
    string val;
    cout << "Ten mon moi [" << c.getTenMon() << "]: ";
    getline(cin, val);
    if (!val.empty()) c.setTenMon(val);

    int tc = 0;
    if (readOptionalIntField("So tin chi moi", c.getSoTinChi(), "so tin chi", tc) && tc > 0) {
        c.setSoTinChi(tc);
    }

    cout << "Khoa quan ly moi [" << c.getKhoa() << "]: ";
    getline(cin, val);
    if (!val.empty()) c.setKhoa(val);

    cout << "Khoa hoc ap dung moi [" << c.getKhoaHoc() << "]: ";
    getline(cin, val);
    if (!val.empty()) c.setKhoaHoc(val);

    cout << "Nganh hoc ap dung moi [" << c.getNganhHoc() << "]: ";
    getline(cin, val);
    if (!val.empty()) c.setNganhHoc(val);

    int hk = 0;
    if (readOptionalIntField("Hoc ky ap dung moi ", c.getHocKy(), "hoc ky", hk) && (hk == 1 || hk == 2)) {
        c.setHocKy(hk);
    }
}

inline void editClassSectionInteractive(ClassSection& sec) {
    cout << "\n--- THONG TIN LOP HOC PHAN HIEN TAI ---\n";
    cout << sec;
    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de giu nguyen) ---\n";
    string val;
    cout << "Nhom lop HP moi [" << sec.getNhomLopHP() << "]: ";
    getline(cin, val);
    if (!val.empty()) sec.setNhomLopHP(val);

    cout << "Giang vien moi [" << sec.getGiangVien() << "]: ";
    getline(cin, val);
    if (!val.empty()) sec.setGiangVien(val);

    cout << "Thu hoc moi [" << sec.getThu() << "]: ";
    getline(cin, val);
    if (!val.empty()) sec.setThu(val);

    int t = 0;
    if (readOptionalIntField("Tiet bat dau moi", sec.getTietBatDau(), "tiet bat dau", t) && t > 0) {
        sec.setTietBatDau(t);
    }

    if (readOptionalIntField("Tiet ket thuc moi", sec.getTietKetThuc(), "tiet ket thuc", t) &&
        t > sec.getTietBatDau()) {
        sec.setTietKetThuc(t);
    }

    cout << "Phong hoc moi [" << sec.getPhong() << "]: ";
    getline(cin, val);
    if (!val.empty()) sec.setPhong(val);

    int ss = 0;
    if (readOptionalIntField("Si so toi da moi", sec.getSiSoToiDa(), "si so", ss) && ss > 0) {
        sec.setSiSoToiDa(ss);
    }

    int hk = 0;
    if (readOptionalIntField("Hoc ky mo lop moi", sec.getHocKy(), "hoc ky", hk) && (hk == 1 || hk == 2)) {
        sec.setHocKy(hk);
    }
}

} // namespace ConsoleView

#endif // CONSOLE_VIEW_H
