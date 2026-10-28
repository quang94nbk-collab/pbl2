#ifndef person_h
#define person_h
#include<iostream>
#include<string>
using namespace std;

class Person {
protected:
    string maNguoi;
    string hoTen;
    string soDienThoai;
    string email;

public:
    Person();
    Person(string ma, string ten, string sdt, string mail);
    virtual ~Person();

    string getMaNguoi() const;
    string getHoTen() const;
    string getSoDienThoai() const;
    string getEmail() const;

    void setMaNguoi(string ma);
    void setHoTen(string ten);
    void setSoDienThoai(string sdt);
    void setEmail(string mail);

    virtual void nhap();
    virtual void xuat() const;
};

inline Person::Person() {
    maNguoi = "";
    hoTen = "";
    soDienThoai = "";
    email = "";
}

inline Person::Person(string ma, string ten, string sdt, string mail) {
    maNguoi = ma;
    hoTen = ten;
    soDienThoai = sdt;
    email = mail;
}

inline Person::~Person() {}

inline string Person::getMaNguoi() const { return maNguoi; }
inline string Person::getHoTen() const { return hoTen; }
inline string Person::getSoDienThoai() const { return soDienThoai; }
inline string Person::getEmail() const { return email; }

inline void Person::setMaNguoi(string ma) { maNguoi = ma; }
inline void Person::setHoTen(string ten) { hoTen = ten; }
inline void Person::setSoDienThoai(string sdt) { soDienThoai = sdt; }
inline void Person::setEmail(string mail) { email = mail; }

inline void Person::nhap() {
    cout << "Ma nguoi: ";
    getline(cin >> ws, maNguoi);
    cout << "Ho ten: ";
    getline(cin, hoTen);
    cout << "So dien thoai: ";
    getline(cin, soDienThoai);
    cout << "Email: ";
    getline(cin, email);
}

inline void Person::xuat() const {
    cout << "Ma: " << maNguoi << '\n';
    cout << "Ho ten: " << hoTen << '\n';
    cout << "So dien thoai: " << soDienThoai << '\n';
    cout << "Email: " << email << '\n';
}

#endif
