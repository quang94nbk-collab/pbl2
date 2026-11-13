#ifndef DTOS_H
#define DTOS_H

#include <string>
using namespace std;

// ============================================================
//  DTOs - Cac cau truc du lieu truyen tai thong tin giua BLL va Presentation
//  (Khong chua logic nghiep vu hay thao tac Console I/O)
// ============================================================

struct SectionStudentInfo {
    string maSV;
    string hoTen;
    string lop;
    string ngayDangKy;
};

struct SectionDetailInfo {
    string maLopHP;
    string maMon;
    string tenMon;
    string nhomLopHP;
    string giangVien;
    string thu;
    int tietBatDau;
    int tietKetThuc;
    string phong;
    int siSoToiDa;
    int soLuongDaDangKy;
    int hocKy;
    int soTinChi;

    SectionDetailInfo()
        : tietBatDau(0), tietKetThuc(0), siSoToiDa(0),
          soLuongDaDangKy(0), hocKy(1), soTinChi(0) {}
};

struct StudentRegistrationDetail {
    string maDangKy;
    string maLopHP;
    string ngayDangKy;
    string tenMon;
    string nhomLopHP;
    string thu;
    int tietBatDau;
    int tietKetThuc;
    string giangVien;
    int hocKy;

    StudentRegistrationDetail() : tietBatDau(0), tietKetThuc(0), hocKy(1) {}
};

struct CreditSummaryItem {
    string maLopHP;
    string tenMon;
    int soTinChi;

    CreditSummaryItem() : soTinChi(0) {}
};

struct TimetableEntry {
    string thu;
    int tietBatDau;
    int tietKetThuc;
    string maLopHP;
    string tenMon;
    string phong;
    string giangVien;

    TimetableEntry() : tietBatDau(0), tietKetThuc(0) {}
};

struct CourseProgressItem {
    string maMon;
    string tenMon;
    int soTinChi;
    bool daDangKy;
    string maLopHP;
    string thu;
    int tietBatDau;
    int tietKetThuc;

    CourseProgressItem()
        : soTinChi(0), daDangKy(false), tietBatDau(0), tietKetThuc(0) {}
};

struct SemesterProgressSummary {
    string maSV;
    string hoTen;
    string lop;
    int currentSemester;
    int totalRequiredCourses;
    int registeredCourses;
    int totalCredits;
    int registeredCredits;

    SemesterProgressSummary()
        : currentSemester(1), totalRequiredCourses(0), registeredCourses(0),
          totalCredits(0), registeredCredits(0) {}
};

#endif // DTOS_H

