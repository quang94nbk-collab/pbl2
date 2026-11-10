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
    int hocKy;
    string nhomLopHP;

public:
    ClassSection();
    ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                 int tietDau, int tietCuoi, string phongHoc, int toiDa,
                 int hk = 1, string nhom = "");

    string getMaLopHP() const;
    string getMaMon() const;
    string getGiangVien() const;
    string getThu() const;
    int getTietBatDau() const;
    int getTietKetThuc() const;
    string getPhong() const;
    int getSiSoToiDa() const;
    int getSoLuongDaDangKy() const;
    int getHocKy() const;
    string getNhomLopHP() const;

    void setMaLopHP(string ma);
    void setMaMon(string ma);
    void setGiangVien(string gv);
    void setThu(string t);
    void setTietBatDau(int tiet);
    void setTietKetThuc(int tiet);
    void setPhong(string ph);
    void setSiSoToiDa(int siso);
    void setSoLuongDaDangKy(int soLuong);
    bool setHocKy(int hk);
    void setNhomLopHP(string nhom);
    bool conCho() const;
    bool tangSoLuong();
    void giamSoLuong();
    bool trungLichVoi(const ClassSection& other) const;

    void nhap();
    void xuat() const;
    bool operator==(const ClassSection& other) const;

    friend ostream& operator<<(ostream& os, const ClassSection& cs);
    friend istream& operator>>(istream& is, ClassSection& cs);
};

ClassSection::ClassSection() {
    maLopHP = "";
    maMon = "";
    giangVien = "";
    thu = "";
    tietBatDau = 0;
    tietKetThuc = 0;
    phong = "";
    siSoToiDa = 50;
    soLuongDaDangKy = 0;
    hocKy = 1;
    nhomLopHP = "";
}

ClassSection::ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                                  int tietDau, int tietCuoi, string phongHoc, int toiDa,
                                  int hk, string nhom) {
    maLopHP = maLop;
    maMon = maMonHoc;
    giangVien = gv;
    thu = ngayHoc;
    tietBatDau = tietDau;
    tietKetThuc = tietCuoi;
    phong = phongHoc;
    siSoToiDa = toiDa;
    soLuongDaDangKy = 0;
    hocKy = hk;
    nhomLopHP = nhom;
}

string ClassSection::getMaLopHP() const { return maLopHP; }
string ClassSection::getMaMon() const { return maMon; }
string ClassSection::getGiangVien() const { return giangVien; }
string ClassSection::getThu() const { return thu; }
int ClassSection::getTietBatDau() const { return tietBatDau; }
int ClassSection::getTietKetThuc() const { return tietKetThuc; }
string ClassSection::getPhong() const { return phong; }
int ClassSection::getSiSoToiDa() const { return siSoToiDa; }
int ClassSection::getSoLuongDaDangKy() const { return soLuongDaDangKy; }
int ClassSection::getHocKy() const { return hocKy; }
string ClassSection::getNhomLopHP() const { return nhomLopHP; }

void ClassSection::setMaLopHP(string ma) { maLopHP = ma; }
void ClassSection::setMaMon(string ma) { maMon = ma; }
void ClassSection::setGiangVien(string gv) { giangVien = gv; }
void ClassSection::setThu(string t) { thu = t; }
void ClassSection::setTietBatDau(int tiet) { tietBatDau = tiet; }
void ClassSection::setTietKetThuc(int tiet) { tietKetThuc = tiet; }
void ClassSection::setPhong(string ph) { phong = ph; }
void ClassSection::setSiSoToiDa(int siso) { if (siso > 0) siSoToiDa = siso; }

void ClassSection::setSoLuongDaDangKy(int soLuong) {
    if (soLuong >= 0) {
        soLuongDaDangKy = soLuong;
    }
}

bool ClassSection::setHocKy(int hk) {
    if (hk == 1 || hk == 2) {
        hocKy = hk;
        return true;
    }
    return false;
}

void ClassSection::setNhomLopHP(string nhom) { nhomLopHP = nhom; }

bool ClassSection::conCho() const {
    return soLuongDaDangKy < siSoToiDa;
}

bool ClassSection::tangSoLuong() {
    if (!conCho()) {
        return false;
    }
    soLuongDaDangKy++;
    return true;
}

void ClassSection::giamSoLuong() {
    if (soLuongDaDangKy > 0) {
        soLuongDaDangKy--;
    }
}

bool ClassSection::trungLichVoi(const ClassSection& other) const {
    if (thu != other.thu) {
        return false;
    }
    return tietBatDau < other.tietKetThuc &&
           other.tietBatDau < tietKetThuc;
}

bool ClassSection::operator==(const ClassSection& other) const {
    return maLopHP == other.maLopHP;
}

ostream& operator<<(ostream& os, const ClassSection& cs) {
    os << "Ma lop hoc phan: " << cs.maLopHP << '\n';
    os << "Ma mon hoc: " << cs.maMon << '\n';
    os << "Nhom lop HP: " << (cs.nhomLopHP.empty() ? "(Chua gan nhom)" : cs.nhomLopHP) << '\n';
    os << "Giang vien: " << cs.giangVien << '\n';
    os << "Thu hoc: " << cs.thu << '\n';
    os << "Tiet: " << cs.tietBatDau << " - " << cs.tietKetThuc << '\n';
    os << "Phong: " << cs.phong << '\n';
    os << "Si so: " << cs.soLuongDaDangKy << "/" << cs.siSoToiDa << '\n';
    os << "Hoc ky: " << cs.hocKy << '\n';
    return os;
}

istream& operator>>(istream& is, ClassSection& cs) {
    cout << "Ma lop hoc phan: ";
    getline(is >> ws, cs.maLopHP);
    cout << "Ma mon hoc: ";
    getline(is, cs.maMon);
    cout << "Nhom lop hoc phan : ";
    getline(is, cs.nhomLopHP);
    cout << "Giang vien: ";
    getline(is, cs.giangVien);
    cout << "Thu hoc : ";
    getline(is, cs.thu);
    cout << "Tiet bat dau: ";
    is >> cs.tietBatDau;
    cout << "Tiet ket thuc : ";
    is >> cs.tietKetThuc;
    is.ignore(10000, '\n');
    cout << "Phong hoc: ";
    getline(is, cs.phong);
    cout << "Si so toi da: ";
    is >> cs.siSoToiDa;
    is.ignore(10000, '\n');

    int hk = 1;
    do {
        cout << "Hoc ky mo lop: ";
        is >> hk;
        if (hk != 1 && hk != 2) {
            cout << "Vui long chon 1 hoac 2.\n";
        }
    } while (hk != 1 && hk != 2);
    is.ignore(10000, '\n');
    cs.hocKy = hk;

    cs.soLuongDaDangKy = 0;
    return is;
}

#endif
