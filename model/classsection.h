#ifndef clss_h
#define clss_h

#include <string>
#include "course.h"
#include "schedule.h"
#include "../DSA/DoublyLinkedList.h"

using namespace std;

class Registration;

// ============================================================
//  ClassSection - Lop hoc phan
//  Quan he:
//    - ClassSection HAS-A Schedule (Composition: Hop thanh)
//      Moi lop hoc phan so huu mot lich hoc Schedule (thu, tiet, phong).
//    - ClassSection ASSOCIATES WITH Course (Association: Lien ket truc tiep - Huong 1)
//      Chua con tro Course* monHoc tro truc tiep toi mon hoc.
//    - ClassSection HAS-MANY Registration (Association Class - Huong 1.2)
//      Chua danh sach con tro toi cac phieu dang ky Registration cua lop.
// ============================================================
class ClassSection {
private:
    string    maLopHP;
    string    maMon;            // Ma mon hoc (dung de luu tru / fallback)
    Course*   monHoc;           // Huong 1: Con tro truc tiep toi doi tuong Course
    string    giangVien;
    Schedule  lichHoc;          // Quan he Composition: Lop hoc phan so huu mot Schedule
    int       siSoToiDa;
    int       soLuongDaDangKy;
    int       hocKy;
    string    nhomLopHP;

    // Huong 1.2: Danh sach con tro Registration (Quan he 2 chieu)
    DoubleLinkedList<Registration*> dsDangKy;

public:
    ClassSection();
    ClassSection(string maLop, string maMonHoc, string gv, string ngayHoc,
                 int tietDau, int tietCuoi, string phongHoc, int toiDa,
                 int hk = 1, string nhom = "");
    ClassSection(string maLop, Course* course, string gv, const Schedule& schedule,
                 int toiDa, int hk = 1, string nhom = "");
    ClassSection(const ClassSection& other);
    ClassSection& operator=(const ClassSection& other);
    ~ClassSection();

    string getMaLopHP() const;
    string getMaMon() const;
    Course* getMonHoc() const;
    void setMonHoc(Course* c);

    string getGiangVien() const;
    int getSiSoToiDa() const;
    int getSoLuongDaDangKy() const;
    int getHocKy() const;
    string getNhomLopHP() const;

    // Getter & Setter cho Schedule (Lich hoc - Composition)
    const Schedule& getSchedule() const;
    Schedule& getSchedule();
    void setSchedule(const Schedule& s);

    // Wrapper cac getter/setter cua Schedule de dam bao tuong thich 100%
    string getThu() const;
    int getTietBatDau() const;
    int getTietKetThuc() const;
    string getPhong() const;

    void setMaLopHP(string ma);
    void setMaMon(string ma);
    void setGiangVien(string gv);
    void setThu(string t);
    void setTietBatDau(int tiet);
    void setTietKetThuc(int tiet);
    void setPhong(string ph);
    void setSiSoToiDa(int siso);
    void setSoLuongDaDangKy(int soLuong);
    bool setHocKy(int hk);
    void setNhomLopHP(string nhom);

    bool conCho() const;
    bool tangSoLuong();
    void giamSoLuong();

    // Kiem tra trung lich: uy quyen cho Schedule xu ly
    bool trungLichVoi(const ClassSection& other) const;

    // Quan ly danh sach Registration (Huong 1.2)
    void themDangKy(Registration* reg);
    bool xoaDangKy(Registration* reg);
    const DoubleLinkedList<Registration*>& getDanhSachDangKy() const;
    int getSoLuongDangKyThucTe() const;
    void clearDangKy();

    bool operator==(const ClassSection& other) const;
};

#endif
