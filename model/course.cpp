#include "course.h"
#include "classsection.h"

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

Course::Course(const Course& other) {
    maMon = other.maMon;
    tenMon = other.tenMon;
    soTinChi = other.soTinChi;
    khoa = other.khoa;
    khoaHoc = other.khoaHoc;
    nganhHoc = other.nganhHoc;
    hocKy = other.hocKy;
    auto cur = other.dsLopHocPhan.getHead();
    while (cur != nullptr) {
        dsLopHocPhan.pushBack(cur->data);
        cur = cur->next;
    }
}

Course& Course::operator=(const Course& other) {
    if (this != &other) {
        maMon = other.maMon;
        tenMon = other.tenMon;
        soTinChi = other.soTinChi;
        khoa = other.khoa;
        khoaHoc = other.khoaHoc;
        nganhHoc = other.nganhHoc;
        hocKy = other.hocKy;
        dsLopHocPhan.clear();
        auto cur = other.dsLopHocPhan.getHead();
        while (cur != nullptr) {
            dsLopHocPhan.pushBack(cur->data);
            cur = cur->next;
        }
    }
    return *this;
}

Course::~Course() {
    dsLopHocPhan.clear();
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
    if (hk >= 1 && hk <= 10) {
        hocKy = hk;
        return true;
    }
    return false;
}

bool Course::setSoTinChi(int tinChi) {
    if (tinChi < 0) {
        return false;
    }
    soTinChi = tinChi;
    return true;
}

void Course::themLopHocPhan(ClassSection* sec) {
    if (sec != nullptr && dsLopHocPhan.find(sec) == nullptr) {
        dsLopHocPhan.pushBack(sec);
    }
}

bool Course::xoaLopHocPhan(ClassSection* sec) {
    if (sec == nullptr) return false;
    auto node = dsLopHocPhan.find(sec);
    if (node != nullptr) {
        dsLopHocPhan.eraseNode(node);
        return true;
    }
    return false;
}

const DoubleLinkedList<ClassSection*>& Course::getDanhSachLopHP() const {
    return dsLopHocPhan;
}

int Course::getSoLopHocPhan() const {
    int count = 0;
    auto cur = dsLopHocPhan.getHead();
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }
    return count;
}

void Course::clearDanhSachLopHP() {
    dsLopHocPhan.clear();
}

bool Course::operator==(const Course& other) const {
    return maMon == other.maMon;
}
