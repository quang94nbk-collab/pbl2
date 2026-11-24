#ifndef reg_h
#define reg_h

#include <string>

using namespace std;

class Student;
class ClassSection;

// ============================================================
//  Registration - Lop lien ket giua Student va ClassSection
//  (Association Class theo Huong 1 - Cach 1.2)
//  Quan he:
//    - Tro truc tiep toi Student* va ClassSection*
//    - Dong thoi luu tru thong tin bo sung cua viec dang ky (ngayDangKy, maDangKy)
// ============================================================
class Registration {
private:
    string maDangKy;
    string maSV;        // Ma SV (dung de serialization / fallback)
    string maLopHP;     // Ma Lop HP (dung de serialization / fallback)
    string ngayDangKy;

    // Huong 1.2: Con tro truc tiep toi 2 thuc the tham gia quan he N-N
    Student*      sinhVien;
    ClassSection* lopHocPhan;

public:
    Registration(string maDK = "", string maSinhVien = "", string maLop = "", string ngayDK = "");
    Registration(Student* sv, ClassSection* lop, string ngayDK = "", string maDK = "");

    string getMaDangKy() const;
    string getMaSV() const;
    string getMaLopHP() const;
    string getNgayDangKy() const;

    void setMaDangKy(string ma);
    void setNgayDangKy(string ngay);

    // Cac phuong thuc con tro thuan OOP
    Student* getSinhVien() const;
    ClassSection* getLopHocPhan() const;
    void setSinhVien(Student* sv);
    void setLopHocPhan(ClassSection* sec);

    bool operator==(const Registration& other) const;
};

#endif
