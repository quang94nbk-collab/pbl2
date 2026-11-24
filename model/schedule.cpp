#include "schedule.h"

Schedule::Schedule()
    : thu(""), tietBatDau(0), tietKetThuc(0), phong("") {}

Schedule::Schedule(string t, int start, int end, string p)
    : thu(t), tietBatDau(start), tietKetThuc(end), phong(p) {}

string Schedule::getThu() const { return thu; }
int    Schedule::getTietBatDau() const { return tietBatDau; }
int    Schedule::getTietKetThuc() const { return tietKetThuc; }
string Schedule::getPhong() const { return phong; }

void Schedule::setThu(const string& t) { thu = t; }
void Schedule::setTietBatDau(int start) { tietBatDau = start; }
void Schedule::setTietKetThuc(int end) { tietKetThuc = end; }
void Schedule::setPhong(const string& p) { phong = p; }

bool Schedule::trungLichVoi(const Schedule& other) const {
    if (thu != other.thu) return false;
    return (tietBatDau < other.tietKetThuc) && (other.tietBatDau < tietKetThuc);
}

bool Schedule::hopLe() const {
    return (tietBatDau > 0) && (tietKetThuc > tietBatDau);
}

bool Schedule::operator==(const Schedule& other) const {
    return thu == other.thu &&
           tietBatDau == other.tietBatDau &&
           tietKetThuc == other.tietKetThuc &&
           phong == other.phong;
}
