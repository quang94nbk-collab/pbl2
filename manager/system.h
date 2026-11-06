#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// DAL Repositories
#include "../dal/StudentRepository.h"
#include "../dal/CourseRepository.h"
#include "../dal/ClassSectionRepository.h"
#include "../dal/RegistrationRepository.h"

// DSA
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../DSA/queue.h"

// Models (gian tiep qua DAL, nhung van can include de dung kieu du lieu)
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/registration.h"
#include "../model/student.h"

using namespace std;

// ============================================================
//  GroupInfo - Thong tin nhom lop hoc phan
//  (Dung khi hien thi dang ky theo nhom)
// ============================================================
struct GroupInfo {
    string groupName;
    string khoa;
    string khoaHoc;
    string nganhHoc;
    int    hocKy;
    int    totalSections;
    int    totalCredits;
    string sectionIDs[50];

    GroupInfo();
};

inline GroupInfo::GroupInfo()
    : groupName(""), khoa(""), khoaHoc(""), nganhHoc(""),
      hocKy(1), totalSections(0), totalCredits(0) {}

// ============================================================
//  SystemManager - Lop quan ly nghiep vu tong hop
//
//  Kien truc:
//    SystemManager
//      +-- StudentRepository      (DAL: Student)
//      +-- CourseRepository       (DAL: Course)
//      +-- ClassSectionRepository (DAL: ClassSection + Waitlist)
//      +-- RegistrationRepository (DAL: Registration)
//
//  Nhiem vu cua SystemManager:
//    - Dieu phoi nghiep vu lien quan nhieu bang (cross-entity)
//    - Kiem tra rang buoc toan ven du lieu (FK, trung lich, ...)
//    - Xu ly logic dang ky / huy dang ky + tu dong xu ly waitlist
//    - Cac tinh toan tong hop (tong tin chi, thoi khoa bieu, ...)
//
//  Cac Repository chi chiu trach nhiem:
//    - Luu tru & truy xuat du lieu (CRUD thuan tuy)
//    - Tim kiem & loc du lieu theo tieu chi don gian
// ============================================================
class SystemManager {
private:
    StudentRepository      studentRepo;
    CourseRepository       courseRepo;
    ClassSectionRepository sectionRepo;
    RegistrationRepository registrationRepo;

    int currentSemester;

    static string today();
    static string toLower(string s);

public:
    SystemManager();

    int  getCurrentSemester() const;
    bool setCurrentSemester(int semester);

    // =========================================================
    //  STUDENT
    // =========================================================
    bool addStudent(const Student& s);
    bool deleteStudent(const string& maSV);
    Student* findStudent(const string& maSV);
    void displayAllStudents() const;
    void searchStudents(const string& kw) const;

    // =========================================================
    //  COURSE
    // =========================================================
    bool addCourse(const Course& c);
    bool deleteCourse(const string& maMon);
    Course* findCourse(const string& maMon);
    void displayAllCourses() const;
    void searchCourses(const string& kw) const;
    void displayCoursesByFilter(const string& khoa, const string& khoaHoc,
                                const string& nganh = "", int hocKy = 0) const;

    // =========================================================
    //  CLASS SECTION
    // =========================================================
    bool addClassSection(const ClassSection& sec);
    bool deleteClassSection(const string& maLopHP);
    ClassSection* findClassSection(const string& maLopHP);
    void displayAllSections() const;
    void searchSections(const string& kw) const;
    void displayStudentsInSection(const string& maLopHP);
    void displaySectionsByGroup() const;

    // =========================================================
    //  REGISTRATION - Logic nghiep vu chinh
    // =========================================================

    // Tim maLopHP sinh vien da dang ky cho 1 mon hoc cu the (neu co)
    string getRegisteredSectionForCourse(const string& maSV, const string& maMon) const;
    bool isSectionRegistered(const string& maSV, const string& maLopHP) const;
    int getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const;
    bool isTimeConflict(const string& maSV, const string& newSectionID);
    bool registerClass(const string& maSV, const string& maLopHP,
                       const string& registrationDate = "", bool checkSemester = true);
    bool cancelRegistration(const string& maSV, const string& maLopHP);
    void displayStudentRegistrations(const string& maSV) const;
    void displayAllRegistrations() const;

    // =========================================================
    //  TINH TOAN & THONG KE
    // =========================================================
    int calculateTotalCredits(const string& maSV) const;
    void displayStudentTimetable(const string& maSV) const;
    void checkTimeConflictInteractive(const string& maSV, const string& maLopHP) const;
    int getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const;
    int getMissingCoursesCount(const string& maSV, string missingIDs[], int maxMissing) const;
    void displaySemesterRegistrationStatus(const string& maSV) const;
    int getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const;

    // =========================================================
    //  Accessor truc tiep (tuong thich nguoc voi code cu)
    // =========================================================
    const DoubleLinkedList<Student>&      getStudents()      const;
    const DoubleLinkedList<Course>&       getCourses()       const;
    const DoubleLinkedList<ClassSection>& getSections()      const;
    const DoubleLinkedList<Registration>& getRegistrations() const;
};

// =============================================================
//  IMPLEMENTATION CAC PHUONG THUC CUA SystemManager
// =============================================================

inline string SystemManager::today() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    if (localTime == nullptr) return "";
    ostringstream date;
    date << put_time(localTime, "%Y-%m-%d");
    return date.str();
}

inline string SystemManager::toLower(string s) {
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = c + 32;
    return s;
}

inline SystemManager::SystemManager() : currentSemester(1) {}

inline int SystemManager::getCurrentSemester() const {
    return currentSemester;
}

inline bool SystemManager::setCurrentSemester(int semester) {
    if (semester == 1 || semester == 2) {
        currentSemester = semester;
        return true;
    }
    return false;
}

// =========================================================
//  STUDENT
// =========================================================

inline bool SystemManager::addStudent(const Student& s) {
    return studentRepo.add(s);
}

inline bool SystemManager::deleteStudent(const string& maSV) {
    // 1. Xoa het dang ky cua sinh vien (can update lai so luong lop HP)
    const int MAX = 100;
    string secIDs[MAX];
    int cnt = registrationRepo.getSectionsByStudent(maSV, secIDs, MAX);
    for (int i = 0; i < cnt; i++)
        cancelRegistration(maSV, secIDs[i]);

    // 2. Xoa sinh vien
    return studentRepo.remove(maSV);
}

inline Student* SystemManager::findStudent(const string& maSV) {
    return studentRepo.findById(maSV);
}

inline void SystemManager::displayAllStudents() const {
    studentRepo.displayAll();
}

inline void SystemManager::searchStudents(const string& kw) const {
    studentRepo.search(kw);
}

// =========================================================
//  COURSE
// =========================================================

inline bool SystemManager::addCourse(const Course& c) {
    return courseRepo.add(c);
}

inline bool SystemManager::deleteCourse(const string& maMon) {
    // Kiem tra con ClassSection thuoc mon nay khong
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaMon() == maMon) {
            cout << "Khong the xoa: Lop hoc phan '" << cur->data.getMaLopHP()
                 << "' dang thuoc mon nay. Vui long xoa lop hoc phan truoc!\n";
            return false;
        }
        cur = cur->next;
    }
    return courseRepo.remove(maMon);
}

inline Course* SystemManager::findCourse(const string& maMon) {
    return courseRepo.findById(maMon);
}

inline void SystemManager::displayAllCourses() const {
    courseRepo.displayAll();
}

inline void SystemManager::searchCourses(const string& kw) const {
    courseRepo.search(kw);
}

inline void SystemManager::displayCoursesByFilter(const string& khoa, const string& khoaHoc,
                                                 const string& nganh, int hocKy) const {
    courseRepo.displayByFilter(khoa, khoaHoc, nganh, hocKy);
}

// =========================================================
//  CLASS SECTION
// =========================================================

inline bool SystemManager::addClassSection(const ClassSection& sec) {
    // Kiem tra FK: maMon phai ton tai
    if (!courseRepo.exists(sec.getMaMon())) return false;
    return sectionRepo.add(sec);
}

inline bool SystemManager::deleteClassSection(const string& maLopHP) {
    // 1. Xoa het Registration cua lop nay
    registrationRepo.removeAllBySection(maLopHP);
    // 2. Xoa lop HP (va waitlist ben trong repo)
    return sectionRepo.remove(maLopHP);
}

inline ClassSection* SystemManager::findClassSection(const string& maLopHP) {
    return sectionRepo.findById(maLopHP);
}

inline void SystemManager::displayAllSections() const {
    sectionRepo.displayAll();
}

inline void SystemManager::searchSections(const string& kw) const {
    sectionRepo.search(kw);
}

inline void SystemManager::displayStudentsInSection(const string& maLopHP) {
    ClassSection* sec = findClassSection(maLopHP);
    if (!sec) { cout << "Khong tim thay lop hoc phan: " << maLopHP << "\n"; return; }
    cout << "  DANH SACH SINH VIEN DANG KY LOP HOC PHAN: " << maLopHP << "\n";
    cout << "  Mon: " << sec->getMaMon()
         << " | Nhom: " << (sec->getNhomLopHP().empty() ? "(Chua gan nhom)" : sec->getNhomLopHP())
         << " | Thu: "  << sec->getThu()
         << " | Tiet: " << sec->getTietBatDau() << "-" << sec->getTietKetThuc() << "\n";
    cout << "  Si so hien tai: " << sec->getSoLuongDaDangKy() << "/" << sec->getSiSoToiDa() << "\n";

    int count = 0;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP) {
            count++;
            string svID = cur->data.getMaSV();
            Student* sv = findStudent(svID);
            cout << left << setw(4) << count
                 << "MSSV: " << setw(12) << svID
                 << " | Ho ten: "  << setw(22) << (sv ? sv->getHoTen() : "(Chua cap nhat)")
                 << " | Lop: "     << setw(10) << (sv ? sv->getLop()   : "-")
                 << " | Ngay DK: " << cur->data.getNgayDangKy() << "\n";
        }
        cur = cur->next;
    }
    if (count == 0) cout << "(Chua co sinh vien nao dang ky vao lop hoc phan nay)\n";
}

inline void SystemManager::displaySectionsByGroup() const {
    cout << "  DANH SACH NHOM LOP HOC PHAN - HOC KY " << currentSemester << "\n";
    const int MAX_GROUPS = 100;
    string groupNames[MAX_GROUPS];
    int groupCount = sectionRepo.getGroupsForSemester(currentSemester, groupNames, MAX_GROUPS);

    if (groupCount == 0) {
        cout << "(Khong co nhom lop hoc phan nao trong hoc ky " << currentSemester << ")\n";
        return;
    }
    for (int g = 0; g < groupCount; g++) {
        cout << "\n>>> NHOM: " << groupNames[g] << "\n";
        int secCount = 0;
        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getHocKy() == currentSemester &&
                cur->data.getNhomLopHP() == groupNames[g]) {
                secCount++;
                string tenMon = cur->data.getMaMon();
                Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
                if (c) tenMon = c->getTenMon();
                cout << "  [" << secCount << "] " << cur->data.getMaLopHP()
                     << " | Mon: " << tenMon
                     << " | GV: "  << cur->data.getGiangVien()
                     << " | Thu "  << cur->data.getThu()
                     << " | Tiet " << cur->data.getTietBatDau() << "-" << cur->data.getTietKetThuc()
                     << " | Phong: "<< cur->data.getPhong()
                     << " | SS: "  << cur->data.getSoLuongDaDangKy() << "/" << cur->data.getSiSoToiDa()
                     << "\n";
            }
            cur = cur->next;
        }
        if (secCount == 0) cout << "  (Khong co lop hoc phan nao trong nhom nay)\n";
    }
    // Chua gan nhom
    bool hasUngrouped = false;
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP().empty()) {
            if (!hasUngrouped) { cout << "\n>>> CHUA GAN NHOM:\n"; hasUngrouped = true; }
            string tenMon = cur->data.getMaMon();
            Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
            if (c) tenMon = c->getTenMon();
            cout << "  " << cur->data.getMaLopHP()
                 << " | Mon: " << tenMon
                 << " | GV: "  << cur->data.getGiangVien()
                 << " | Thu "  << cur->data.getThu()
                 << " | Tiet " << cur->data.getTietBatDau() << "-" << cur->data.getTietKetThuc()
                 << " | SS: "  << cur->data.getSoLuongDaDangKy() << "/" << cur->data.getSiSoToiDa()
                 << "\n";
        }
        cur = cur->next;
    }
}

// =========================================================
//  REGISTRATION - Logic nghiep vu chinh
// =========================================================

// Tim maLopHP sinh vien da dang ky cho 1 mon hoc cu the (neu co)
inline string SystemManager::getRegisteredSectionForCourse(const string& maSV, const string& maMon) const {
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            ClassSection* sec =
                const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
            if (sec && sec->getMaMon() == maMon)
                return sec->getMaLopHP();
        }
        cur = cur->next;
    }
    return "";
}

inline bool SystemManager::isSectionRegistered(const string& maSV, const string& maLopHP) const {
    return registrationRepo.isRegistered(maSV, maLopHP);
}

inline int SystemManager::getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const {
    return registrationRepo.getSectionsByStudent(maSV, outSectionIDs, maxCount);
}

inline bool SystemManager::isTimeConflict(const string& maSV, const string& newSectionID) {
    ClassSection* target = findClassSection(newSectionID);
    if (!target) return false;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            ClassSection* existing = findClassSection(cur->data.getMaLopHP());
            if (existing && existing->trungLichVoi(*target)) return true;
        }
        cur = cur->next;
    }
    return false;
}

inline bool SystemManager::registerClass(const string& maSV, const string& maLopHP,
                                         const string& registrationDate, bool checkSemester) {
    Student*      student = findStudent(maSV);
    ClassSection* section = findClassSection(maLopHP);
    if (!student || !section) return false;

    if (checkSemester && section->getHocKy() != currentSemester) {
        cout << "Dang ky that bai: Lop hoc phan '" << maLopHP << "' mo cho Hoc ky "
             << section->getHocKy() << ", trong khi he thong dang o Hoc ky "
             << currentSemester << " do Admin quyet dinh.\n";
        return false;
    }

    Course* course = findCourse(section->getMaMon());
    if (course) {
        if (checkSemester && course->getHocKy() != 0 && course->getHocKy() != currentSemester) {
            cout << "Dang ky that bai: Mon hoc '" << course->getTenMon()
                 << "' thuoc chuong trinh Hoc ky " << course->getHocKy()
                 << ", trong khi he thong dang o Hoc ky " << currentSemester << ".\n";
            return false;
        }
        if (!course->getKhoa().empty() && course->getKhoa() != "Chung") {
            if (student->getKhoa().empty() || student->getKhoa() != course->getKhoa()) {
                cout << "Dang ky that bai: Mon hoc thuoc Khoa '" << course->getKhoa()
                     << "' (Khoa cua sinh vien: '"
                     << (student->getKhoa().empty() ? "Chua cap nhat" : student->getKhoa()) << "').\n";
                return false;
            }
        }
        if (!course->getKhoaHoc().empty() && course->getKhoaHoc() != "Chung") {
            if (student->getKhoaHoc().empty() || student->getKhoaHoc() != course->getKhoaHoc()) {
                cout << "Dang ky that bai: Mon hoc ap dung cho Khoa hoc '" << course->getKhoaHoc()
                     << "' (Khoa hoc cua sinh vien: '"
                     << (student->getKhoaHoc().empty() ? "Chua cap nhat" : student->getKhoaHoc()) << "').\n";
                return false;
            }
        }
        if (!course->getNganhHoc().empty() && course->getNganhHoc() != "Chung") {
            if (student->getNganhHoc().empty() || student->getNganhHoc() != course->getNganhHoc()) {
                cout << "Dang ky that bai: Mon hoc ap dung cho Nganh '" << course->getNganhHoc()
                     << "' (Nganh cua sinh vien: '"
                     << (student->getNganhHoc().empty() ? "Chua cap nhat" : student->getNganhHoc()) << "').\n";
                return false;
            }
        }
    }

    if (registrationRepo.isRegistered(maSV, maLopHP)) {
        cout << "Dang ky that bai: Sinh vien da dang ky lop hoc phan nay roi.\n";
        return false;
    }

    string oldSecID = getRegisteredSectionForCourse(maSV, section->getMaMon());
    if (!oldSecID.empty() && oldSecID != maLopHP) {
        Course* c = findCourse(section->getMaMon());
        cout << "Dang ky that bai: Ban da dang ky mon '"
             << (c ? c->getTenMon() : section->getMaMon())
             << "' o lop '" << oldSecID << "'.\n";
        cout << ">> Vui long HUY lop hoc phan '" << oldSecID
             << "' truoc khi dang ky lop hoc phan moi cua mon nay!\n";
        return false;
    }

    if (isTimeConflict(maSV, maLopHP)) {
        cout << "Dang ky that bai: Trung lich hoc voi lop hoc phan da dang ky.\n";
        return false;
    }

    if (!section->conCho()) {
        Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
        if (q) {
            q->push(maSV);
            cout << "Lop da day. Sinh vien " << maSV << " duoc dua vao hang doi.\n";
        }
        return false;
    }

    const string date = registrationDate.empty() ? today() : registrationDate;
    if (date.empty() || !section->tangSoLuong()) return false;

    const string regID = registrationRepo.getRegID(maSV, maLopHP);
    registrationRepo.add(Registration(regID, maSV, maLopHP, date));
    return true;
}

inline bool SystemManager::cancelRegistration(const string& maSV, const string& maLopHP) {
    const string regID = registrationRepo.getRegID(maSV, maLopHP);
    if (!registrationRepo.findById(regID)) return false;

    ClassSection* section = findClassSection(maLopHP);
    if (section) section->giamSoLuong();

    registrationRepo.remove(regID);

    // Tu dong dang ky tu hang doi
    if (section) {
        Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
        if (q) {
            string nextSV;
            while (q->pop(nextSV)) {
                Student* s  = findStudent(nextSV);
                Course*  c  = findCourse(section->getMaMon());
                bool match  = true;
                if (s && c) {
                    if (!c->getKhoa().empty()    && c->getKhoa()    != "Chung" && s->getKhoa()    != c->getKhoa())    match = false;
                    if (!c->getKhoaHoc().empty() && c->getKhoaHoc() != "Chung" && s->getKhoaHoc() != c->getKhoaHoc()) match = false;
                    if (!c->getNganhHoc().empty()&& c->getNganhHoc()!= "Chung" && s->getNganhHoc()!= c->getNganhHoc())match = false;
                }
                if (!match) continue;
                if (!isTimeConflict(nextSV, maLopHP)) {
                    section->tangSoLuong();
                    const string newRegID = registrationRepo.getRegID(nextSV, maLopHP);
                    registrationRepo.add(Registration(newRegID, nextSV, maLopHP, today()));
                    cout << "Tu dong dang ky cho sinh vien " << nextSV
                         << " tu hang doi vao lop " << maLopHP << ".\n";
                    break;
                } else {
                    cout << "Sinh vien " << nextSV << " bi trung lich, chuyen sang ban tiep theo.\n";
                }
            }
        }
    }
    return true;
}

inline void SystemManager::displayStudentRegistrations(const string& maSV) const {
    cout << "  DANH SACH LOP DA DANG KY - SV: " << maSV << "\n";
    int count = 0;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            count++;
            cout << "[" << count << "] Lop HP: " << cur->data.getMaLopHP()
                 << " | Ngay DK: " << cur->data.getNgayDangKy() << "\n";
            ClassSection* sec =
                const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
            if (sec) {
                string tenMon = sec->getMaMon();
                Course* c = const_cast<SystemManager*>(this)->findCourse(sec->getMaMon());
                if (c) tenMon = c->getTenMon();
                cout << "    Mon: " << tenMon
                     << " | Nhom: " << (sec->getNhomLopHP().empty() ? "(Khong)" : sec->getNhomLopHP())
                     << " | Thu "   << sec->getThu()
                     << " | Tiet "  << sec->getTietBatDau() << "-" << sec->getTietKetThuc()
                     << " | GV: "   << sec->getGiangVien() << "\n";
            }
        }
        cur = cur->next;
    }
    if (count == 0) cout << "(Chua dang ky lop hoc phan nao)\n";
}

inline void SystemManager::displayAllRegistrations() const {
    registrationRepo.displayAll();
}

// =========================================================
//  TINH TOAN & THONG KE
// =========================================================

inline int SystemManager::calculateTotalCredits(const string& maSV) const {
    int totalCredits = 0;
    cout << "  TONG KET TIN CHI DA DANG KY - MSSV: " << maSV << "\n";
    int count = 0;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            ClassSection* sec =
                const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
            if (sec) {
                Course* c = const_cast<SystemManager*>(this)->findCourse(sec->getMaMon());
                int tc = (c ? c->getSoTinChi() : 0);
                totalCredits += tc;
                count++;
                cout << "[" << count << "] Lop HP: " << setw(22) << left << sec->getMaLopHP()
                     << " | Mon: " << setw(20) << (c ? c->getTenMon() : sec->getMaMon())
                     << " | So tin chi: " << tc << "\n";
            }
        }
        cur = cur->next;
    }
    cout << ">> TONG SO TIN CHI DA DANG KY: " << totalCredits
         << " tin chi (" << count << " lop hoc phan)\n";
    return totalCredits;
}

inline void SystemManager::displayStudentTimetable(const string& maSV) const {
    cout << "                   THOI KHOA BIEU SINH VIEN: " << maSV << "\n";
    cout << "                   (Hoc ky he thong: Hoc ky " << currentSemester << ")\n";
    const string days[]     = {"2","3","4","5","6","7","CN"};
    const string dayNames[] = {"THU HAI (Thu 2)","THU BA (Thu 3)","THU TU (Thu 4)",
                               "THU NAM (Thu 5)","THU SAU (Thu 6)","THU BAY (Thu 7)","CHU NHAT"};
    int totalClasses = 0;
    for (int d = 0; d < 7; d++) {
        bool hasClass = false;
        auto cur = registrationRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == maSV) {
                ClassSection* sec =
                    const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
                if (sec && sec->getThu() == days[d]) {
                    if (!hasClass) { cout << "\n>>> " << dayNames[d] << ":\n"; hasClass = true; }
                    Course* c = const_cast<SystemManager*>(this)->findCourse(sec->getMaMon());
                    cout << "  - Tiet " << setw(2) << right << sec->getTietBatDau()
                         << " -> " << setw(2) << left << sec->getTietKetThuc()
                         << " | Lop HP: " << setw(22) << sec->getMaLopHP()
                         << " | Mon: "    << setw(20) << (c ? c->getTenMon() : sec->getMaMon())
                         << " | Phong: "  << setw(8)  << sec->getPhong()
                         << " | GV: "     << sec->getGiangVien() << "\n";
                    totalClasses++;
                }
            }
            cur = cur->next;
        }
    }
    if (totalClasses == 0)
        cout << "\n(Ban chua dang ky lop hoc phan nao trong thoi khoa bieu)\n";
}

inline void SystemManager::checkTimeConflictInteractive(const string& maSV, const string& maLopHP) const {
    ClassSection* target =
        const_cast<SystemManager*>(this)->findClassSection(maLopHP);
    if (!target) { cout << "Khong tim thay lop hoc phan: " << maLopHP << "\n"; return; }
    cout << "\n--- KIEM TRA TRUNG LICH HOC ---\n";
    cout << "Lop can kiem tra: " << target->getMaLopHP()
         << " | Thu " << target->getThu()
         << " | Tiet " << target->getTietBatDau() << "-" << target->getTietKetThuc()
         << " | Phong: " << target->getPhong() << "\n";
    bool conflict = false;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            ClassSection* existing =
                const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
            if (existing && existing->trungLichVoi(*target)) {
                conflict = true;
                cout << ">> [CANH BAO] TRUNG LICH voi lop da dang ky: "
                     << existing->getMaLopHP()
                     << " (Mon: " << existing->getMaMon()
                     << ", Thu " << existing->getThu()
                     << ", Tiet " << existing->getTietBatDau() << "-" << existing->getTietKetThuc()
                     << ", Phong: " << existing->getPhong() << ")\n";
            }
        }
        cur = cur->next;
    }
    if (!conflict)
        cout << ">> [HOP LE] Lop hoc phan KHONG BI TRUNG LICH voi bat ky lop nao ban da dang ky!\n";
}

inline int SystemManager::getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const {
    Student* student = const_cast<SystemManager*>(this)->findStudent(maSV);
    if (!student) return 0;
    return courseRepo.getMatchingCourses(
        student->getKhoa(), student->getKhoaHoc(), student->getNganhHoc(),
        currentSemester, courseIDs, maxCourses);
}

inline int SystemManager::getMissingCoursesCount(const string& maSV, string missingIDs[], int maxMissing) const {
    const int MAX_REQ = 50;
    string reqIDs[MAX_REQ];
    int reqCount = getSemesterRequiredCourses(maSV, reqIDs, MAX_REQ);
    int missing = 0;
    for (int i = 0; i < reqCount; i++) {
        if (getRegisteredSectionForCourse(maSV, reqIDs[i]).empty()) {
            if (missing < maxMissing && missingIDs) missingIDs[missing] = reqIDs[i];
            missing++;
        }
    }
    return missing;
}

inline void SystemManager::displaySemesterRegistrationStatus(const string& maSV) const {
    Student* student = const_cast<SystemManager*>(this)->findStudent(maSV);
    if (!student) return;
    cout << "  RANG BUOC & TIEN DO DANG KY TAT CA MON HOC - HOC KY " << currentSemester << "\n";
    cout << "  Sinh vien: " << student->getHoTen()
         << " (MSSV: " << student->getMaSV()
         << " | Lop: "  << student->getLop() << ")\n";

    const int MAX_REQ = 50;
    string reqIDs[MAX_REQ];
    int reqCount = getSemesterRequiredCourses(maSV, reqIDs, MAX_REQ);
    if (reqCount == 0) {
        cout << "(Khong tim thay mon hoc nao duoc mo cho chuong trinh cua ban trong hoc ky "
             << currentSemester << ")\n";
        return;
    }

    int registeredCount = 0, totalCredits = 0, registeredCredits = 0;
    for (int i = 0; i < reqCount; i++) {
        Course* c = const_cast<SystemManager*>(this)->findCourse(reqIDs[i]);
        string tenMon = (c ? c->getTenMon() : reqIDs[i]);
        int tc = (c ? c->getSoTinChi() : 0);
        totalCredits += tc;
        string regSec = getRegisteredSectionForCourse(maSV, reqIDs[i]);
        cout << "[" << (i + 1) << "] [" << reqIDs[i] << "] "
             << setw(26) << left << tenMon << " (" << tc << " TC) -> ";
        if (!regSec.empty()) {
            registeredCount++;
            registeredCredits += tc;
            ClassSection* s = const_cast<SystemManager*>(this)->findClassSection(regSec);
            cout << "[DA DANG KY: " << regSec;
            if (s) cout << " | Thu " << s->getThu() << " Tiet " << s->getTietBatDau() << "-" << s->getTietKetThuc();
            cout << "]\n";
        } else {
            cout << "[CHUA DANG KY - CAN BO SUNG]\n";
        }
    }
    cout << "Tong so mon can hoc trong ky: " << reqCount << " mon (" << totalCredits << " tin chi)\n";
    cout << "So mon da dang ky:           " << registeredCount << "/" << reqCount
         << " mon (" << registeredCredits << "/" << totalCredits << " tin chi)\n";

    if (registeredCount == reqCount) {
        cout << ">> [TIEN DO HOC TAP]: HOAN THANH! Ban da dang ky DAY DU tat ca cac mon trong Hoc ky "
             << currentSemester << "!\n";
    } else {
        int missing = reqCount - registeredCount;
        cout << ">> [CANH BAO TIEN DO]: CHUA HOAN THANH! Ban con thieu " << missing
             << " mon trong Hoc ky " << currentSemester << ".\n";
        cout << ">> Cac mon con thieu:\n";
        for (int i = 0; i < reqCount; i++) {
            if (getRegisteredSectionForCourse(maSV, reqIDs[i]).empty()) {
                Course* c = const_cast<SystemManager*>(this)->findCourse(reqIDs[i]);
                cout << "   - [" << reqIDs[i] << "] "
                     << (c ? c->getTenMon() : reqIDs[i])
                     << " (" << (c ? c->getSoTinChi() : 0) << " TC)\n";
            }
        }
        cout << ">> Khuyen nghi: Ban nen dang ky bo sung day du cac mon de dam bao dung tien do chuong trinh dao tao!\n";
    }
}

inline int SystemManager::getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const {
    Student* student = const_cast<SystemManager*>(this)->findStudent(maSV);
    if (!student) return 0;

    const int MAX_ALL = 100;
    string allGroupNames[MAX_ALL];
    int allGroupCount = sectionRepo.getGroupsForSemester(currentSemester, allGroupNames, MAX_ALL);
    int count = 0;

    for (int i = 0; i < allGroupCount && count < maxGroups; i++) {
        string gName = allGroupNames[i];
        GroupInfo g;
        g.groupName = gName;
        g.hocKy = currentSemester;
        bool groupMatchesStudent = true;
        bool hasMatchingSection  = false;

        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == gName) {
                if (g.totalSections < 50) g.sectionIDs[g.totalSections++] = cur->data.getMaLopHP();
                Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
                if (c) {
                    g.totalCredits += c->getSoTinChi();
                    if (g.khoa.empty()     && !c->getKhoa().empty()     && c->getKhoa()     != "Chung") g.khoa     = c->getKhoa();
                    if (g.khoaHoc.empty()  && !c->getKhoaHoc().empty()  && c->getKhoaHoc()  != "Chung") g.khoaHoc  = c->getKhoaHoc();
                    if (g.nganhHoc.empty() && !c->getNganhHoc().empty() && c->getNganhHoc() != "Chung") g.nganhHoc = c->getNganhHoc();
                    bool mK  = c->getKhoa().empty()    || c->getKhoa()    == "Chung" || student->getKhoa().empty()    || student->getKhoa()    == c->getKhoa();
                    bool mKH = c->getKhoaHoc().empty() || c->getKhoaHoc() == "Chung" || student->getKhoaHoc().empty() || student->getKhoaHoc() == c->getKhoaHoc();
                    bool mNg = c->getNganhHoc().empty()|| c->getNganhHoc()== "Chung" || student->getNganhHoc().empty()|| student->getNganhHoc()== c->getNganhHoc();
                    if (mK && mKH && mNg) hasMatchingSection = true;
                    else                  groupMatchesStudent = false;
                }
            }
            cur = cur->next;
        }
        if (groupMatchesStudent && hasMatchingSection) eligibleGroups[count++] = g;
    }

    if (count == 0) {
        for (int i = 0; i < allGroupCount && count < maxGroups; i++) {
            string gName = allGroupNames[i];
            GroupInfo g;
            g.groupName = gName;
            g.hocKy = currentSemester;
            auto cur = sectionRepo.getList().getHead();
            while (cur != nullptr) {
                if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == gName) {
                    if (g.totalSections < 50) g.sectionIDs[g.totalSections++] = cur->data.getMaLopHP();
                    Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
                    if (c) g.totalCredits += c->getSoTinChi();
                }
                cur = cur->next;
            }
            eligibleGroups[count++] = g;
        }
    }
    return count;
}

// =========================================================
//  Accessor truc tiep (tuong thich nguoc voi code cu)
// =========================================================

inline const DoubleLinkedList<Student>& SystemManager::getStudents() const {
    return studentRepo.getList();
}

inline const DoubleLinkedList<Course>& SystemManager::getCourses() const {
    return courseRepo.getList();
}

inline const DoubleLinkedList<ClassSection>& SystemManager::getSections() const {
    return sectionRepo.getList();
}

inline const DoubleLinkedList<Registration>& SystemManager::getRegistrations() const {
    return registrationRepo.getList();
}

#endif // SYSTEM_H
