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
    string khoa;      // Khoa quan ly / ap dung (vi du: CNTT, Dien, Co khi...)
    string khoaHoc;   // Khoa hoc cua sinh vien (vi du: K22, K23...)
    string nganhHoc;  // Nganh hoc (vi du: Dac thu, Ngon ngu Nhat, Chung...)
    int hocKy;        // Hoc ky: 1 hoac 2 (do Admin quyet dinh)

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
};

inline Course::Course() {
    maMon = "";
    tenMon = "";
    soTinChi = 0;
    khoa = "";
    khoaHoc = "";
    nganhHoc = "";
    hocKy = 1;
}

inline Course::Course(string ma, string ten, int tinChi, string k, string kh, string ng, int hk) {
    maMon = ma;
    tenMon = ten;
    soTinChi = tinChi;
    khoa = k;
    khoaHoc = kh;
    nganhHoc = ng;
    hocKy = hk;
}

inline string Course::getMaMon() const { return maMon; }
inline string Course::getTenMon() const { return tenMon; }
inline int Course::getSoTinChi() const { return soTinChi; }
inline string Course::getKhoa() const { return khoa; }
inline string Course::getKhoaHoc() const { return khoaHoc; }
inline string Course::getNganhHoc() const { return nganhHoc; }
inline int Course::getHocKy() const { return hocKy; }

inline void Course::setMaMon(string ma) { maMon = ma; }
inline void Course::setTenMon(string ten) { tenMon = ten; }
inline void Course::setKhoa(string k) { khoa = k; }
inline void Course::setKhoaHoc(string kh) { khoaHoc = kh; }
inline void Course::setNganhHoc(string ng) { nganhHoc = ng; }

inline bool Course::setHocKy(int hk) {
    if (hk == 1 || hk == 2) {
        hocKy = hk;
        return true;
    }
    return false;
}

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

    cout << "Khoa quan ly (vi du: CNTT, Dien, Co khi): ";
    getline(cin, khoa);
    cout << "Khoa hoc ap dung (vi du: K22, K23): ";
    getline(cin, khoaHoc);
    cout << "Nganh hoc (vi du: Dac thu, Ngon ngu Nhat, hoac de 'Chung'): ";
    getline(cin, nganhHoc);

    int hk = 1;
    do {
        cout << "Hoc ky ap dung (1 hoac 2): ";
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

#endif
