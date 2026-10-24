#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <cerrno>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include "system.h"

using namespace std;

class FileManager {
private:
    const string studentFile = "students.txt";
    const string courseFile = "courses.txt";
    const string curriculumFile = "curriculums.txt";
    const string sectionFile = "sections.txt";
    const string registrationFile = "registrations.txt";

    bool loadStudents(SystemManager& system, string& error) const {
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
            istringstream row(line);
            string id, name, phone, email, className, major;
            if (!(row >> quoted(id) >> quoted(name) >> quoted(phone) >>
                  quoted(email) >> quoted(className) >> quoted(major))) {
                error = "Du lieu sinh vien khong hop le.";
                return false;
            }
            if (!system.addStudent(Student(id, name, phone, email, className, major))) {
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

    bool loadCourses(SystemManager& system, string& error) const {
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
            istringstream row(line);
            string id, name;
            int credits;
            if (!(row >> quoted(id) >> quoted(name) >> credits)) {
                error = "Du lieu mon hoc khong hop le.";
                return false;
            }
            if (!system.addCourse(Course(id, name, credits))) {
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

    bool loadCurriculums(SystemManager& system, string& error) const {
        errno = 0;
        ifstream file(curriculumFile.c_str());
        if (!file.is_open()) {
            if (errno == ENOENT) {
                return true;
            }
            error = "Khong mo duoc tep " + curriculumFile;
            return false;
        }

        string line;
        while (getline(file, line)) {
            istringstream row(line);
            string maKhoa, maMon;
            int hocKy, batBuocInt;
            if (!(row >> quoted(maKhoa) >> quoted(maMon) >> hocKy >> batBuocInt)) {
                error = "Du lieu chuong trinh dao tao khong hop le.";
                return false;
            }
            CurriculumItem item(maKhoa, maMon, hocKy, batBuocInt != 0);
            if (!system.addCurriculumItem(item)) {
                error = "Khong the nap mon trong CTDT: " + maKhoa + " - " + maMon;
                return false;
            }
        }
        if (file.bad()) {
            error = "Loi khi doc tep " + curriculumFile;
            return false;
        }
        return true;
    }

    bool loadSections(SystemManager& system, string& error) const {
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
            istringstream row(line);
            string id, courseID, instructor, day, room;
            int startPeriod, endPeriod, capacity;
            if (!(row >> quoted(id) >> quoted(courseID) >>
                  quoted(instructor) >> quoted(day) >> startPeriod >>
                  endPeriod >> quoted(room) >> capacity)) {
                error = "Du lieu lop hoc phan khong hop le.";
                return false;
            }
            ClassSection section(id, courseID, instructor, day, startPeriod,
                                 endPeriod, room, capacity);
            if (!system.addClassSection(section)) {
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

    bool loadRegistrations(SystemManager& system, string& error) const {
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
            istringstream row(line);
            string id, studentID, sectionID, date;
            if (!(row >> quoted(id) >> quoted(studentID) >>
                  quoted(sectionID) >> quoted(date))) {
                error = "Du lieu dang ky khong hop le.";
                return false;
            }
            if (!system.registerClass(studentID, sectionID, date)) {
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

public:
    bool loadAllData(SystemManager& system, string& error) const {
        return loadStudents(system, error) &&
               loadCourses(system, error) &&
               loadCurriculums(system, error) &&
               loadSections(system, error) &&
               loadRegistrations(system, error);
    }

    bool saveAllData(const SystemManager& system, string& error) const {
        ofstream students(studentFile.c_str());
        ofstream courses(courseFile.c_str());
        ofstream curriculums(curriculumFile.c_str());
        ofstream sections(sectionFile.c_str());
        ofstream registrations(registrationFile.c_str());
        if (!students || !courses || !curriculums || !sections || !registrations) {
            error = "Khong mo duoc tep du lieu de ghi.";
            return false;
        }

        auto student = system.getStudents().getHead();
        while (student != nullptr) {
            students << quoted(student->data.getMaSV()) << ' '
                     << quoted(student->data.getHoTen()) << ' '
                     << quoted(student->data.getSoDienThoai()) << ' '
                     << quoted(student->data.getEmail()) << ' '
                     << quoted(student->data.getLop()) << ' '
                     << quoted(student->data.getNganhHoc()) << '\n';
            student = student->next;
        }

        auto course = system.getCourses().getHead();
        while (course != nullptr) {
            courses << quoted(course->data.getMaMon()) << ' '
                    << quoted(course->data.getTenMon()) << ' '
                    << course->data.getSoTinChi() << '\n';
            course = course->next;
        }

        auto curItem = system.getCurriculums().getHead();
        while (curItem != nullptr) {
            curriculums << quoted(curItem->data.getMaKhoa()) << ' '
                        << quoted(curItem->data.getMaMon()) << ' '
                        << curItem->data.getHocKy() << ' '
                        << (curItem->data.isBatBuoc() ? 1 : 0) << '\n';
            curItem = curItem->next;
        }

        auto section = system.getSections().getHead();
        while (section != nullptr) {
            sections << quoted(section->data.getMaLopHP()) << ' '
                     << quoted(section->data.getMaMon()) << ' '
                     << quoted(section->data.getGiangVien()) << ' '
                     << quoted(section->data.getThu()) << ' '
                     << section->data.getTietBatDau() << ' '
                     << section->data.getTietKetThuc() << ' '
                     << quoted(section->data.getPhong()) << ' '
                     << section->data.getSiSoToiDa() << '\n';
            section = section->next;
        }

        auto registration = system.getRegistrations().getHead();
        while (registration != nullptr) {
            registrations << quoted(registration->data.getMaDangKy()) << ' '
                          << quoted(registration->data.getMaSV()) << ' '
                          << quoted(registration->data.getMaLopHP()) << ' '
                          << quoted(registration->data.getNgayDangKy()) << '\n';
            registration = registration->next;
        }

        if (!students || !courses || !curriculums || !sections || !registrations) {
            error = "Loi khi ghi tep du lieu.";
            return false;
        }
        return true;
    }
};

#endif
