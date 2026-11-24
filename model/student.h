#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include "../DSA/DoublyLinkedList.h"
#include <string>

using namespace std;

class Registration;

// ============================================================
//  Cac ham tien ich giai ma thong tin tu Ma Sinh Vien
//  Quy uoc MSSV 9 chu so: [3 so Ma Khoa] + [2 so Khoa hoc] + [4 so Thu tu]
//  - 3 so dau: Ma Khoa (101: CoKhi, 102: CNTT, 103: Dien, 105: DTVT, 106: Hoa, 107: XayDung)
//  - 2 so sau: Khoa hoc (23 -> K23, 22 -> K22)
//  - 4 so cuoi: 4 chu so bat ky (VD: 0001, 0169)
// ============================================================
string getKhoaFromMaSV(const string& maSV);
string getKhoaHocFromMaSV(const string& maSV);
string getTenKhoaDayDu(const string& maKhoaOrNganh);
bool isValidMaSV(const string& maSV);

// ============================================================
//  Student - Lop dai dien cho Sinh vien
//  Quan he:
//    - Student IS-A Person (Inheritance: Ke thua)
//    - Student HAS-MANY Registration (Huong 1: Quan he 2 chieu)
// ============================================================
class Student : public Person {
private:
    string lop;
    string nganhHoc;
    string khoa;      
    string khoaHoc;
    int soKiDaHoc;

    // Huong 1: Danh sach con tro Registration ma sinh vien da dang ky
    DoubleLinkedList<Registration*> dsDangKy;

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh, string k = "", string kh = "", int soKi = 0);
    Student(const Student& other);
    Student& operator=(const Student& other);
    ~Student();

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;
    string getKhoa() const;
    string getKhoaHoc() const;
    int getSoKiDaHoc() const;
    int getHocKyHienTai() const;
    int getHocKyHienTai(int systemSemester) const;
    int getHocKyTienTrinh(int systemSemester = 1) const;

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);
    void setKhoa(string k);
    void setKhoaHoc(string kh);
    void setSoKiDaHoc(int soKi);

    // Quan ly danh sach Registration (Huong 1.2)
    void themDangKy(Registration* reg);
    bool xoaDangKy(Registration* reg);
    const DoubleLinkedList<Registration*>& getDanhSachDangKy() const;
    int getSoMonDangKy() const;
    void clearDangKy();

    bool operator==(const Student& other) const;
};

#endif // STUDENT_H
