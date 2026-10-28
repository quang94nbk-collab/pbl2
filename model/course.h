#ifndef course_h
#define course_h
#include<iostream>
#include <string>
using namespace std;

class Course {
private:
    string maMon;
    string tenMon;
    int soTinChi;

public:
    Course();
    Course(string ma, string ten, int tinChi);

    string getMaMon() const;
    string getTenMon() const;
    int getSoTinChi() const;

    void setMaMon(string ma);
    void setTenMon(string ten);
    bool setSoTinChi(int tinChi);

    void nhap();
    void xuat() const;
    bool operator==(const Course& other) const;
};

inline Course::Course() {
    maMon = "";
    tenMon = "";
    soTinChi = 0;
}

inline Course::Course(string ma, string ten, int tinChi) {
    maMon = ma;
    tenMon = ten;
    soTinChi = tinChi;
}

inline string Course::getMaMon() const { return maMon; }
inline string Course::getTenMon() const { return tenMon; }
inline int Course::getSoTinChi() const { return soTinChi; }

inline void Course::setMaMon(string ma) { maMon = ma; }
inline void Course::setTenMon(string ten) { tenMon = ten; }

inline bool Course::setSoTinChi(int tinChi) {
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
        cout << "So tin chi (> 0): ";
        cin >> tinChi;
        if (tinChi <= 0) {
            cout << "So tin chi phai lon hon 0. Nhap lai.\n";
        }
    } while (tinChi <= 0);
    cin.ignore(10000, '\n');
    soTinChi = tinChi;
}

inline void Course::xuat() const {
    cout << "Ma mon hoc: " << maMon << '\n';
    cout << "Ten mon hoc: " << tenMon << '\n';
    cout << "So tin chi: " << soTinChi << '\n';
}

inline bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}

#endif
