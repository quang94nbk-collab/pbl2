#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <cerrno>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include "system1.h"

using namespace std;

class FileManager {
private:
    const string studentFile = "students.txt";
    const string courseFile = "courses.txt";
    const string sectionFile = "sections.txt";
    const string registrationFile = "registrations.txt";
    const string semesterFile = "semester.txt";

    bool loadSemester(SystemManager& system, string& error) const {
        errno = 0;
        ifstream file(semesterFile.c_str());
        if (!file.is_open()) {
            if (errno == ENOENT) {
                return true; // Mac dinh la ki 1 neu chua co tep
            }
            error = "Khong mo duoc tep " + semesterFile;
            return false;
        }
        int sem = 1;
        if (file >> sem) {
            if (sem == 1 || sem == 2) {
                system.setCurrentSemester(sem);
            }
        }
        if (file.bad()) {
            error = "Loi khi doc tep " + semesterFile;
            return false;
        }
        return true;
    }

    bool saveSemester(const SystemManager& system, string& error) const {
        ofstream file(semesterFile.c_str());
        if (!file) {
            error = "Khong mo duoc tep " + semesterFile + " de ghi.";
            return false;
        }
        file << system.getCurrentSemester() << '\n';
        return true;
    }

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
            string khoa = "";
            string khoaHoc = "";
            if (row >> quoted(khoa) >> quoted(khoaHoc)) {
                // doc thanh cong khoa va khoaHoc
            }
            if (!system.addStudent(Student(id, name, phone, email, className, major, khoa, khoaHoc))) {
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
            string khoa = "";
            string khoaHoc = "";
            string nganh = "";
            int isBB = 1;
            int hocKy = 1;
            if (row >> quoted(khoa) >> quoted(khoaHoc)) {
                if (row >> quoted(nganh) >> isBB) {
                    if (row >> hocKy) {
                        // doc thanh cong hocKy
                    }
                } else {
                    row.clear();
                    int tempBB = 1;
                    if (row >> tempBB) {
                        isBB = tempBB;
                    }
                }
            }
            if (!system.addCourse(Course(id, name, credits, khoa, khoaHoc, nganh, isBB == 1, hocKy))) {
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
            int hocKy = 1;
            if (row >> hocKy) {
                // doc thanh cong hocKy
            }
            ClassSection section(id, courseID, instructor, day, startPeriod,
                                 endPeriod, room, capacity, hocKy);
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
            if (!system.registerClass(studentID, sectionID, date, false)) {
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
        return loadSemester(system, error) &&
               loadStudents(system, error) &&
               loadCourses(system, error) &&
               loadSections(system, error) &&
               loadRegistrations(system, error);
    }

    bool saveAllData(const SystemManager& system, string& error) const {
        if (!saveSemester(system, error)) {
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

        auto student = system.getStudents().getHead();
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

        auto course = system.getCourses().getHead();
        while (course != nullptr) {
            courses << quoted(course->data.getMaMon()) << ' '
                    << quoted(course->data.getTenMon()) << ' '
                    << course->data.getSoTinChi() << ' '
                    << quoted(course->data.getKhoa()) << ' '
                    << quoted(course->data.getKhoaHoc()) << ' '
                    << quoted(course->data.getNganhHoc()) << ' '
                    << (course->data.isBatBuoc() ? 1 : 0) << ' '
                    << course->data.getHocKy() << '\n';
            course = course->next;
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
                     << section->data.getSiSoToiDa() << ' '
                     << section->data.getHocKy() << '\n';
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

        if (!students || !courses || !sections || !registrations) {
            error = "Loi khi ghi tep du lieu.";
            return false;
        }
        return true;
    }
};

#endif
