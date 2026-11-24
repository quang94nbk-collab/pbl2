#ifndef course_h
#define course_h

#include <string>
#include "../DSA/DoublyLinkedList.h"

using namespace std;

class ClassSection;

// ============================================================
//  Course - Lop dai dien cho Mon hoc
//  Quan he:
//    - Course HAS-MANY ClassSection (Aggregation: Chua dung - Huong 2)
//      Mot mon hoc quan ly danh sach cac lop hoc phan truc thuoc.
// ============================================================
class Course {
private:
    string maMon;
    string tenMon;
    int soTinChi;
    string khoa;      
    string khoaHoc;  
    string nganhHoc;  
    int hocKy;       

    // Huong 2: Quan he Aggregation - Mon hoc chua danh sach cac Lop hoc phan
    DoubleLinkedList<ClassSection*> dsLopHocPhan;

public:
    Course();
    Course(string ma, string ten, int tinChi, string k = "", string kh = "", string ng = "", int hk = 1);
    Course(const Course& other);
    Course& operator=(const Course& other);
    ~Course();

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

    // Cac phuong thuc quan ly danh sach Lop hoc phan (Aggregation)
    void themLopHocPhan(ClassSection* sec);
    bool xoaLopHocPhan(ClassSection* sec);
    const DoubleLinkedList<ClassSection*>& getDanhSachLopHP() const;
    int getSoLopHocPhan() const;
    void clearDanhSachLopHP();

    bool operator==(const Course& other) const;
};

#endif
