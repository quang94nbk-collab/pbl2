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

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh);

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);

    void nhap() override;
    void xuat() const override;
    bool operator==(const Student& other) const;
};
Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
}

Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    nganhHoc = nganh;
}

string Student::getMaSV() const { return maNguoi; }
string Student::getLop() const { return lop; }
string Student::getNganhHoc() const { return nganhHoc; }

void Student::setMaSV(string maSV) { maNguoi = maSV; }
void Student::setLop(string lopSV) { lop = lopSV; }
void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }

void Student::nhap() {
    cout << "Ma sinh vien: ";
    getline(cin >> ws, maNguoi);
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

void Student::xuat() const {
    cout << "Ma sinh vien: " << maNguoi << '\n';
    cout << "Ho ten: " << hoTen << '\n';
    cout << "So dien thoai: " << soDienThoai << '\n';
    cout << "Email: " << email << '\n';
    cout << "Lop: " << lop << '\n';
    cout << "Nganh hoc: " << nganhHoc << '\n';
}

bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif