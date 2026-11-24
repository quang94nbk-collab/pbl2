#ifndef REGISTRATION_SERVICE_H
#define REGISTRATION_SERVICE_H

#include <string>
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/queue.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/dtos.h"
#include "../model/registration.h"
#include "../model/student.h"

class StudentRepository;
class CourseRepository;
class ClassSectionRepository;
class RegistrationRepository;

class RegistrationService {
private:
    StudentRepository&      studentRepo;
    CourseRepository&       courseRepo;
    ClassSectionRepository& sectionRepo;
    RegistrationRepository& registrationRepo;
    int&                    currentSemester;

    static std::string today();
    template <typename T>
    static void copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst);

public:
    RegistrationService(StudentRepository& sRepo,
                        CourseRepository& cRepo,
                        ClassSectionRepository& csRepo,
                        RegistrationRepository& rRepo,
                        int& curSem);

    // Business queries
    std::string getRegisteredSectionForCourse(const std::string& maSV, const std::string& maMon) const;
    bool isSectionRegistered(const std::string& maSV, const std::string& maLopHP) const;
    bool isStudentInWaitlist(const std::string& maSV, const std::string& maLopHP) const;
    int getStudentRegisteredSections(const std::string& maSV, std::string outSectionIDs[], int maxCount) const;
    bool isTimeConflict(const std::string& maSV, const std::string& newSectionID) const;

    // Core actions
    bool registerClass(const std::string& maSV, const std::string& maLopHP,
                       const std::string& registrationDate = "", bool checkSemester = true,
                       DoubleLinkedList<std::string>* outRemovedWaitlists = nullptr);
    bool cancelRegistration(const std::string& maSV, const std::string& maLopHP,
                            DoubleLinkedList<std::string>* outWaitlistNotices = nullptr);

    void getStudentRegistrations(const std::string& maSV,
                                 DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                 int semester = 0) const;
    void getAllRegistrations(DoubleLinkedList<Registration>& outList) const;

    // Waitlist
    bool canShowWaitlist(const std::string& maLopHP) const;
    bool hasWaitlist(const std::string& maLopHP) const;
    int  getWaitlistCount(const std::string& maLopHP) const;
    bool getWaitlistStudents(const std::string& maLopHP, DoubleLinkedList<Student>& outStudents) const;
    int  removeStudentFromCourseWaitlists(const std::string& maSV, const std::string& maMon,
                                          const std::string& exceptSectionID = "",
                                          DoubleLinkedList<std::string>* outRemovedWaitlists = nullptr);
    bool removeStudentFromWaitlist(const std::string& maSV, const std::string& maLopHP);

    // Calculations & Statistics
    int calculateTotalCredits(const std::string& maSV,
                              DoubleLinkedList<CreditSummaryItem>* outDetails = nullptr,
                              int semester = 0) const;
    void getStudentTimetable(const std::string& maSV, DoubleLinkedList<TimetableEntry>& outEntries) const;
    bool checkTimeConflictDetails(const std::string& maSV, const std::string& maLopHP,
                                  ClassSection& outTarget,
                                  DoubleLinkedList<ClassSection>& outConflicts) const;
    int getSemesterRequiredCourses(const std::string& maSV, std::string courseIDs[], int maxCourses) const;
    int getMissingCoursesCount(const std::string& maSV, std::string missingIDs[], int maxMissing) const;
    bool getSemesterRegistrationStatus(const std::string& maSV,
                                       SemesterProgressSummary& outSummary,
                                       DoubleLinkedList<CourseProgressItem>& outItems) const;
    int getEligibleGroupsForStudent(const std::string& maSV, GroupInfo eligibleGroups[], int maxGroups) const;
};

#endif // REGISTRATION_SERVICE_H
