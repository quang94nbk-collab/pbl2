#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

// Models & DTOs
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/dtos.h"
#include "../model/registration.h"
#include "../model/student.h"

// DSA
#include "../DSA/DoublyLinkedList.h"

// DAL Repositories & Storage
#include "../dal/repositories/StudentRepository.h"
#include "../dal/repositories/CourseRepository.h"
#include "../dal/repositories/ClassSectionRepository.h"
#include "../dal/repositories/RegistrationRepository.h"
#include "../dal/file/FileManager.h"

// Business Services
#include "exception.h"
#include "StudentService.h"
#include "CourseService.h"
#include "ClassSectionService.h"
#include "RegistrationService.h"

using namespace std;

// ============================================================
//  SystemManager - Facade dieu phoi toan bo he thong nghiep vu
//
//  Kien truc 3-Layer chuan:
//    UI (Console / Qt GUI)
//       â”‚
//       â–¼
//    SystemManager (Facade Pattern)
//       â”œâ”€â”€ StudentService      (Quan ly nghiep vu Sinh vien)
//       â”œâ”€â”€ CourseService       (Quan ly nghiep vu Mon hoc)
//       â”œâ”€â”€ ClassSectionService (Quan ly nghiep vu Lop hoc phan)
//       â””â”€â”€ RegistrationService (Quan ly Dang ky, Waitlist, TKB, Tien do)
//       â”‚
//       â–¼
//    DAL Repositories & FileManager
// ============================================================
class SystemManager {
private:
    StudentRepository       studentRepo;
    CourseRepository        courseRepo;
    ClassSectionRepository  sectionRepo;
    RegistrationRepository  registrationRepo;
    FileManager             fileManager;

    int currentSemester;

    CourseService           courseService;
    ClassSectionService     sectionService;
    RegistrationService     registrationService;
    StudentService          studentService;

    // Tai lap lien ket con tro thuan OOP sau khi nap du lieu tu file
    void linkObjectReferences();

public:
    SystemManager();

    // =========================================================
    //  DATA PERSISTENCE (Giao tiep voi DAL FileManager)
    // =========================================================
    bool loadData(string& error);
    bool saveData(string& error) const;

    int  getCurrentSemester() const;
    bool setCurrentSemester(int semester);
    static bool isSectionOpenInSemester(int secHocKy, int sysSemester);
    int  getStudentStandardSemester(const string& maSV) const;

    // =========================================================
    //  STUDENT SERVICE (Uy quyen cho StudentService)
    // =========================================================
    bool addStudent(const Student& s);
    bool deleteStudent(const string& maSV, DoubleLinkedList<string>* outWaitlistNotices = nullptr);
    Student* findStudent(const string& maSV);
    const Student* findStudent(const string& maSV) const;
    void getAllStudents(DoubleLinkedList<Student>& outList) const;
    void searchStudents(const string& kw, DoubleLinkedList<Student>& outList) const;

    // =========================================================
    //  COURSE SERVICE (Uy quyen cho CourseService)
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
    //  CLASS SECTION SERVICE (Uy quyen cho ClassSectionService)
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
    //  REGISTRATION SERVICE (Uy quyen cho RegistrationService)
    // =========================================================
    string getRegisteredSectionForCourse(const string& maSV, const string& maMon) const;
    bool isSectionRegistered(const string& maSV, const string& maLopHP) const;
    bool isStudentInWaitlist(const string& maSV, const string& maLopHP) const;
    int getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const;
    bool isTimeConflict(const string& maSV, const string& newSectionID) const;
    bool registerClass(const string& maSV, const string& maLopHP,
                       const string& registrationDate = "", bool checkSemester = true,
                       DoubleLinkedList<string>* outRemovedWaitlists = nullptr);
    bool cancelRegistration(const string& maSV, const string& maLopHP,
                            DoubleLinkedList<string>* outWaitlistNotices = nullptr);
    void getStudentRegistrations(const string& maSV,
                                 DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                 int semester = 0) const;
    void getAllRegistrations(DoubleLinkedList<Registration>& outList) const;

    // Waitlist
    bool canShowWaitlist(const string& maLopHP) const;
    bool hasWaitlist(const string& maLopHP) const;
    int  getWaitlistCount(const string& maLopHP) const;
    bool getWaitlistStudents(const string& maLopHP, DoubleLinkedList<Student>& outStudents) const;
    int  removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                          const string& exceptSectionID = "",
                                          DoubleLinkedList<string>* outRemovedWaitlists = nullptr);
    bool removeStudentFromWaitlist(const string& maSV, const string& maLopHP);

    // Tinh toan, thong ke & tien do hoc tap
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

    // Service accessors (danh cho mo rong khi can goi truc tiep cac service chuyen biet)
    StudentService& getStudentService() { return studentService; }
    CourseService& getCourseService() { return courseService; }
    ClassSectionService& getSectionService() { return sectionService; }
    RegistrationService& getRegistrationService() { return registrationService; }
};

#endif // SYSTEM_H
