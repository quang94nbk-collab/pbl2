#ifndef CURRICULUM_H
#define CURRICULUM_H

#include <iostream>
#include <string>

using namespace std;

class CurriculumItem {
private:
    string maKhoa;      // "100", "101", "102", "103", hoac "ALL"
    string maMon;        // Khop voi Course::getMaMon()
    int hocKy;           // Hoc ky goi y (1, 2, 3...)
    bool batBuoc;        // true: Bat buoc, false: Tu chon

public:
    CurriculumItem();
    CurriculumItem(string khoa, string mon, int ky, bool isRequired = true);

    string getMaKhoa() const;
    string getTenKhoa() const;
    string getMaMon() const;
    int getHocKy() const;
    bool isBatBuoc() const;

    void setMaKhoa(string khoa);
    void setMaMon(string mon);
    void setHocKy(int ky);
    void setBatBuoc(bool isRequired);

    void xuat() const;
    bool operator==(const CurriculumItem& other) const;
};

inline CurriculumItem::CurriculumItem()
    : maKhoa(""), maMon(""), hocKy(1), batBuoc(true) {}

inline CurriculumItem::CurriculumItem(string khoa, string mon, int ky, bool isRequired)
    : maKhoa(khoa), maMon(mon), hocKy(ky), batBuoc(isRequired) {}

inline string CurriculumItem::getMaKhoa() const { return maKhoa; }

inline string CurriculumItem::getTenKhoa() const {
    if (maKhoa == "100") return "Khoa Co ban / Dai cuong";
    if (maKhoa == "101") return "Khoa Co khi";
    if (maKhoa == "102") return "Khoa Cong nghe Thong tin";
    if (maKhoa == "103") return "Khoa Dien - Dien tu";
    if (maKhoa == "ALL") return "Dung chung cho tat ca cac khoa";
    return "Khong xac dinh";
}

inline string CurriculumItem::getMaMon() const { return maMon; }
inline int CurriculumItem::getHocKy() const { return hocKy; }
inline bool CurriculumItem::isBatBuoc() const { return batBuoc; }

inline void CurriculumItem::setMaKhoa(string khoa) { maKhoa = khoa; }
inline void CurriculumItem::setMaMon(string mon) { maMon = mon; }
inline void CurriculumItem::setHocKy(int ky) { hocKy = ky; }
inline void CurriculumItem::setBatBuoc(bool isRequired) { batBuoc = isRequired; }

inline void CurriculumItem::xuat() const {
    cout << "Khoa: " << maKhoa << " (" << getTenKhoa() << ") | "
         << "Ma mon: " << maMon << " | "
         << "Hoc ky: " << hocKy << " | "
         << (batBuoc ? "Bat buoc" : "Tu chon") << '\n';
}

inline bool CurriculumItem::operator==(const CurriculumItem& other) const {
    return maKhoa == other.maKhoa && maMon == other.maMon;
}

#endif

