#ifndef course_h
#define course_h
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

inline bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}

#endif
