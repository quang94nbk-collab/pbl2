#ifndef course_h
#define course_h
#include <iostream>
#include <string>
using namespace std;

class Course {
private:
    string maMon;
    string tenMon;
    int soTinChi;
    string khoa;      
    string khoaHoc;  
    string nganhHoc;  
    int hocKy;       

public:
    Course();
    Course(string ma, string ten, int tinChi, string k = "", string kh = "", string ng = "", int hk = 1);

    string getMaMon() const;
    string getTenMon() const;
    int getSoTinChi() const;
    string getKhoa() const;
    string getKhoaHoc() const;
    string getNganhHoc() const;
    int getHocKy() const;

    void setMaMon(string ma);
    void setTenMon(string ten);
    bool setSoTinChi(int tinChi);
    void setKhoa(string k);
    void setKhoaHoc(string kh);
    void setNganhHoc(string ng);
    bool setHocKy(int hk);

    void nhap();
    void xuat() const;
    bool operator==(const Course& other) const;

    friend ostream& operator<<(ostream& os, const Course& c);
    friend istream& operator>>(istream& is, Course& c);
};

Course::Course() {
    maMon = "";
    tenMon = "";
    soTinChi = 0;
    khoa = "";
    khoaHoc = "";
    nganhHoc = "";
    hocKy = 1;
}

Course::Course(string ma, string ten, int tinChi, string k, string kh, string ng, int hk) {
    maMon = ma;
    tenMon = ten;
    soTinChi = tinChi;
    khoa = k;
    khoaHoc = kh;
    nganhHoc = ng;
    hocKy = hk;
}

string Course::getMaMon() const { return maMon; }
string Course::getTenMon() const { return tenMon; }
int Course::getSoTinChi() const { return soTinChi; }
string Course::getKhoa() const { return khoa; }
string Course::getKhoaHoc() const { return khoaHoc; }
string Course::getNganhHoc() const { return nganhHoc; }
int Course::getHocKy() const { return hocKy; }

void Course::setMaMon(string ma) { maMon = ma; }
void Course::setTenMon(string ten) { tenMon = ten; }
void Course::setKhoa(string k) { khoa = k; }
void Course::setKhoaHoc(string kh) { khoaHoc = kh; }
void Course::setNganhHoc(string ng) { nganhHoc = ng; }

bool Course::setHocKy(int hk) {
    if (hk == 1 || hk == 2) {
        hocKy = hk;
        return true;
    }
    return false;
}

bool Course::setSoTinChi(int tinChi) {
    if (tinChi <= 0) {
        return false;
    }
    soTinChi = tinChi;
    return true;
}

bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}

ostream& operator<<(ostream& os, const Course& c) {
    os << "Ma mon hoc: " << c.maMon << '\n';
    os << "Ten mon hoc: " << c.tenMon << '\n';
    os << "So tin chi: " << c.soTinChi << '\n';
    os << "Khoa: " << (c.khoa.empty() ? "Tat ca" : c.khoa) << '\n';
    os << "Khoa hoc ap dung: " << (c.khoaHoc.empty() ? "Tat ca" : c.khoaHoc) << '\n';
    os << "Nganh hoc ap dung: " << (c.nganhHoc.empty() ? "Tat ca" : c.nganhHoc) << '\n';
    os << "Hoc ky: " << c.hocKy << '\n';
    return os;
}

istream& operator>>(istream& is, Course& c) {
    cout << "Ma mon hoc: ";
    getline(is >> ws, c.maMon);
    cout << "Ten mon hoc: ";
    getline(is, c.tenMon);

    int tinChi;
    do {
        cout << "So tin chi: ";
        is >> tinChi;
        if (tinChi <= 0) {
            cout << "So tin chi phai lon hon 0. Nhap lai.\n";
        }
    } while (tinChi <= 0);
    is.ignore(10000, '\n');
    c.soTinChi = tinChi;

    cout << "Khoa quan ly : ";
    getline(is, c.khoa);
    cout << "Khoa hoc ap dung : ";
    getline(is, c.khoaHoc);
    cout << "Nganh hoc : ";
    getline(is, c.nganhHoc);

    int hk = 1;
    do {
        cout << "Hoc ky ap dung : ";
        is >> hk;
        if (hk != 1 && hk != 2) {
            cout << "Vui long chon 1 hoac 2.\n";
        }
    } while (hk != 1 && hk != 2);
    is.ignore(10000, '\n');
    c.hocKy = hk;

    return is;
}

#endif
