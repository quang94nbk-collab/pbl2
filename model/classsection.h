#ifndef clss_h
#define clss_h
#include<iostream>
#include <string>
using namespace std;

class ClassSection {
private:
    string maLopHP;
    string maMon;
    string giangVien;
    string thu;
    int tietBatDau;
    int tietKetThuc;
    string phong;
    int siSoToiDa;
    int soLuongDaDangKy;

public:
    ClassSection();
    ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                 int tietDau, int tietCuoi, string phongHoc, int toiDa);

    string getMaLopHP() const;
    string getMaMon() const;
    string getGiangVien() const;
    string getThu() const;
    int getTietBatDau() const;
    int getTietKetThuc() const;
    string getPhong() const;
    int getSiSoToiDa() const;
    int getSoLuongDaDangKy() const;

    void setSoLuongDaDangKy(int soLuong);
    bool conCho() const;
    bool tangSoLuong();
    void giamSoLuong();
    bool trungLichVoi(const ClassSection& other) const;

    void nhap();
    void xuat() const;
    bool operator==(const ClassSection& other) const;
};

inline ClassSection::ClassSection() {
    maLopHP = "";
    maMon = "";
    giangVien = "";
    thu = "";
    tietBatDau = 0;
    tietKetThuc = 0;
    phong = "";
    siSoToiDa = 50;
    soLuongDaDangKy = 0;
}

inline ClassSection::ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                           int tietDau, int tietCuoi, string phongHoc, int toiDa) {
    maLopHP = maLop;
    maMon = maMonHoc;
    giangVien = gv;
    thu = ngayHoc;
    tietBatDau = tietDau;
    tietKetThuc = tietCuoi;
    phong = phongHoc;
    siSoToiDa = toiDa;
    soLuongDaDangKy = 0;
}

inline string ClassSection::getMaLopHP() const { return maLopHP; }
inline string ClassSection::getMaMon() const { return maMon; }
inline string ClassSection::getGiangVien() const { return giangVien; }
inline string ClassSection::getThu() const { return thu; }
inline int ClassSection::getTietBatDau() const { return tietBatDau; }
inline int ClassSection::getTietKetThuc() const { return tietKetThuc; }
inline string ClassSection::getPhong() const { return phong; }
inline int ClassSection::getSiSoToiDa() const { return siSoToiDa; }
inline int ClassSection::getSoLuongDaDangKy() const { return soLuongDaDangKy; }

inline void ClassSection::setSoLuongDaDangKy(int soLuong) {
    if (soLuong >= 0) {
        soLuongDaDangKy = soLuong;
    }
}

inline bool ClassSection::conCho() const {
    return soLuongDaDangKy < siSoToiDa;
}

inline bool ClassSection::tangSoLuong() {
    if (!conCho()) {
        return false;
    }
    soLuongDaDangKy++;
    return true;
}

inline void ClassSection::giamSoLuong() {
    if (soLuongDaDangKy > 0) {
        soLuongDaDangKy--;
    }
}

inline bool ClassSection::trungLichVoi(const ClassSection& other) const {
    if (thu != other.thu) {
        return false;
    }
    return tietBatDau < other.tietKetThuc &&
           other.tietBatDau < tietKetThuc;
}

inline void ClassSection::nhap() {
    cout << "Ma lop hoc phan: ";
    getline(cin >> ws, maLopHP);
    cout << "Ma mon hoc: ";
    getline(cin, maMon);
    cout << "Giang vien: ";
    getline(cin, giangVien);
    cout << "Thu hoc (vi du: 2, 3, 4...): ";
    getline(cin, thu);
    cout << "Tiet bat dau: ";
    cin >> tietBatDau;
    cout << "Tiet ket thuc (khong tinh tiet ket thuc): ";
    cin >> tietKetThuc;
    cin.ignore(10000, '\n');
    cout << "Phong hoc: ";
    getline(cin, phong);
    cout << "Si so toi da: ";
    cin >> siSoToiDa;
    cin.ignore(10000, '\n');
    soLuongDaDangKy = 0;
}

inline void ClassSection::xuat() const {
    cout << "Ma lop hoc phan: " << maLopHP << '\n';
    cout << "Ma mon hoc: " << maMon << '\n';
    cout << "Giang vien: " << giangVien << '\n';
    cout << "Thu hoc: " << thu << '\n';
    cout << "Tiet: " << tietBatDau << " - " << tietKetThuc << '\n';
    cout << "Phong: " << phong << '\n';
    cout << "Si so: " << soLuongDaDangKy << "/" << siSoToiDa << '\n';
}

inline bool ClassSection::operator==(const ClassSection& other) const {
    return maLopHP == other.maLopHP;
}

#endif
