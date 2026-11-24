#ifndef person_h
#define person_h

#include <string>
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
};

#endif
