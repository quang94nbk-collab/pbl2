#include "schedule.h"

string Schedule::normalizeThu(const string& raw) {
    size_t first = raw.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = raw.find_last_not_of(" \t\r\n");
    string t = raw.substr(first, last - first + 1);

    if (t == "2" || t == "Thu 2" || t == "thu 2" || t == "Thứ 2" || t == "thứ 2" || t == "Hai" || t == "hai" || t == "T2") return "2";
    if (t == "3" || t == "Thu 3" || t == "thu 3" || t == "Thứ 3" || t == "thứ 3" || t == "Ba"  || t == "ba"  || t == "T3") return "3";
    if (t == "4" || t == "Thu 4" || t == "thu 4" || t == "Thứ 4" || t == "thứ 4" || t == "Tu"  || t == "Tư"  || t == "T4") return "4";
    if (t == "5" || t == "Thu 5" || t == "thu 5" || t == "Thứ 5" || t == "thứ 5" || t == "Nam" || t == "Năm" || t == "T5") return "5";
    if (t == "6" || t == "Thu 6" || t == "thu 6" || t == "Thứ 6" || t == "thứ 6" || t == "Sau" || t == "Sáu" || t == "T6") return "6";
    if (t == "7" || t == "Thu 7" || t == "thu 7" || t == "Thứ 7" || t == "thứ 7" || t == "Bay" || t == "Bảy" || t == "T7") return "7";
    if (t == "CN" || t == "cn" || t == "8" || t == "Chu nhat" || t == "chu nhat" || t == "Chủ nhật" || t == "chủ nhật") return "CN";
    return t;
}

Schedule::Schedule()
    : thu(""), tietBatDau(0), tietKetThuc(0), phong("") {}

Schedule::Schedule(string t, int start, int end, string p)
    : thu(normalizeThu(t)), tietBatDau(start), tietKetThuc(end), phong(p) {}

string Schedule::getThu() const { return thu; }
int    Schedule::getTietBatDau() const { return tietBatDau; }
int    Schedule::getTietKetThuc() const { return tietKetThuc; }
string Schedule::getPhong() const { return phong; }

void Schedule::setThu(const string& t) { thu = normalizeThu(t); }
void Schedule::setTietBatDau(int start) { tietBatDau = start; }
void Schedule::setTietKetThuc(int end) { tietKetThuc = end; }
void Schedule::setPhong(const string& p) { phong = p; }

bool Schedule::trungLichVoi(const Schedule& other) const {
    if (thu.empty() || other.thu.empty() || thu != other.thu) return false;
    if (!hopLe() || !other.hopLe()) return false;
    // Doan dong [tietBatDau, tietKetThuc]: VD Tiet 1-3 va Tiet 3-5 trung nhau tai Tiet 3
    return (tietBatDau <= other.tietKetThuc) && (other.tietBatDau <= tietKetThuc);
}

bool Schedule::hopLe() const {
    return (tietBatDau > 0) && (tietKetThuc >= tietBatDau);
}

bool Schedule::operator==(const Schedule& other) const {
    return thu == other.thu &&
           tietBatDau == other.tietBatDau &&
           tietKetThuc == other.tietKetThuc &&
           phong == other.phong;
}
