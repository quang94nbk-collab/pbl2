#include "student.h"

// ============================================================
//  Cac ham tien ich giai ma thong tin tu Ma Sinh Vien
// ============================================================
string getKhoaFromMaSV(const string& maSV) {
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

string getKhoaHocFromMaSV(const string& maSV) {
    if (maSV.length() >= 5) {
        char c1 = maSV[3];
        char c2 = maSV[4];
        if (c1 >= '0' && c1 <= '9' && c2 >= '0' && c2 <= '9') {
            return string("K") + c1 + c2;
        }
    }
    return "";
}

string getTenKhoaDayDu(const string& maKhoaOrNganh) {
    if (maKhoaOrNganh == "102" || maKhoaOrNganh == "CNTT") return "Cong nghe Thong tin";
    if (maKhoaOrNganh == "105" || maKhoaOrNganh == "DTVT") return "Dien tu - Vien thong";
    if (maKhoaOrNganh == "101" || maKhoaOrNganh == "CoKhi") return "Co khi";
    if (maKhoaOrNganh == "103" || maKhoaOrNganh == "Dien") return "Dien - Dien tu";
    if (maKhoaOrNganh == "106" || maKhoaOrNganh == "Hoa") return "Hoa";
    if (maKhoaOrNganh == "107" || maKhoaOrNganh == "XayDung") return "Xay dung";
    return maKhoaOrNganh;
}

bool isValidMaSV(const string& maSV) {
    if (maSV.length() != 9) return false;
    for (size_t i = 0; i < maSV.length(); ++i) {
        if (maSV[i] < '0' || maSV[i] > '9') return false;
    }
    if (getKhoaFromMaSV(maSV).empty()) return false;
    return true;
}

// ============================================================
//  Student Implementation
// ============================================================
Student::Student() : Person() {
    lop = "";
    nganhHoc = "";
    khoa = "";
    khoaHoc = "";
    soKiDaHoc = 0;
}

Student::Student(string maSV, string ten, string sdt, string mail,
                 string lopSV, string nganh, string k, string kh, int soKi)
    : Person(maSV, ten, sdt, mail) {
    lop = lopSV;
    soKiDaHoc = (soKi >= 0 ? soKi : 0);

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

Student::Student(const Student& other)
    : Person(other.maNguoi, other.hoTen, other.soDienThoai, other.email) {
    lop = other.lop;
    nganhHoc = other.nganhHoc;
    khoa = other.khoa;
    khoaHoc = other.khoaHoc;
    soKiDaHoc = other.soKiDaHoc;

    auto cur = other.dsDangKy.getHead();
    while (cur != nullptr) {
        dsDangKy.pushBack(cur->data);
        cur = cur->next;
    }
}

Student& Student::operator=(const Student& other) {
    if (this != &other) {
        maNguoi = other.maNguoi;
        hoTen = other.hoTen;
        soDienThoai = other.soDienThoai;
        email = other.email;
        lop = other.lop;
        nganhHoc = other.nganhHoc;
        khoa = other.khoa;
        khoaHoc = other.khoaHoc;
        soKiDaHoc = other.soKiDaHoc;

        dsDangKy.clear();
        auto cur = other.dsDangKy.getHead();
        while (cur != nullptr) {
            dsDangKy.pushBack(cur->data);
            cur = cur->next;
        }
    }
    return *this;
}

Student::~Student() {
    dsDangKy.clear();
}

string Student::getMaSV() const { return maNguoi; }
string Student::getLop() const { return lop; }
string Student::getNganhHoc() const { return nganhHoc; }
string Student::getKhoa() const { return khoa; }
string Student::getKhoaHoc() const { return khoaHoc; }
int Student::getSoKiDaHoc() const { return soKiDaHoc; }

int Student::getHocKyTienTrinh(int systemSemester) const {
    string kh = khoaHoc;
    if (kh.empty()) kh = getKhoaHocFromMaSV(maNguoi);
    if (kh.length() >= 3 && (kh[0] == 'K' || kh[0] == 'k')) {
        try {
            int khoaNum = stoi(kh.substr(1));
            const int currentBaseYear = 24;
            int namThu = (currentBaseYear - khoaNum) + 1;
            if (namThu < 1) namThu = 1;
            int sem = (namThu - 1) * 2 + (systemSemester == 2 ? 2 : 1);
            if (sem > 0) return sem;
        } catch (...) {}
    }
    if (soKiDaHoc > 0) return soKiDaHoc + 1;
    return (systemSemester > 0 ? systemSemester : 1);
}

int Student::getHocKyHienTai() const { return getHocKyTienTrinh(1); }
int Student::getHocKyHienTai(int systemSemester) const { return getHocKyTienTrinh(systemSemester); }

void Student::setMaSV(string maSV) {
    maNguoi = maSV;
    if (khoa.empty()) khoa = getKhoaFromMaSV(maSV);
    if (khoaHoc.empty()) khoaHoc = getKhoaHocFromMaSV(maSV);
    if (nganhHoc.empty() && !khoa.empty()) nganhHoc = khoa;
}

void Student::setLop(string lopSV) { lop = lopSV; }
void Student::setNganhHoc(string nganh) { nganhHoc = nganh; }
void Student::setKhoa(string k) { khoa = k; }
void Student::setKhoaHoc(string kh) { khoaHoc = kh; }
void Student::setSoKiDaHoc(int soKi) {
    if (soKi >= 0) soKiDaHoc = soKi;
}

void Student::themDangKy(Registration* reg) {
    if (reg != nullptr && dsDangKy.find(reg) == nullptr) {
        dsDangKy.pushBack(reg);
    }
}

bool Student::xoaDangKy(Registration* reg) {
    if (reg == nullptr) return false;
    auto node = dsDangKy.find(reg);
    if (node != nullptr) {
        dsDangKy.eraseNode(node);
        return true;
    }
    return false;
}

const DoubleLinkedList<Registration*>& Student::getDanhSachDangKy() const {
    return dsDangKy;
}

int Student::getSoMonDangKy() const {
    int count = 0;
    auto cur = dsDangKy.getHead();
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }
    return count;
}

void Student::clearDangKy() {
    dsDangKy.clear();
}

bool Student::operator==(const Student& other) const {
    return maNguoi == other.maNguoi;
}
