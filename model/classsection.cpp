#include "classsection.h"
#include "registration.h"

ClassSection::ClassSection() {
    maLopHP = "";
    maMon = "";
    monHoc = nullptr;
    giangVien = "";
    lichHoc = Schedule("", 0, 0, "");
    siSoToiDa = 50;
    soLuongDaDangKy = 0;
    hocKy = 1;
    nhomLopHP = "";
}

ClassSection::ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                           int tietDau, int tietCuoi, string phongHoc, int toiDa,
                           int hk, string nhom)
    : lichHoc(ngayHoc, tietDau, tietCuoi, phongHoc) {
    maLopHP = maLop;
    maMon = maMonHoc;
    monHoc = nullptr;
    giangVien = gv;
    siSoToiDa = toiDa;
    soLuongDaDangKy = 0;
    hocKy = hk;
    nhomLopHP = nhom;
}

ClassSection::ClassSection(string maLop, Course* course, string gv, const Schedule& schedule,
                           int toiDa, int hk, string nhom)
    : lichHoc(schedule) {
    maLopHP = maLop;
    monHoc = course;
    maMon = course ? course->getMaMon() : "";
    giangVien = gv;
    siSoToiDa = toiDa;
    soLuongDaDangKy = 0;
    hocKy = hk;
    nhomLopHP = nhom;
}

ClassSection::ClassSection(const ClassSection& other)
    : lichHoc(other.lichHoc) {
    maLopHP = other.maLopHP;
    maMon = other.maMon;
    monHoc = other.monHoc;
    giangVien = other.giangVien;
    siSoToiDa = other.siSoToiDa;
    soLuongDaDangKy = other.soLuongDaDangKy;
    hocKy = other.hocKy;
    nhomLopHP = other.nhomLopHP;

    auto cur = other.dsDangKy.getHead();
    while (cur != nullptr) {
        dsDangKy.pushBack(cur->data);
        cur = cur->next;
    }
}

ClassSection& ClassSection::operator=(const ClassSection& other) {
    if (this != &other) {
        maLopHP = other.maLopHP;
        maMon = other.maMon;
        monHoc = other.monHoc;
        giangVien = other.giangVien;
        lichHoc = other.lichHoc;
        siSoToiDa = other.siSoToiDa;
        soLuongDaDangKy = other.soLuongDaDangKy;
        hocKy = other.hocKy;
        nhomLopHP = other.nhomLopHP;

        dsDangKy.clear();
        auto cur = other.dsDangKy.getHead();
        while (cur != nullptr) {
            dsDangKy.pushBack(cur->data);
            cur = cur->next;
        }
    }
    return *this;
}

ClassSection::~ClassSection() {
    dsDangKy.clear();
}

string ClassSection::getMaLopHP() const { return maLopHP; }

string ClassSection::getMaMon() const {
    if (monHoc != nullptr) return monHoc->getMaMon();
    return maMon;
}

Course* ClassSection::getMonHoc() const { return monHoc; }

void ClassSection::setMonHoc(Course* c) {
    monHoc = c;
    if (c != nullptr) maMon = c->getMaMon();
}

string ClassSection::getGiangVien() const { return giangVien; }
int ClassSection::getSiSoToiDa() const { return siSoToiDa; }
int ClassSection::getSoLuongDaDangKy() const { return soLuongDaDangKy; }
int ClassSection::getHocKy() const { return hocKy; }
string ClassSection::getNhomLopHP() const { return nhomLopHP; }

const Schedule& ClassSection::getSchedule() const { return lichHoc; }
Schedule& ClassSection::getSchedule() { return lichHoc; }
void ClassSection::setSchedule(const Schedule& s) { lichHoc = s; }

string ClassSection::getThu() const { return lichHoc.getThu(); }
int ClassSection::getTietBatDau() const { return lichHoc.getTietBatDau(); }
int ClassSection::getTietKetThuc() const { return lichHoc.getTietKetThuc(); }
string ClassSection::getPhong() const { return lichHoc.getPhong(); }

void ClassSection::setMaLopHP(string ma) { maLopHP = ma; }
void ClassSection::setMaMon(string ma) { maMon = ma; }
void ClassSection::setGiangVien(string gv) { giangVien = gv; }
void ClassSection::setThu(string t) { lichHoc.setThu(t); }
void ClassSection::setTietBatDau(int tiet) { lichHoc.setTietBatDau(tiet); }
void ClassSection::setTietKetThuc(int tiet) { lichHoc.setTietKetThuc(tiet); }
void ClassSection::setPhong(string ph) { lichHoc.setPhong(ph); }
void ClassSection::setSiSoToiDa(int siso) { if (siso > 0) siSoToiDa = siso; }

void ClassSection::setSoLuongDaDangKy(int soLuong) {
    if (soLuong >= 0) {
        soLuongDaDangKy = soLuong;
    }
}

bool ClassSection::setHocKy(int hk) {
    if (hk >= 1 && hk <= 10) {
        hocKy = hk;
        return true;
    }
    return false;
}

void ClassSection::setNhomLopHP(string nhom) { nhomLopHP = nhom; }

bool ClassSection::conCho() const {
    return soLuongDaDangKy < siSoToiDa;
}

bool ClassSection::tangSoLuong() {
    if (!conCho()) {
        return false;
    }
    soLuongDaDangKy++;
    return true;
}

void ClassSection::giamSoLuong() {
    if (soLuongDaDangKy > 0) {
        soLuongDaDangKy--;
    }
}

bool ClassSection::trungLichVoi(const ClassSection& other) const {
    return lichHoc.trungLichVoi(other.lichHoc);
}

void ClassSection::themDangKy(Registration* reg) {
    if (reg != nullptr && dsDangKy.find(reg) == nullptr) {
        dsDangKy.pushBack(reg);
    }
}

bool ClassSection::xoaDangKy(Registration* reg) {
    if (reg == nullptr) return false;
    auto node = dsDangKy.find(reg);
    if (node != nullptr) {
        dsDangKy.eraseNode(node);
        return true;
    }
    return false;
}

const DoubleLinkedList<Registration*>& ClassSection::getDanhSachDangKy() const {
    return dsDangKy;
}

int ClassSection::getSoLuongDangKyThucTe() const {
    int count = 0;
    auto cur = dsDangKy.getHead();
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }
    return count;
}

void ClassSection::clearDangKy() {
    dsDangKy.clear();
}

bool ClassSection::operator==(const ClassSection& other) const {
    return maLopHP == other.maLopHP;
}
