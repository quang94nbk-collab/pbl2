#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
using namespace std;

// ============================================================
//  Schedule - Lop dai dien cho Thoi khoa bieu / Lich hoc
//  Quan he:
//    - ClassSection HAS-A Schedule (Composition: Hop thanh)
// ============================================================
class Schedule {
private:
    string thu;         // Thu hoc chuan hoa: "2".."7", "CN"
    int    tietBatDau;  // Tiet bat dau (>= 1)
    int    tietKetThuc; // Tiet ket thuc (>= tietBatDau, doan dong)
    string phong;       // Phong hoc

public:
    static string normalizeThu(const string& t);

    Schedule();
    Schedule(string t, int start, int end, string p);

    string getThu() const;
    int    getTietBatDau() const;
    int    getTietKetThuc() const;
    string getPhong() const;

    void setThu(const string& t);
    void setTietBatDau(int start);
    void setTietKetThuc(int end);
    void setPhong(const string& p);

    // Kiem tra 2 lich hoc co bi trung nhau khong
    bool trungLichVoi(const Schedule& other) const;

    // Kiem tra tinh hop le cua tiet hoc
    bool hopLe() const;

    bool operator==(const Schedule& other) const;
};

#endif // SCHEDULE_H
