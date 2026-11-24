#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "../repositories/StudentRepository.h"
#include "../repositories/CourseRepository.h"
#include "../repositories/ClassSectionRepository.h"
#include "../repositories/RegistrationRepository.h"

using namespace std;

// ============================================================
//  FileManager - Thuoc tang Storage / Persistence (File I/O)
//  Chiu trach nhiem doc/ghi du lieu tu cac tep van ban vao cac Repository
// ============================================================
class FileManager {
private:
    const string studentFile = "students.txt";
    const string courseFile = "courses.txt";
    const string sectionFile = "sections.txt";
    const string registrationFile = "registrations.txt";
    const string semesterFile = "semester.txt";
    const string waitlistFile = "waitlist.txt";

    bool loadSemester(int& currentSemester, string& error) const;
    bool saveSemester(int currentSemester, string& error) const;
    bool loadStudents(StudentRepository& studentRepo, string& error) const;
    bool loadCourses(CourseRepository& courseRepo, string& error) const;
    bool loadSections(ClassSectionRepository& sectionRepo, const CourseRepository& courseRepo, string& error) const;
    bool loadRegistrations(RegistrationRepository& registrationRepo,
                           const StudentRepository& studentRepo,
                           ClassSectionRepository& sectionRepo,
                           string& error) const;
    bool loadWaitlists(ClassSectionRepository& sectionRepo, string& error) const;
    bool saveWaitlists(const ClassSectionRepository& sectionRepo, string& error) const;

public:
    bool loadAllData(StudentRepository& studentRepo,
                     CourseRepository& courseRepo,
                     ClassSectionRepository& sectionRepo,
                     RegistrationRepository& registrationRepo,
                     int& currentSemester,
                     string& error) const;

    bool saveAllData(const StudentRepository& studentRepo,
                     const CourseRepository& courseRepo,
                     const ClassSectionRepository& sectionRepo,
                     const RegistrationRepository& registrationRepo,
                     int currentSemester,
                     string& error) const;
};

#endif // FILEMANAGER_H
