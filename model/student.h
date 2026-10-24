#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include <iostream>
#include <string>
using namespace std;

class Student : public Person {
private:
    string lop;
    string nganhHoc;
    string khoa;      // Khoa cua sinh vien (vi du: CNTT, Dien, Co khi...)
    string khoaHoc;   // Khoa hoc cua sinh vien (vi du: K22, K23...)

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh, string k = "", string kh = "");

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;
    string getKhoa() const;
    string getKhoaHoc() const;

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);
    void setKhoa(string k);
    void setKhoaHoc(string kh);

    void nhap() override;
    void xuat() const override;
    bool operator==(const Student& other) const;
};

inline Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
    khoa = "";
    khoaHoc = "";
}

inline Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh, string k, string kh)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    nganhHoc = nganh;
    khoa = k;
    khoaHoc = kh;
}

inline string Student::getMaSV() const { return maNguoi; }
inline string Student::getLop() const { return lop; }
inline string Student::getNganhHoc() const { return nganhHoc; }
inline string Student::getKhoa() const { return khoa; }
inline string Student::getKhoaHoc() const { return khoaHoc; }

inline void Student::setMaSV(string maSV) { maNguoi = maSV; }
inline void Student::setLop(string lopSV) { lop = lopSV; }
inline void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }
inline void Student::setKhoa(string k) { khoa = k; }
inline void Student::setKhoaHoc(string kh) { khoaHoc = kh; }

inline void Student::nhap() {
    cout << "Ma sinh vien: ";
    getline(cin >> ws, maNguoi);
    cout << "Ho ten: ";
    getline(cin, hoTen);
    cout << "So dien thoai: ";
    getline(cin, soDienThoai);
    cout << "Email: ";
    getline(cin, email);
    cout << "Khoa (vi du: CNTT, Dien, Co khi): ";
    getline(cin, khoa);
    cout << "Khoa hoc (vi du: K22, K23): ";
    getline(cin, khoaHoc);
    cout << "Lop: ";
    getline(cin, lop);
    cout << "Nganh hoc: ";
    getline(cin, nganhHoc);
}

inline void Student::xuat() const {
    cout << "Ma sinh vien: " << maNguoi << '\n';
    cout << "Ho ten: " << hoTen << '\n';
    cout << "So dien thoai: " << soDienThoai << '\n';
    cout << "Email: " << email << '\n';
    cout << "Khoa: " << (khoa.empty() ? "Chua cap nhat" : khoa) << '\n';
    cout << "Khoa hoc: " << (khoaHoc.empty() ? "Chua cap nhat" : khoaHoc) << '\n';
    cout << "Lop: " << lop << '\n';
    cout << "Nganh hoc: " << nganhHoc << '\n';
}

inline bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif