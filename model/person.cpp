#include "person.h"

Person::Person() {
    maNguoi = "";
    hoTen = "";
    soDienThoai = "";
    email = "";
}

Person::Person(string ma, string ten, string sdt, string mail) {
    maNguoi = ma;
    hoTen = ten;
    soDienThoai = sdt;
    email = mail;
}

Person::~Person() {}

string Person::getMaNguoi() const { return maNguoi; }
string Person::getHoTen() const { return hoTen; }
string Person::getSoDienThoai() const { return soDienThoai; }
string Person::getEmail() const { return email; }

void Person::setMaNguoi(string ma) { maNguoi = ma; }
void Person::setHoTen(string ten) { hoTen = ten; }
void Person::setSoDienThoai(string sdt) { soDienThoai = sdt; }
void Person::setEmail(string mail) { email = mail; }
