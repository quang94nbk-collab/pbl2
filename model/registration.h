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

inline Registration::Registration(string maDK, string maSinhVien, string maLop, string ngayDK)
    : maDangKy(maDK), maSV(maSinhVien), maLopHP(maLop), ngayDangKy(ngayDK) {}

inline string Registration::getMaDangKy() const { return maDangKy; }
inline string Registration::getMaSV() const { return maSV; }
inline string Registration::getMaLopHP() const { return maLopHP; }
inline string Registration::getNgayDangKy() const { return ngayDangKy; }

inline void Registration::xuat() const {
    cout << "Phieu DK: " << maDangKy << " | Ma SV: " << maSV << " | Lop HP: " << maLopHP << " | Ngay: " << ngayDangKy << '\n';
}

#endif
