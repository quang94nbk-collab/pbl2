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
Course::Course() {
    maMon = "";
    tenMon = "";
    soTinChi = 0;
}

Course::Course(string ma, string ten, int tinChi) {
    maMon = ma;
    tenMon = ten;
    soTinChi = tinChi;
}

string Course::getMaMon() const { return maMon; }
string Course::getTenMon() const { return tenMon; }
int Course::getSoTinChi() const { return soTinChi; }

void Course::setMaMon(string ma) { maMon = ma; }
void Course::setTenMon(string ten) { tenMon = ten; }

bool Course::setSoTinChi(int tinChi) {
    if (tinChi <= 0) {
        return false;
    }
    soTinChi = tinChi;
    return true;
}

void Course::nhap() {
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

void Course::xuat() const {
    cout << "Ma mon hoc: " << maMon << '\n';
    cout << "Ten mon hoc: " << tenMon << '\n';
    cout << "So tin chi: " << soTinChi << '\n';
}

bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}

#endif
