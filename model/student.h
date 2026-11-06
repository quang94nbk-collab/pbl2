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
    string khoa;      
    string khoaHoc;   

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

Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
    khoa = "";
    khoaHoc = "";
}

Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh, string k, string kh)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    nganhHoc = nganh;
    khoa = k;
    khoaHoc = kh;
}

string Student::getMaSV() const { return maNguoi; }
string Student::getLop() const { return lop; }
string Student::getNganhHoc() const { return nganhHoc; }
string Student::getKhoa() const { return khoa; }
string Student::getKhoaHoc() const { return khoaHoc; }

void Student::setMaSV(string maSV) { maNguoi = maSV; }
void Student::setLop(string lopSV) { lop = lopSV; }
void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }
void Student::setKhoa(string k) { khoa = k; }
void Student::setKhoaHoc(string kh) { khoaHoc = kh; }

void Student::nhap() {
    cout << "Ma sinh vien: ";
    getline(cin >> ws, maNguoi);
    cout << "Ho ten: ";
    getline(cin, hoTen);
    cout << "So dien thoai: ";
    getline(cin, soDienThoai);
    cout << "Email: ";
    getline(cin, email);
    cout << "Khoa: ";
    getline(cin, khoa);
    cout << "Khoa hoc: ";
    getline(cin, khoaHoc);
    cout << "Lop: ";
    getline(cin, lop);
    cout << "Nganh hoc: ";
    getline(cin, nganhHoc);
}

void Student::xuat() const {
    cout << "Ma sinh vien: " << maNguoi << '\n';
    cout << "Ho ten: " << hoTen << '\n';
    cout << "So dien thoai: " << soDienThoai << '\n';
    cout << "Email: " << email << '\n';
    cout << "Khoa: " << (khoa.empty() ? "Chua cap nhat" : khoa) << '\n';
    cout << "Khoa hoc: " << (khoaHoc.empty() ? "Chua cap nhat" : khoaHoc) << '\n';
    cout << "Lop: " << lop << '\n';
    cout << "Nganh hoc: " << nganhHoc << '\n';
}

bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif