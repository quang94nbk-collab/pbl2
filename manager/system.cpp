#include "system.h"

SystemManager::SystemManager()
    : currentSemester(1),
      courseService(courseRepo, sectionRepo),
      sectionService(sectionRepo, courseRepo, registrationRepo, studentRepo),
      registrationService(studentRepo, courseRepo, sectionRepo, registrationRepo, currentSemester),
      studentService(studentRepo, registrationRepo, sectionRepo, &registrationService) {
}

void SystemManager::linkObjectReferences() {
    // 1. Reset cac lien ket cu trong bo nho (neu co)
    auto cCur = courseRepo.getList().getHead();
    while (cCur != nullptr) {
        Course* c = courseRepo.findById(cCur->data.getMaMon());
        if (c != nullptr) c->clearDanhSachLopHP();
        cCur = cCur->next;
    }
    auto sCur = studentRepo.getList().getHead();
    while (sCur != nullptr) {
        Student* s = studentRepo.findById(sCur->data.getMaSV());
        if (s != nullptr) s->clearDangKy();
        sCur = sCur->next;
    }

    // 2. Lien ket Course <-> ClassSection
    // - ClassSection tro toi Course (Association - Huong 1)
    // - Course them ClassSection vao dsLopHocPhan (Aggregation - Huong 2)
    auto secCur = sectionRepo.getList().getHead();
    while (secCur != nullptr) {
        ClassSection* sec = sectionRepo.findById(secCur->data.getMaLopHP());
        if (sec != nullptr) {
            sec->clearDangKy();
            Course* c = courseRepo.findById(sec->getMaMon());
            if (c != nullptr) {
                sec->setMonHoc(c);
                c->themLopHocPhan(sec);
            }
        }
        secCur = secCur->next;
    }

    // 3. Lien ket Registration <-> Student va ClassSection (Huong 1.2)
    // - Registration tro truc tiep toi Student* va ClassSection* (Association Class)
    // - Student luu con tro Registration* vao danh sach cua minh (Quan he 2 chieu)
    // - ClassSection luu con tro Registration* vao danh sach cua minh (Quan he 2 chieu)
    auto regCur = registrationRepo.getList().getHead();
    while (regCur != nullptr) {
        Registration* reg = registrationRepo.findById(regCur->data.getMaDangKy());
        if (reg != nullptr) {
            Student* s = studentRepo.findById(reg->getMaSV());
            ClassSection* sec = sectionRepo.findById(reg->getMaLopHP());
            if (s != nullptr && sec != nullptr) {
                reg->setSinhVien(s);
                reg->setLopHocPhan(sec);
                s->themDangKy(reg);
                sec->themDangKy(reg);
            }
        }
        regCur = regCur->next;
    }
}

// =========================================================
//  DATA PERSISTENCE
// =========================================================

bool SystemManager::loadData(string& error) {
    bool ok = fileManager.loadAllData(studentRepo, courseRepo, sectionRepo, registrationRepo,
                                      currentSemester, error);
    if (ok) {
        linkObjectReferences();
    }
    return ok;
}

bool SystemManager::saveData(string& error) const {
    return fileManager.saveAllData(studentRepo, courseRepo, sectionRepo, registrationRepo,
                                   currentSemester, error);
}

int SystemManager::getCurrentSemester() const {
    return currentSemester;
}

bool SystemManager::setCurrentSemester(int semester) {
    if (semester >= 1 && semester <= 10) {
        currentSemester = semester;
        return true;
    }
    return false;
}

// =========================================================
//  STUDENT
// =========================================================

bool SystemManager::addStudent(const Student& s) {
    return studentService.addStudent(s);
}

bool SystemManager::deleteStudent(const string& maSV, DoubleLinkedList<string>* outWaitlistNotices) {
    return studentService.deleteStudent(maSV, outWaitlistNotices);
}

Student* SystemManager::findStudent(const string& maSV) {
    return studentService.findStudent(maSV);
}

const Student* SystemManager::findStudent(const string& maSV) const {
    return studentService.findStudent(maSV);
}

void SystemManager::getAllStudents(DoubleLinkedList<Student>& outList) const {
    studentService.getAllStudents(outList);
}

void SystemManager::searchStudents(const string& kw, DoubleLinkedList<Student>& outList) const {
    studentService.searchStudents(kw, outList);
}

// =========================================================
//  COURSE
// =========================================================

bool SystemManager::addCourse(const Course& c) {
    return courseService.addCourse(c);
}

bool SystemManager::deleteCourse(const string& maMon, string* outMessage) {
    return courseService.deleteCourse(maMon, outMessage);
}

Course* SystemManager::findCourse(const string& maMon) {
    return courseService.findCourse(maMon);
}

const Course* SystemManager::findCourse(const string& maMon) const {
    return courseService.findCourse(maMon);
}

void SystemManager::getAllCourses(DoubleLinkedList<Course>& outList) const {
    courseService.getAllCourses(outList);
}

void SystemManager::searchCourses(const string& kw, DoubleLinkedList<Course>& outList) const {
    courseService.searchCourses(kw, outList);
}

void SystemManager::filterCourses(const string& khoa, const string& khoaHoc,
                                   const string& nganh, int hocKy,
                                   DoubleLinkedList<Course>& outList) const {
    courseService.filterCourses(khoa, khoaHoc, nganh, hocKy, outList);
}

// =========================================================
//  CLASS SECTION
// =========================================================

bool SystemManager::addClassSection(const ClassSection& sec) {
    return sectionService.addClassSection(sec);
}

bool SystemManager::deleteClassSection(const string& maLopHP) {
    return sectionService.deleteClassSection(maLopHP);
}

ClassSection* SystemManager::findClassSection(const string& maLopHP) {
    return sectionService.findClassSection(maLopHP);
}

const ClassSection* SystemManager::findClassSection(const string& maLopHP) const {
    return sectionService.findClassSection(maLopHP);
}

void SystemManager::getAllSections(DoubleLinkedList<ClassSection>& outList) const {
    sectionService.getAllSections(outList);
}

void SystemManager::searchSections(const string& kw, DoubleLinkedList<ClassSection>& outList) const {
    sectionService.searchSections(kw, outList);
}

bool SystemManager::getStudentsInSection(const string& maLopHP, ClassSection& outSection,
                                          DoubleLinkedList<SectionStudentInfo>& outStudents) const {
    return sectionService.getStudentsInSection(maLopHP, outSection, outStudents);
}

int SystemManager::getGroupsForCurrentSemester(string groupNames[], int maxGroups) const {
    return sectionService.getGroupsForCurrentSemester(currentSemester, groupNames, maxGroups);
}

void SystemManager::getSectionsByGroupInCurrentSemester(const string& groupName,
                                                         DoubleLinkedList<SectionDetailInfo>& outSections) const {
    sectionService.getSectionsByGroupInCurrentSemester(currentSemester, groupName, outSections);
}

void SystemManager::getUngroupedSectionsInCurrentSemester(DoubleLinkedList<SectionDetailInfo>& outSections) const {
    sectionService.getUngroupedSectionsInCurrentSemester(currentSemester, outSections);
}

// =========================================================
//  REGISTRATION & WAITLIST & STATISTICS
// =========================================================

string SystemManager::getRegisteredSectionForCourse(const string& maSV, const string& maMon) const {
    return registrationService.getRegisteredSectionForCourse(maSV, maMon);
}

bool SystemManager::isSectionRegistered(const string& maSV, const string& maLopHP) const {
    return registrationService.isSectionRegistered(maSV, maLopHP);
}

bool SystemManager::isStudentInWaitlist(const string& maSV, const string& maLopHP) const {
    return registrationService.isStudentInWaitlist(maSV, maLopHP);
}

int SystemManager::getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const {
    return registrationService.getStudentRegisteredSections(maSV, outSectionIDs, maxCount);
}

bool SystemManager::isTimeConflict(const string& maSV, const string& newSectionID) const {
    return registrationService.isTimeConflict(maSV, newSectionID);
}

bool SystemManager::registerClass(const string& maSV, const string& maLopHP,
                                   const string& registrationDate, bool checkSemester,
                                   DoubleLinkedList<string>* outRemovedWaitlists) {
    return registrationService.registerClass(maSV, maLopHP, registrationDate, checkSemester, outRemovedWaitlists);
}

bool SystemManager::cancelRegistration(const string& maSV, const string& maLopHP,
                                        DoubleLinkedList<string>* outWaitlistNotices) {
    return registrationService.cancelRegistration(maSV, maLopHP, outWaitlistNotices);
}

void SystemManager::getStudentRegistrations(const string& maSV,
                                             DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                             int semester) const {
    registrationService.getStudentRegistrations(maSV, outDetails, semester);
}

void SystemManager::getAllRegistrations(DoubleLinkedList<Registration>& outList) const {
    registrationService.getAllRegistrations(outList);
}

bool SystemManager::canShowWaitlist(const string& maLopHP) const {
    return registrationService.canShowWaitlist(maLopHP);
}

bool SystemManager::hasWaitlist(const string& maLopHP) const {
    return registrationService.hasWaitlist(maLopHP);
}

int SystemManager::getWaitlistCount(const string& maLopHP) const {
    return registrationService.getWaitlistCount(maLopHP);
}

bool SystemManager::getWaitlistStudents(const string& maLopHP, DoubleLinkedList<Student>& outStudents) const {
    return registrationService.getWaitlistStudents(maLopHP, outStudents);
}

int SystemManager::removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                                     const string& exceptSectionID,
                                                     DoubleLinkedList<string>* outRemovedWaitlists) {
    return registrationService.removeStudentFromCourseWaitlists(maSV, maMon, exceptSectionID, outRemovedWaitlists);
}

bool SystemManager::removeStudentFromWaitlist(const string& maSV, const string& maLopHP) {
    return registrationService.removeStudentFromWaitlist(maLopHP, maSV);
}

int SystemManager::calculateTotalCredits(const string& maSV,
                                          DoubleLinkedList<CreditSummaryItem>* outDetails,
                                          int semester) const {
    return registrationService.calculateTotalCredits(maSV, outDetails, semester);
}

void SystemManager::getStudentTimetable(const string& maSV, DoubleLinkedList<TimetableEntry>& outEntries) const {
    registrationService.getStudentTimetable(maSV, outEntries);
}

bool SystemManager::checkTimeConflictDetails(const string& maSV, const string& maLopHP,
                                              ClassSection& outTarget,
                                              DoubleLinkedList<ClassSection>& outConflicts) const {
    return registrationService.checkTimeConflictDetails(maSV, maLopHP, outTarget, outConflicts);
}

int SystemManager::getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const {
    return registrationService.getSemesterRequiredCourses(maSV, courseIDs, maxCourses);
}

int SystemManager::getMissingCoursesCount(const string& maSV, string missingIDs[], int maxMissing) const {
    return registrationService.getMissingCoursesCount(maSV, missingIDs, maxMissing);
}

bool SystemManager::getSemesterRegistrationStatus(const string& maSV,
                                                   SemesterProgressSummary& outSummary,
                                                   DoubleLinkedList<CourseProgressItem>& outItems) const {
    return registrationService.getSemesterRegistrationStatus(maSV, outSummary, outItems);
}

int SystemManager::getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const {
    return registrationService.getEligibleGroupsForStudent(maSV, eligibleGroups, maxGroups);
}
