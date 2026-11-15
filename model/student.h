#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include <string>
using namespace std;

// ============================================================
//  Cac ham tien ich giai ma thong tin tu Ma Sinh Vien
//  Quy uoc MSSV 9 chu so: [3 so Ma Khoa] + [2 so Khoa hoc] + [4 so Thu tu]
//  Vi du: 102230001 -> Khoa: CNTT, Khoa hoc: K23
// ============================================================
inline string getKhoaFromMaSV(const string& maSV) {
    if (maSV.length() >= 3) {
        string prefix = maSV.substr(0, 3);
        if (prefix == "102") return "CNTT";
        if (prefix == "105") return "DTVT";
        if (prefix == "101") return "CoKhi";
        if (prefix == "103") return "Dien";
        if (prefix == "106") return "Hoa";
        if (prefix == "107") return "XayDung";
    }
    return "";
}

inline string getKhoaHocFromMaSV(const string& maSV) {
    if (maSV.length() >= 5) {
        char c1 = maSV[3];
        char c2 = maSV[4];
        if (c1 >= '0' && c1 <= '9' && c2 >= '0' && c2 <= '9') {
            return string("K") + c1 + c2;
        }
    }
    return "";
}

inline string getTenKhoaDayDu(const string& maKhoaOrNganh) {
    if (maKhoaOrNganh == "102" || maKhoaOrNganh == "CNTT") return "Cong nghe Thong tin";
    if (maKhoaOrNganh == "105" || maKhoaOrNganh == "DTVT") return "Dien tu - Vien thong";
    if (maKhoaOrNganh == "101" || maKhoaOrNganh == "CoKhi") return "Co khi";
    if (maKhoaOrNganh == "103" || maKhoaOrNganh == "Dien") return "Dien - Dien tu";
    if (maKhoaOrNganh == "106" || maKhoaOrNganh == "Hoa") return "Hoa";
    if (maKhoaOrNganh == "107" || maKhoaOrNganh == "XayDung") return "Xay dung";
    return maKhoaOrNganh;
}

class Student : public Person {
private:
    string lop;
    string nganhHoc;
    string khoa;      
    string khoaHoc;
    int soKiDaHoc;

public:
    Student();
    Student(string maSV, string ten, string sdt, string mail,
            string lopSV, string nganh, string k = "", string kh = "", int soKi = 0);

    string getMaSV() const;
    string getLop() const;
    string getNganhHoc() const;
    string getKhoa() const;
    string getKhoaHoc() const;
    int getSoKiDaHoc() const;
    int getHocKyHienTai() const; // Tien trinh hoc ky hien tai = soKiDaHoc + 1

    void setMaSV(string maSV);
    void setLop(string lopSV);
    void setNganhHoc(string nganh);
    void setKhoa(string k);
    void setKhoaHoc(string kh);
    void setSoKiDaHoc(int soKi);

    bool operator==(const Student& other) const;
};

inline Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
    khoa = "";
    khoaHoc = "";
    soKiDaHoc = 0;
}

inline Student::Student(string maSV, string ten, string sdt, string mail,
                        string lopSV, string nganh, string k, string kh, int soKi)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    soKiDaHoc = (soKi >= 0 ? soKi : 0);

    // Tu dong nhan dien Khoa va Khoa hoc neu chua duoc truyen vao
    if (k.empty()) {
        k = getKhoaFromMaSV(maSV);
    }
    if (kh.empty()) {
        kh = getKhoaHocFromMaSV(maSV);
    }
    if (nganh.empty() && !k.empty()) {
        nganh = k;
    }

    khoa = k;
    khoaHoc = kh;
    nganhHoc = nganh;
}

inline string Student::getMaSV() const { return maNguoi; }
inline string Student::getLop() const { return lop; }
inline string Student::getNganhHoc() const { return nganhHoc; }
inline string Student::getKhoa() const { return khoa; }
inline string Student::getKhoaHoc() const { return khoaHoc; }
inline int Student::getSoKiDaHoc() const { return soKiDaHoc; }
inline int Student::getHocKyHienTai() const { return soKiDaHoc + 1; }

inline void Student::setMaSV(string maSV) {
    maNguoi = maSV;
    if (khoa.empty()) khoa = getKhoaFromMaSV(maSV);
    if (khoaHoc.empty()) khoaHoc = getKhoaHocFromMaSV(maSV);
    if (nganhHoc.empty() && !khoa.empty()) nganhHoc = khoa;
}

inline void Student::setLop(string lopSV) { lop = lopSV; }
inline void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }
inline void Student::setKhoa(string k) { khoa = k; }
inline void Student::setKhoaHoc(string kh) { khoaHoc = kh; }
inline void Student::setSoKiDaHoc(int soKi) {
    if (soKi >= 0) soKiDaHoc = soKi;
}

inline bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}

#endif // STUDENT_H
