#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <sstream>
#include <string>

#include "exception.h"

// DAL Repositories
#include "../dal/StudentRepository.h"
#include "../dal/CourseRepository.h"
#include "../dal/ClassSectionRepository.h"
#include "../dal/RegistrationRepository.h"
#include "../dal/filemanager.h"

// DSA
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../DSA/queue.h"

// Models (gian tiep qua DAL, nhung van can include de dung kieu du lieu)
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/dtos.h"
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
//      +-- FileManager            (DAL: File persistence)
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
    FileManager            fileManager;

    int currentSemester;

    static string today();
    static string toLower(string s);

    template <typename T>
    static void copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst);

    SectionDetailInfo buildSectionDetail(const ClassSection& sec) const;

public:
    SystemManager();

    // =========================================================
    //  DATA PERSISTENCE (Giao tiep voi DAL FileManager)
    // =========================================================
    bool loadData(string& error);
    bool saveData(string& error) const;

    int  getCurrentSemester() const;
    bool setCurrentSemester(int semester);

    // =========================================================
    //  STUDENT
    // =========================================================
    bool addStudent(const Student& s);
    bool deleteStudent(const string& maSV, DoubleLinkedList<string>* outWaitlistNotices = nullptr);
    Student* findStudent(const string& maSV);
    const Student* findStudent(const string& maSV) const;
    void getAllStudents(DoubleLinkedList<Student>& outList) const;
    void searchStudents(const string& kw, DoubleLinkedList<Student>& outList) const;

    // =========================================================
    //  COURSE
    // =========================================================
    bool addCourse(const Course& c);
    bool deleteCourse(const string& maMon, string* outMessage = nullptr);
    Course* findCourse(const string& maMon);
    const Course* findCourse(const string& maMon) const;
    void getAllCourses(DoubleLinkedList<Course>& outList) const;
    void searchCourses(const string& kw, DoubleLinkedList<Course>& outList) const;
    void filterCourses(const string& khoa, const string& khoaHoc,
                       const string& nganh, int hocKy,
                       DoubleLinkedList<Course>& outList) const;

    // =========================================================
    //  CLASS SECTION
    // =========================================================
    bool addClassSection(const ClassSection& sec);
    bool deleteClassSection(const string& maLopHP);
    ClassSection* findClassSection(const string& maLopHP);
    const ClassSection* findClassSection(const string& maLopHP) const;
    void getAllSections(DoubleLinkedList<ClassSection>& outList) const;
    void searchSections(const string& kw, DoubleLinkedList<ClassSection>& outList) const;
    bool getStudentsInSection(const string& maLopHP, ClassSection& outSection,
                              DoubleLinkedList<SectionStudentInfo>& outStudents) const;
    int getGroupsForCurrentSemester(string groupNames[], int maxGroups) const;
    void getSectionsByGroupInCurrentSemester(const string& groupName,
                                             DoubleLinkedList<SectionDetailInfo>& outSections) const;
    void getUngroupedSectionsInCurrentSemester(DoubleLinkedList<SectionDetailInfo>& outSections) const;

    // =========================================================
    //  REGISTRATION - Logic nghiep vu chinh
    // =========================================================

    // Tim maLopHP sinh vien da dang ky cho 1 mon hoc cu the (neu co)
    string getRegisteredSectionForCourse(const string& maSV, const string& maMon) const;
    bool isSectionRegistered(const string& maSV, const string& maLopHP) const;
    bool isStudentInWaitlist(const string& maSV, const string& maLopHP) const;
    int getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const;
    bool isTimeConflict(const string& maSV, const string& newSectionID) const;
    bool registerClass(const string& maSV, const string& maLopHP,
                       const string& registrationDate = "", bool checkSemester = true);
    bool cancelRegistration(const string& maSV, const string& maLopHP,
                            DoubleLinkedList<string>* outWaitlistNotices = nullptr);
    void getStudentRegistrations(const string& maSV,
                                 DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                 int semester = 0) const;
    void getAllRegistrations(DoubleLinkedList<Registration>& outList) const;

    // =========================================================
    //  TINH TOAN & THONG KE
    // =========================================================
    int calculateTotalCredits(const string& maSV,
                              DoubleLinkedList<CreditSummaryItem>* outDetails = nullptr,
                              int semester = 0) const;
    void getStudentTimetable(const string& maSV, DoubleLinkedList<TimetableEntry>& outEntries) const;
    bool checkTimeConflictDetails(const string& maSV, const string& maLopHP,
                                  ClassSection& outTarget,
                                  DoubleLinkedList<ClassSection>& outConflicts) const;
    int getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const;
    int getMissingCoursesCount(const string& maSV, string missingIDs[], int maxMissing) const;
    bool getSemesterRegistrationStatus(const string& maSV,
                                       SemesterProgressSummary& outSummary,
                                       DoubleLinkedList<CourseProgressItem>& outItems) const;
    int getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const;

    // =========================================================
    //  Accessor truc tiep (tuong thich nguoc voi code cu)
    //  -> Da chuyen sang cac ham truy van getAll... / DTOs de dam bao dong goi 3 layer
    // =========================================================
};

// =============================================================
//  IMPLEMENTATION CAC PHUONG THUC CUA SystemManager
// =============================================================

inline string SystemManager::today() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    if (localTime == nullptr) return "";
    char buf[16];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d", localTime) == 0) return "";
    return string(buf);
}

inline string SystemManager::toLower(string s) {
    return utf8ToLower(s);
}

template <typename T>
inline void SystemManager::copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst) {
    dst.clear();
    auto cur = src.getHead();
    while (cur != nullptr) {
        dst.pushBack(cur->data);
        cur = cur->next;
    }
}

inline SectionDetailInfo SystemManager::buildSectionDetail(const ClassSection& sec) const {
    SectionDetailInfo info;
    info.maLopHP = sec.getMaLopHP();
    info.maMon = sec.getMaMon();
    info.nhomLopHP = sec.getNhomLopHP();
    info.giangVien = sec.getGiangVien();
    info.thu = sec.getThu();
    info.tietBatDau = sec.getTietBatDau();
    info.tietKetThuc = sec.getTietKetThuc();
    info.phong = sec.getPhong();
    info.siSoToiDa = sec.getSiSoToiDa();
    info.soLuongDaDangKy = sec.getSoLuongDaDangKy();
    info.hocKy = sec.getHocKy();
    const Course* c = findCourse(sec.getMaMon());
    info.tenMon = c ? c->getTenMon() : sec.getMaMon();
    info.soTinChi = c ? c->getSoTinChi() : 0;
    return info;
}

inline SystemManager::SystemManager() : currentSemester(1) {}

inline bool SystemManager::loadData(string& error) {
    return fileManager.loadAllData(studentRepo, courseRepo, sectionRepo, registrationRepo,
                                   currentSemester, error);
}

inline bool SystemManager::saveData(string& error) const {
    return fileManager.saveAllData(studentRepo, courseRepo, sectionRepo, registrationRepo,
                                   currentSemester, error);
}

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

inline bool SystemManager::deleteStudent(const string& maSV,
                                         DoubleLinkedList<string>* outWaitlistNotices) {
    // 1. Xoa het dang ky cua sinh vien (can update lai so luong lop HP)
    const int MAX = 100;
    string secIDs[MAX];
    int cnt = registrationRepo.getSectionsByStudent(maSV, secIDs, MAX);
    for (int i = 0; i < cnt; i++)
        cancelRegistration(maSV, secIDs[i], outWaitlistNotices);

    // 2. Xoa sinh vien
    return studentRepo.remove(maSV);
}

inline Student* SystemManager::findStudent(const string& maSV) {
    return studentRepo.findById(maSV);
}

inline const Student* SystemManager::findStudent(const string& maSV) const {
    return studentRepo.findById(maSV);
}

inline void SystemManager::getAllStudents(DoubleLinkedList<Student>& outList) const {
    copyList(studentRepo.getList(), outList);
}

inline void SystemManager::searchStudents(const string& kw, DoubleLinkedList<Student>& outList) const {
    studentRepo.search(kw, outList);
}

// =========================================================
//  COURSE
// =========================================================

inline bool SystemManager::addCourse(const Course& c) {
    return courseRepo.add(c);
}

inline bool SystemManager::deleteCourse(const string& maMon, string* outMessage) {
    // Kiem tra con ClassSection thuoc mon nay khong
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaMon() == maMon) {
            if (outMessage != nullptr) {
                *outMessage = "Khong the xoa: Lop hoc phan '" + cur->data.getMaLopHP() +
                              "' dang thuoc mon nay. Vui long xoa lop hoc phan truoc!";
            }
            return false;
        }
        cur = cur->next;
    }
    return courseRepo.remove(maMon);
}

inline Course* SystemManager::findCourse(const string& maMon) {
    return courseRepo.findById(maMon);
}

inline const Course* SystemManager::findCourse(const string& maMon) const {
    return courseRepo.findById(maMon);
}

inline void SystemManager::getAllCourses(DoubleLinkedList<Course>& outList) const {
    copyList(courseRepo.getList(), outList);
}

inline void SystemManager::searchCourses(const string& kw, DoubleLinkedList<Course>& outList) const {
    courseRepo.search(kw, outList);
}

inline void SystemManager::filterCourses(const string& khoa, const string& khoaHoc,
                                         const string& nganh, int hocKy,
                                         DoubleLinkedList<Course>& outList) const {
    courseRepo.filter(khoa, khoaHoc, nganh, hocKy, outList);
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

inline const ClassSection* SystemManager::findClassSection(const string& maLopHP) const {
    return sectionRepo.findById(maLopHP);
}

inline void SystemManager::getAllSections(DoubleLinkedList<ClassSection>& outList) const {
    copyList(sectionRepo.getList(), outList);
}

inline void SystemManager::searchSections(const string& kw, DoubleLinkedList<ClassSection>& outList) const {
    sectionRepo.search(kw, outList);
}

inline bool SystemManager::getStudentsInSection(const string& maLopHP, ClassSection& outSection,
                                                DoubleLinkedList<SectionStudentInfo>& outStudents) const {
    outStudents.clear();
    const ClassSection* sec = findClassSection(maLopHP);
    if (!sec) return false;
    outSection = *sec;

    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP) {
            SectionStudentInfo item;
            item.maSV = cur->data.getMaSV();
            const Student* sv = findStudent(item.maSV);
            item.hoTen = sv ? sv->getHoTen() : "(Chua cap nhat)";
            item.lop = sv ? sv->getLop() : "-";
            item.ngayDangKy = cur->data.getNgayDangKy();
            outStudents.pushBack(item);
        }
        cur = cur->next;
    }
    return true;
}

inline int SystemManager::getGroupsForCurrentSemester(string groupNames[], int maxGroups) const {
    return sectionRepo.getGroupsForSemester(currentSemester, groupNames, maxGroups);
}

inline void SystemManager::getSectionsByGroupInCurrentSemester(const string& groupName,
                                                                 DoubleLinkedList<SectionDetailInfo>& outSections) const {
    outSections.clear();
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == groupName) {
            outSections.pushBack(buildSectionDetail(cur->data));
        }
        cur = cur->next;
    }
}

inline void SystemManager::getUngroupedSectionsInCurrentSemester(DoubleLinkedList<SectionDetailInfo>& outSections) const {
    outSections.clear();
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        // Chua gan nhom
        if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP().empty()) {
            outSections.pushBack(buildSectionDetail(cur->data));
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
            const ClassSection* sec = findClassSection(cur->data.getMaLopHP());
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

inline bool SystemManager::isStudentInWaitlist(const string& maSV, const string& maLopHP) const {
    const Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
    return q != nullptr && q->contains(maSV);
}

inline int SystemManager::getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const {
    return registrationRepo.getSectionsByStudent(maSV, outSectionIDs, maxCount);
}

inline bool SystemManager::isTimeConflict(const string& maSV, const string& newSectionID) const {
    const ClassSection* target = findClassSection(newSectionID);
    if (!target) return false;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* existing = findClassSection(cur->data.getMaLopHP());
            if (existing && existing->getHocKy() == target->getHocKy() &&
                existing->trungLichVoi(*target)) {
                return true;
            }
        }
        cur = cur->next;
    }
    return false;
}

inline bool SystemManager::registerClass(const string& maSV, const string& maLopHP,
                                         const string& registrationDate, bool checkSemester) {
    Student*      student = findStudent(maSV);
    ClassSection* section = findClassSection(maLopHP);

    if (!student)
        throw NotFoundException("Khong tim thay sinh vien voi ma: " + maSV);
    if (!section)
        throw NotFoundException("Khong tim thay lop hoc phan voi ma: " + maLopHP);

    if (checkSemester && section->getHocKy() != currentSemester)
        throw WrongSemesterException(
            "Lop hoc phan '" + maLopHP + "' mo cho Hoc ky " +
            to_string(section->getHocKy()) +
            ", trong khi he thong dang o Hoc ky " + to_string(currentSemester));

    Course* course = findCourse(section->getMaMon());
    if (course) {
        if (checkSemester && course->getHocKy() != 0 && course->getHocKy() != currentSemester)
            throw WrongSemesterException(
                "Mon hoc '" + course->getTenMon() + "' thuoc chuong trinh Hoc ky " +
                to_string(course->getHocKy()) +
                ", trong khi he thong dang o Hoc ky " + to_string(currentSemester));

        if (!course->getKhoa().empty() && course->getKhoa() != "Chung") {
            if (student->getKhoa().empty() || student->getKhoa() != course->getKhoa())
                throw NotEligibleException(
                    "Mon hoc thuoc Khoa '" + course->getKhoa() +
                    "' (Khoa cua sinh vien: '" +
                    (student->getKhoa().empty() ? "Chua cap nhat" : student->getKhoa()) + "')");
        }
        if (!course->getKhoaHoc().empty() && course->getKhoaHoc() != "Chung") {
            if (student->getKhoaHoc().empty() || student->getKhoaHoc() != course->getKhoaHoc())
                throw NotEligibleException(
                    "Mon hoc ap dung cho Khoa hoc '" + course->getKhoaHoc() +
                    "' (Khoa hoc cua sinh vien: '" +
                    (student->getKhoaHoc().empty() ? "Chua cap nhat" : student->getKhoaHoc()) + "')");
        }
        if (!course->getNganhHoc().empty() && course->getNganhHoc() != "Chung") {
            if (student->getNganhHoc().empty() || student->getNganhHoc() != course->getNganhHoc())
                throw NotEligibleException(
                    "Mon hoc ap dung cho Nganh '" + course->getNganhHoc() +
                    "' (Nganh cua sinh vien: '" +
                    (student->getNganhHoc().empty() ? "Chua cap nhat" : student->getNganhHoc()) + "')");
        }
    }

    if (registrationRepo.isRegistered(maSV, maLopHP))
        throw DuplicateException("Sinh vien da dang ky lop hoc phan '" + maLopHP + "' roi");

    string oldSecID = getRegisteredSectionForCourse(maSV, section->getMaMon());
    if (!oldSecID.empty() && oldSecID != maLopHP) {
        Course* c = findCourse(section->getMaMon());
        throw DuplicateException(
            "Ban da dang ky mon '" +
            (c ? c->getTenMon() : section->getMaMon()) +
            "' o lop '" + oldSecID +
            "'. Vui long HUY lop cu truoc khi dang ky lop moi");
    }

    if (isTimeConflict(maSV, maLopHP))
        throw TimeConflictException("Trung lich hoc voi mot lop hoc phan da dang ky");

    if (!section->conCho()) {
        Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
        if (q) {
            if (q->contains(maSV)) {
                throw DuplicateException(
                    "Sinh vien '" + maSV + "' da nam trong hang doi cho cua lop '" + maLopHP + "' roi");
            }
            q->push(maSV);
            throw ClassFullException(
                "Lop '" + maLopHP + "' da het cho. Sinh vien '" + maSV + "' duoc them vao hang doi cho");
        }
        throw ClassFullException("Lop '" + maLopHP + "' da het cho");
    }

    const string date = registrationDate.empty() ? today() : registrationDate;
    if (date.empty() || !section->tangSoLuong())
        throw AppException("Loi noi bo khi thuc hien dang ky");

    const string regID = registrationRepo.getRegID(maSV, maLopHP);
    registrationRepo.add(Registration(regID, maSV, maLopHP, date));
    return true;
}

inline bool SystemManager::cancelRegistration(const string& maSV, const string& maLopHP,
                                              DoubleLinkedList<string>* outWaitlistNotices) {
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
                    if (outWaitlistNotices != nullptr) {
                        outWaitlistNotices->pushBack("Tu dong dang ky cho sinh vien " + nextSV +
                                                     " tu hang doi vao lop " + maLopHP + ".");
                    }
                    break;
                } else {
                    if (outWaitlistNotices != nullptr) {
                        outWaitlistNotices->pushBack("Sinh vien " + nextSV +
                                                     " bi trung lich, chuyen sang ban tiep theo.");
                    }
                }
            }
        }
    }
    return true;
}

inline void SystemManager::getStudentRegistrations(const string& maSV,
                                                   DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                                   int semester) const {
    outDetails.clear();
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* sec = findClassSection(cur->data.getMaLopHP());
            if (sec) {
                if (semester != 0 && sec->getHocKy() != semester) {
                    cur = cur->next;
                    continue;
                }
                StudentRegistrationDetail item;
                item.maDangKy = cur->data.getMaDangKy();
                item.maLopHP = cur->data.getMaLopHP();
                item.ngayDangKy = cur->data.getNgayDangKy();
                const Course* c = findCourse(sec->getMaMon());
                item.tenMon = c ? c->getTenMon() : sec->getMaMon();
                item.nhomLopHP = sec->getNhomLopHP();
                item.thu = sec->getThu();
                item.tietBatDau = sec->getTietBatDau();
                item.tietKetThuc = sec->getTietKetThuc();
                item.giangVien = sec->getGiangVien();
                item.hocKy = sec->getHocKy();
                outDetails.pushBack(item);
            }
        }
        cur = cur->next;
    }
}

inline void SystemManager::getAllRegistrations(DoubleLinkedList<Registration>& outList) const {
    copyList(registrationRepo.getList(), outList);
}

// =========================================================
//  TINH TOAN & THONG KE
// =========================================================

inline int SystemManager::calculateTotalCredits(const string& maSV,
                                                DoubleLinkedList<CreditSummaryItem>* outDetails,
                                                int semester) const {
    if (outDetails != nullptr) outDetails->clear();
    int totalCredits = 0;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* sec = findClassSection(cur->data.getMaLopHP());
            if (sec) {
                if (semester != 0 && sec->getHocKy() != semester) {
                    cur = cur->next;
                    continue;
                }
                const Course* c = findCourse(sec->getMaMon());
                int tc = (c ? c->getSoTinChi() : 0);
                totalCredits += tc;
                if (outDetails != nullptr) {
                    CreditSummaryItem item;
                    item.maLopHP = sec->getMaLopHP();
                    item.tenMon = c ? c->getTenMon() : sec->getMaMon();
                    item.soTinChi = tc;
                    outDetails->pushBack(item);
                }
            }
        }
        cur = cur->next;
    }
    return totalCredits;
}

inline void SystemManager::getStudentTimetable(const string& maSV,
                                               DoubleLinkedList<TimetableEntry>& outEntries) const {
    outEntries.clear();
    const string days[] = {"2", "3", "4", "5", "6", "7", "CN"};
    for (int d = 0; d < 7; d++) {
        auto cur = registrationRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == maSV) {
                const ClassSection* sec = findClassSection(cur->data.getMaLopHP());
                if (sec && sec->getHocKy() == currentSemester && sec->getThu() == days[d]) {
                    const Course* c = findCourse(sec->getMaMon());
                    TimetableEntry entry;
                    entry.thu = sec->getThu();
                    entry.tietBatDau = sec->getTietBatDau();
                    entry.tietKetThuc = sec->getTietKetThuc();
                    entry.maLopHP = sec->getMaLopHP();
                    entry.tenMon = c ? c->getTenMon() : sec->getMaMon();
                    entry.phong = sec->getPhong();
                    entry.giangVien = sec->getGiangVien();
                    outEntries.pushBack(entry);
                }
            }
            cur = cur->next;
        }
    }
}

inline bool SystemManager::checkTimeConflictDetails(const string& maSV, const string& maLopHP,
                                                    ClassSection& outTarget,
                                                    DoubleLinkedList<ClassSection>& outConflicts) const {
    outConflicts.clear();
    const ClassSection* target = findClassSection(maLopHP);
    if (!target) return false;
    outTarget = *target;

    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* existing = findClassSection(cur->data.getMaLopHP());
            if (existing && existing->getHocKy() == target->getHocKy() &&
                existing->trungLichVoi(*target)) {
                outConflicts.pushBack(*existing);
            }
        }
        cur = cur->next;
    }
    return true;
}

inline int SystemManager::getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const {
    const Student* student = findStudent(maSV);
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

inline bool SystemManager::getSemesterRegistrationStatus(const string& maSV,
                                                         SemesterProgressSummary& outSummary,
                                                         DoubleLinkedList<CourseProgressItem>& outItems) const {
    outItems.clear();
    const Student* student = findStudent(maSV);
    if (!student) return false;

    outSummary.maSV = student->getMaSV();
    outSummary.hoTen = student->getHoTen();
    outSummary.lop = student->getLop();
    outSummary.currentSemester = currentSemester;
    outSummary.totalRequiredCourses = 0;
    outSummary.registeredCourses = 0;
    outSummary.totalCredits = 0;
    outSummary.registeredCredits = 0;

    const int MAX_REQ = 50;
    string reqIDs[MAX_REQ];
    int reqCount = getSemesterRequiredCourses(maSV, reqIDs, MAX_REQ);
    outSummary.totalRequiredCourses = reqCount;

    for (int i = 0; i < reqCount; i++) {
        const Course* c = findCourse(reqIDs[i]);
        CourseProgressItem item;
        item.maMon = reqIDs[i];
        item.tenMon = c ? c->getTenMon() : reqIDs[i];
        item.soTinChi = c ? c->getSoTinChi() : 0;
        outSummary.totalCredits += item.soTinChi;

        string regSec = getRegisteredSectionForCourse(maSV, reqIDs[i]);
        if (!regSec.empty()) {
            item.daDangKy = true;
            item.maLopHP = regSec;
            outSummary.registeredCourses++;
            outSummary.registeredCredits += item.soTinChi;
            const ClassSection* s = findClassSection(regSec);
            if (s) {
                item.thu = s->getThu();
                item.tietBatDau = s->getTietBatDau();
                item.tietKetThuc = s->getTietKetThuc();
            }
        }
        outItems.pushBack(item);
    }
    return true;
}

inline int SystemManager::getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const {
    const Student* student = findStudent(maSV);
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
                const Course* c = findCourse(cur->data.getMaMon());
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
                    const Course* c = findCourse(cur->data.getMaMon());
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
//  -> Da thay the bang cac phuong thuc truy van getAll... / DTO de bao dam tinh dong goi 3 layer
// =========================================================

#endif // SYSTEM_H
