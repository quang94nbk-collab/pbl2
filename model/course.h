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

inline void Course::nhap() {
    cout << "Ma mon hoc: ";
    getline(cin >> ws, maMon);
    cout << "Ten mon hoc: ";
    getline(cin, tenMon);

    int tinChi;
    do {
        cout << "So tin chi: ";
        cin >> tinChi;
        if (tinChi <= 0) {
            cout << "So tin chi phai lon hon 0. Nhap lai.\n";
        }
    } while (tinChi <= 0);
    cin.ignore(10000, '\n');
    soTinChi = tinChi;

    cout << "Khoa quan ly : ";
    getline(cin, khoa);
    cout << "Khoa hoc ap dung : ";
    getline(cin, khoaHoc);
    cout << "Nganh hoc : ";
    getline(cin, nganhHoc);

    int hk = 1;
    do {
        cout << "Hoc ky ap dung : ";
        cin >> hk;
        if (hk != 1 && hk != 2) {
            cout << "Vui long chon 1 hoac 2.\n";
        }
    } while (hk != 1 && hk != 2);
    cin.ignore(10000, '\n');
    hocKy = hk;
}

inline void Course::xuat() const {
    cout << "Ma mon hoc: " << maMon << '\n';
    cout << "Ten mon hoc: " << tenMon << '\n';
    cout << "So tin chi: " << soTinChi << '\n';
    cout << "Khoa: " << (khoa.empty() ? "Tat ca" : khoa) << '\n';
    cout << "Khoa hoc ap dung: " << (khoaHoc.empty() ? "Tat ca" : khoaHoc) << '\n';
    cout << "Nganh hoc ap dung: " << (nganhHoc.empty() ? "Tat ca" : nganhHoc) << '\n';
    cout << "Hoc ky: " << hocKy << '\n';
}

inline bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}

inline ostream& operator<<(ostream& os, const Course& c) {
    c.xuat();
    return os;
}

inline istream& operator>>(istream& is, Course& c) {
    c.nhap();
    return is;
}

#endif
