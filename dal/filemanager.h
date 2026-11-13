#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <cerrno>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include "StudentRepository.h"
#include "CourseRepository.h"
#include "ClassSectionRepository.h"
#include "RegistrationRepository.h"

using namespace std;

// ============================================================
//  FileManager - Thuoc tang Data Access Layer (DAL)
//  Chiu trach nhiem doc/ghi du lieu tu cac tep van ban vao cac Repository
//  Khong phu thuoc vao tang BLL (SystemManager) hay Presentation
// ============================================================
class FileManager {
private:
    const string studentFile = "students.txt";
    const string courseFile = "courses.txt";
    const string sectionFile = "sections.txt";
    const string registrationFile = "registrations.txt";
    const string semesterFile = "semester.txt";

    bool loadSemester(int& currentSemester, string& error) const;
    bool saveSemester(int currentSemester, string& error) const;
    bool loadStudents(StudentRepository& studentRepo, string& error) const;
    bool loadCourses(CourseRepository& courseRepo, string& error) const;
    bool loadSections(ClassSectionRepository& sectionRepo, const CourseRepository& courseRepo, string& error) const;
    bool loadRegistrations(RegistrationRepository& registrationRepo,
                           const StudentRepository& studentRepo,
                           ClassSectionRepository& sectionRepo,
                           string& error) const;

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

inline bool FileManager::loadSemester(int& currentSemester, string& error) const {
    errno = 0;
    ifstream file(semesterFile.c_str());
    if (!file.is_open()) {
        if (errno == ENOENT) {
            return true;
        }
        error = "Khong mo duoc tep " + semesterFile;
        return false;
    }
    int sem = 1;
    if (file >> sem) {
        if (sem == 1 || sem == 2) {
            currentSemester = sem;
        }
    }
    if (file.bad()) {
        error = "Loi khi doc tep " + semesterFile;
        return false;
    }
    return true;
}

inline bool FileManager::saveSemester(int currentSemester, string& error) const {
    ofstream file(semesterFile.c_str());
    if (!file) {
        error = "Khong mo duoc tep " + semesterFile + " de ghi.";
        return false;
    }
    file << currentSemester << '\n';
    return true;
}

inline bool FileManager::loadStudents(StudentRepository& studentRepo, string& error) const {
    errno = 0;
    ifstream file(studentFile.c_str());
    if (!file.is_open()) {
        if (errno == ENOENT) {
            return true;
        }
        error = "Khong mo duoc tep " + studentFile;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) continue;
        istringstream row(line);
        string id, name, phone, email, className, major;
        if (!(row >> quoted(id) >> quoted(name) >> quoted(phone) >>
              quoted(email) >> quoted(className) >> quoted(major))) {
            error = "Du lieu sinh vien khong hop le.";
            return false;
        }
        string khoa = "";
        string khoaHoc = "";
        if (row >> quoted(khoa) >> quoted(khoaHoc)) {
        }
        if (!studentRepo.add(Student(id, name, phone, email, className, major, khoa, khoaHoc))) {
            error = "Khong the nap sinh vien: " + id;
            return false;
        }
    }
    if (file.bad()) {
        error = "Loi khi doc tep " + studentFile;
        return false;
    }
    return true;
}

inline bool FileManager::loadCourses(CourseRepository& courseRepo, string& error) const {
    errno = 0;
    ifstream file(courseFile.c_str());
    if (!file.is_open()) {
        if (errno == ENOENT) {
            return true;
        }
        error = "Khong mo duoc tep " + courseFile;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) continue;
        istringstream row(line);
        string id, name;
        int credits;
        if (!(row >> quoted(id) >> quoted(name) >> credits)) {
            error = "Du lieu mon hoc khong hop le.";
            return false;
        }
        string khoa = "";
        string khoaHoc = "";
        string nganh = "";
        int hocKy = 1;
        if (row >> quoted(khoa) >> quoted(khoaHoc)) {
            if (row >> quoted(nganh)) {
                int ignoredBB = 1;
                if (row >> ignoredBB) {
                    if (row >> hocKy) {
                    }
                }
            }
        }
        if (!courseRepo.add(Course(id, name, credits, khoa, khoaHoc, nganh, hocKy))) {
            error = "Khong the nap mon hoc: " + id;
            return false;
        }
    }
    if (file.bad()) {
        error = "Loi khi doc tep " + courseFile;
        return false;
    }
    return true;
}

inline bool FileManager::loadSections(ClassSectionRepository& sectionRepo,
                                      const CourseRepository& courseRepo,
                                      string& error) const {
    errno = 0;
    ifstream file(sectionFile.c_str());
    if (!file.is_open()) {
        if (errno == ENOENT) {
            return true;
        }
        error = "Khong mo duoc tep " + sectionFile;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) continue;
        istringstream row(line);
        string id, courseID, instructor, day, room;
        int startPeriod, endPeriod, capacity;
        if (!(row >> quoted(id) >> quoted(courseID) >>
              quoted(instructor) >> quoted(day) >> startPeriod >>
              endPeriod >> quoted(room) >> capacity)) {
            error = "Du lieu lop hoc phan khong hop le.";
            return false;
        }
        int hocKy = 1;
        string nhomLopHP = "";
        if (row >> hocKy) {
            if (row >> quoted(nhomLopHP)) {
            }
        }
        ClassSection section(id, courseID, instructor, day, startPeriod,
                             endPeriod, room, capacity, hocKy, nhomLopHP);
        if (!courseRepo.exists(courseID) || !sectionRepo.add(section)) {
            error = "Khong the nap lop hoc phan: " + id;
            return false;
        }
    }
    if (file.bad()) {
        error = "Loi khi doc tep " + sectionFile;
        return false;
    }
    return true;
}

inline bool FileManager::loadRegistrations(RegistrationRepository& registrationRepo,
                                           const StudentRepository& studentRepo,
                                           ClassSectionRepository& sectionRepo,
                                           string& error) const {
    errno = 0;
    ifstream file(registrationFile.c_str());
    if (!file.is_open()) {
        if (errno == ENOENT) {
            return true;
        }
        error = "Khong mo duoc tep " + registrationFile;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) continue;
        istringstream row(line);
        string id, studentID, sectionID, date;
        if (!(row >> quoted(id) >> quoted(studentID) >>
              quoted(sectionID) >> quoted(date))) {
            error = "Du lieu dang ky khong hop le.";
            return false;
        }
        const Student* student = studentRepo.findById(studentID);
        ClassSection* section = sectionRepo.findById(sectionID);
        if (student == nullptr || section == nullptr ||
            registrationRepo.isRegistered(studentID, sectionID) ||
            !section->tangSoLuong()) {
            error = "Khong the nap dang ky: " + id;
            return false;
        }
        const string regID = id.empty() ? registrationRepo.getRegID(studentID, sectionID) : id;
        if (!registrationRepo.add(Registration(regID, studentID, sectionID, date))) {
            section->giamSoLuong();
            error = "Khong the nap dang ky: " + id;
            return false;
        }
    }
    if (file.bad()) {
        error = "Loi khi doc tep " + registrationFile;
        return false;
    }
    return true;
}

inline bool FileManager::loadAllData(StudentRepository& studentRepo,
                                     CourseRepository& courseRepo,
                                     ClassSectionRepository& sectionRepo,
                                     RegistrationRepository& registrationRepo,
                                     int& currentSemester,
                                     string& error) const {
    return loadSemester(currentSemester, error) &&
           loadStudents(studentRepo, error) &&
           loadCourses(courseRepo, error) &&
           loadSections(sectionRepo, courseRepo, error) &&
           loadRegistrations(registrationRepo, studentRepo, sectionRepo, error);
}

inline bool FileManager::saveAllData(const StudentRepository& studentRepo,
                                     const CourseRepository& courseRepo,
                                     const ClassSectionRepository& sectionRepo,
                                     const RegistrationRepository& registrationRepo,
                                     int currentSemester,
                                     string& error) const {
    if (!saveSemester(currentSemester, error)) {
        return false;
    }

    ofstream students(studentFile.c_str());
    ofstream courses(courseFile.c_str());
    ofstream sections(sectionFile.c_str());
    ofstream registrations(registrationFile.c_str());
    if (!students || !courses || !sections || !registrations) {
        error = "Khong mo duoc tep du lieu de ghi.";
        return false;
    }

    auto student = studentRepo.getList().getHead();
    while (student != nullptr) {
        students << quoted(student->data.getMaSV()) << ' '
                 << quoted(student->data.getHoTen()) << ' '
                 << quoted(student->data.getSoDienThoai()) << ' '
                 << quoted(student->data.getEmail()) << ' '
                 << quoted(student->data.getLop()) << ' '
                 << quoted(student->data.getNganhHoc()) << ' '
                 << quoted(student->data.getKhoa()) << ' '
                 << quoted(student->data.getKhoaHoc()) << '\n';
        student = student->next;
    }

    auto course = courseRepo.getList().getHead();
    while (course != nullptr) {
        courses << quoted(course->data.getMaMon()) << ' '
                << quoted(course->data.getTenMon()) << ' '
                << course->data.getSoTinChi() << ' '
                << quoted(course->data.getKhoa()) << ' '
                << quoted(course->data.getKhoaHoc()) << ' '
                << quoted(course->data.getNganhHoc()) << ' '
                << 0 << ' '
                << course->data.getHocKy() << '\n';
        course = course->next;
    }

    auto section = sectionRepo.getList().getHead();
    while (section != nullptr) {
        sections << quoted(section->data.getMaLopHP()) << ' '
                 << quoted(section->data.getMaMon()) << ' '
                 << quoted(section->data.getGiangVien()) << ' '
                 << quoted(section->data.getThu()) << ' '
                 << section->data.getTietBatDau() << ' '
                 << section->data.getTietKetThuc() << ' '
                 << quoted(section->data.getPhong()) << ' '
                 << section->data.getSiSoToiDa() << ' '
                 << section->data.getHocKy() << ' '
                 << quoted(section->data.getNhomLopHP()) << '\n';
        section = section->next;
    }

    auto registration = registrationRepo.getList().getHead();
    while (registration != nullptr) {
        registrations << quoted(registration->data.getMaDangKy()) << ' '
                      << quoted(registration->data.getMaSV()) << ' '
                      << quoted(registration->data.getMaLopHP()) << ' '
                      << quoted(registration->data.getNgayDangKy()) << '\n';
        registration = registration->next;
    }

    if (!students || !courses || !sections || !registrations) {
        error = "Loi khi ghi tep du lieu.";
        return false;
    }
    return true;
}

#endif
