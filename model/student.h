#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include <cctype>
#include <iostream>
#include <string>
using namespace std;

class Student : public Person {
private:
    string lop;
    string nganhHoc;

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh);

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;

    string getMaKhoa() const;
    string getTenKhoa() const;
    static bool isValidMaSV(const string& ma);

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);

    void nhap() override;
    void xuat() const override;
    bool operator==(const Student& other) const;
};

inline Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
}

inline Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    nganhHoc = nganh;
}

inline string Student::getMaSV() const { return maNguoi; }
inline string Student::getLop() const { return lop; }
inline string Student::getNganhHoc() const { return nganhHoc; }

inline string Student::getMaKhoa() const {
    if (maNguoi.length() >= 3) {
        return maNguoi.substr(0, 3);
    }
    return "";
}

inline string Student::getTenKhoa() const {
    string mk = getMaKhoa();
    if (mk == "100") return "Khoa Co ban / Dai cuong";
    if (mk == "101") return "Khoa Co khi";
    if (mk == "102") return "Khoa Cong nghe Thong tin";
    if (mk == "103") return "Khoa Dien - Dien tu";
    return "Khong xac dinh";
}

inline bool Student::isValidMaSV(const string& ma) {
    if (ma.length() != 8) return false;
    for (char c : ma) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    string prefix = ma.substr(0, 3);
    return (prefix >= "100" && prefix <= "103");
}

inline void Student::setMaSV(string maSV) { maNguoi = maSV; }
inline void Student::setLop(string lopSV) { lop = lopSV; }
inline void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }

inline void Student::nhap() {
    do {
        cout << "Ma sinh vien (8 chu so, 3 so dau 100-103): ";
        getline(cin >> ws, maNguoi);
        if (!isValidMaSV(maNguoi)) {
            cout << "Ma sinh vien khong hop le (phai gom 8 chu so, 3 so dau tu 100 den 103). Vui long nhap lai!\n";
        }
    } while (!isValidMaSV(maNguoi));

    cout << "Khoa: " << getTenKhoa() << '\n';
    cout << "Ho ten: ";
    getline(cin, hoTen);
    cout << "So dien thoai: ";
    getline(cin, soDienThoai);
    cout << "Email: ";
    getline(cin, email);
    cout << "Lop: ";
    getline(cin, lop);
    cout << "Nganh hoc: ";
    getline(cin, nganhHoc);
}

inline void Student::xuat() const {
    cout << "Ma sinh vien: " << maNguoi << " [" << getTenKhoa() << "]\n";
    cout << "Ho ten: " << hoTen << '\n';
    cout << "So dien thoai: " << soDienThoai << '\n';
    cout << "Email: " << email << '\n';
    cout << "Lop: " << lop << '\n';
    cout << "Nganh hoc: " << nganhHoc << '\n';
}

inline bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif