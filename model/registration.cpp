#include "registration.h"
#include "student.h"
#include "classsection.h"

Registration::Registration(string maDK, string maSinhVien, string maLop, string ngayDK)
    : maDangKy(maDK), maSV(maSinhVien), maLopHP(maLop), ngayDangKy(ngayDK),
      sinhVien(nullptr), lopHocPhan(nullptr) {}

Registration::Registration(Student* sv, ClassSection* lop, string ngayDK, string maDK)
    : ngayDangKy(ngayDK), sinhVien(sv), lopHocPhan(lop) {
    maSV = sv ? sv->getMaSV() : "";
    maLopHP = lop ? lop->getMaLopHP() : "";
    if (maDK.empty() && sv && lop) {
        maDangKy = "DK_" + maSV + "_" + maLopHP;
    } else {
        maDangKy = maDK;
    }
}

string Registration::getMaDangKy() const { return maDangKy; }

string Registration::getMaSV() const {
    if (sinhVien != nullptr) return sinhVien->getMaSV();
    return maSV;
}

string Registration::getMaLopHP() const {
    if (lopHocPhan != nullptr) return lopHocPhan->getMaLopHP();
    return maLopHP;
}

string Registration::getNgayDangKy() const { return ngayDangKy; }

void Registration::setMaDangKy(string ma) { maDangKy = ma; }
void Registration::setNgayDangKy(string ngay) { ngayDangKy = ngay; }

Student* Registration::getSinhVien() const { return sinhVien; }
ClassSection* Registration::getLopHocPhan() const { return lopHocPhan; }

void Registration::setSinhVien(Student* sv) {
    sinhVien = sv;
    if (sv != nullptr) maSV = sv->getMaSV();
}

void Registration::setLopHocPhan(ClassSection* sec) {
    lopHocPhan = sec;
    if (sec != nullptr) maLopHP = sec->getMaLopHP();
}

bool Registration::operator==(const Registration& other) const {
    return maDangKy == other.maDangKy;
}
