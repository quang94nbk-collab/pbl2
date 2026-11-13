#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include <string>
using namespace std;

class Student : public Person {
private:
    string lop;
    string nganhHoc;
    string khoa;      
    string khoaHoc;   

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh, string k = "", string kh = "");

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;
    string getKhoa() const;
    string getKhoaHoc() const;

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);
    void setKhoa(string k);
    void setKhoaHoc(string kh);

    bool operator==(const Student& other) const;
};
inline Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
    khoa = "";
    khoaHoc = "";
}

inline Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh, string k, string kh)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    nganhHoc = nganh;
    khoa = k;
    khoaHoc = kh;
}

inline string Student::getMaSV() const { return maNguoi; }
inline string Student::getLop() const { return lop; }
inline string Student::getNganhHoc() const { return nganhHoc; }
inline string Student::getKhoa() const { return khoa; }
inline string Student::getKhoaHoc() const { return khoaHoc; }

inline void Student::setMaSV(string maSV) { maNguoi = maSV; }
inline void Student::setLop(string lopSV) { lop = lopSV; }
inline void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }
inline void Student::setKhoa(string k) { khoa = k; }
inline void Student::setKhoaHoc(string kh) { khoaHoc = kh; }

inline bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif