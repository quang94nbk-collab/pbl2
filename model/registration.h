#ifndef reg_h
#define reg_h
#include<iostream>
#include <string>
using namespace std;

class Registration {
private:
    string maDangKy, maSV, maLopHP, ngayDangKy;
public:
    Registration(string maDK="", string maSinhVien="", string maLop="", string ngayDK="");
    
    string getMaDangKy() const;
    string getMaSV() const;
    string getMaLopHP() const;
    string getNgayDangKy() const;

    void xuat() const;

    friend ostream& operator<<(ostream& os, const Registration& r);
    friend istream& operator>>(istream& is, Registration& r);
};
Registration::Registration(string maDK, string maSinhVien, string maLop, string ngayDK)
    : maDangKy(maDK), maSV(maSinhVien), maLopHP(maLop), ngayDangKy(ngayDK) {}

string Registration::getMaDangKy() const { return maDangKy; }
string Registration::getMaSV() const { return maSV; }
string Registration::getMaLopHP() const { return maLopHP; }
string Registration::getNgayDangKy() const { return ngayDangKy; }

ostream& operator<<(ostream& os, const Registration& r) {
    os << "Phieu DK: " << r.maDangKy << " | Ma SV: " << r.maSV
       << " | Lop HP: " << r.maLopHP << " | Ngay: " << r.ngayDangKy << '\n';
    return os;
}
istream& operator>>(istream& is, Registration& r) {
    cout << "Ma dang ky: ";
    getline(is >> ws, r.maDangKy);
    cout << "Ma sinh vien: ";
    getline(is, r.maSV);
    cout << "Ma lop hoc phan: ";
    getline(is, r.maLopHP);
    cout << "Ngay dang ky: ";
    getline(is, r.ngayDangKy);
    return is;
}

#endif
