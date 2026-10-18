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
};
Registration::Registration(string maDK, string maSinhVien, string maLop, string ngayDK)
    : maDangKy(maDK), maSV(maSinhVien), maLopHP(maLop), ngayDangKy(ngayDK) {}

string Registration::getMaDangKy() const { return maDangKy; }
string Registration::getMaSV() const { return maSV; }
string Registration::getMaLopHP() const { return maLopHP; }
string Registration::getNgayDangKy() const { return ngayDangKy; }

void Registration::xuat() const {
    cout << "Phieu DK: " << maDangKy << " | Ma SV: " << maSV << " | Lop HP: " << maLopHP << " | Ngay: " << ngayDangKy << '\n';
}

#endif
