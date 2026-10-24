#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../DSA/DoublyLinkedList.h"
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/registration.h"
#include "../model/student.h"

using namespace std;

class SystemManager {
private:
    DoubleLinkedList<Student> studentList;
    DoubleLinkedList<Course> courseList;
    DoubleLinkedList<ClassSection> sectionList;
    DoubleLinkedList<Registration> registrationList;
    int currentSemester; // Ki hoc do Admin quyet dinh (1 hoac 2)

    static string today() {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        if (localTime == nullptr) {
            return "";
        }

        ostringstream date;
        date << put_time(localTime, "%Y-%m-%d");
        return date.str();
    }

public:
    SystemManager() : currentSemester(1) {}

    int getCurrentSemester() const { return currentSemester; }

    bool setCurrentSemester(int semester) {
        if (semester == 1 || semester == 2) {
            currentSemester = semester;
            return true;
        }
        return false;
    }

    bool addStudent(const Student& newStudent) {
        if (newStudent.getMaSV().empty() || findStudent(newStudent.getMaSV()) != nullptr) {
            return false;
        }
        studentList.pushBack(newStudent);
        return true;
    }

    bool deleteStudent(const string& studentID) {
        Student* student = findStudent(studentID);
        if (student == nullptr) {
            return false;
        }

        auto current = registrationList.getHead();
        while (current != nullptr) {
            auto next = current->next;
            if (current->data.getMaSV() == studentID) {
                ClassSection* section = findClassSection(current->data.getMaLopHP());
                if (section != nullptr) {
                    section->giamSoLuong();
                }
                registrationList.eraseNode(current);
            }
            current = next;
        }

        studentList.eraseValue(*student);
        return true;
    }

    Student* findStudent(const string& studentID) {
        auto current = studentList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID) {
                return &current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    void displayAllStudents() const {
        auto current = studentList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            cout << "----------------------\n";
            current = current->next;
        }
    }

    void displayAllCourses() const {
        auto current = courseList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            cout << "----------------------\n";
            current = current->next;
        }
    }

    void displayCoursesByKhoaKhoaHocAndNganh(const string& khoa, const string& khoaHoc, const string& nganh = "", int hocKy = 0) const {
        cout << "\n======================================================\n";
        cout << "  DANH SACH MON HOC\n";
        cout << "  - Khoa:      " << (khoa.empty() ? "TAT CA" : khoa) << "\n";
        cout << "  - Khoa hoc:  " << (khoaHoc.empty() ? "TAT CA" : khoaHoc) << "\n";
        cout << "  - Nganh hoc: " << (nganh.empty() ? "TAT CA" : nganh) << "\n";
        cout << "  - Hoc ky:    " << (hocKy == 0 ? "TAT CA (1 va 2)" : to_string(hocKy)) << "\n";
        cout << "======================================================\n";

        cout << "\n>>> [MON BAT BUOC]:\n";
        int countBB = 0;
        auto current = courseList.getHead();
        while (current != nullptr) {
            bool matchKhoa = khoa.empty() || current->data.getKhoa().empty() ||
                             current->data.getKhoa() == "Chung" || current->data.getKhoa() == khoa;
            bool matchKhoaHoc = khoaHoc.empty() || current->data.getKhoaHoc().empty() ||
                                current->data.getKhoaHoc() == "Chung" || current->data.getKhoaHoc() == khoaHoc;
            bool matchNganh = nganh.empty() || current->data.getNganhHoc().empty() ||
                              current->data.getNganhHoc() == "Chung" || current->data.getNganhHoc() == nganh;
            bool matchHocKy = (hocKy == 0) || (current->data.getHocKy() == hocKy);

            if (matchKhoa && matchKhoaHoc && matchNganh && matchHocKy && current->data.isBatBuoc()) {
                countBB++;
                cout << "[" << countBB << "]\n";
                current->data.xuat();
                cout << "----------------------\n";
            }
            current = current->next;
        }
        if (countBB == 0) {
            cout << "(Khong co mon bat buoc phu hop)\n";
        }

        cout << "\n>>> [MON TU CHON]:\n";
        int countTC = 0;
        current = courseList.getHead();
        while (current != nullptr) {
            bool matchKhoa = khoa.empty() || current->data.getKhoa().empty() ||
                             current->data.getKhoa() == "Chung" || current->data.getKhoa() == khoa;
            bool matchKhoaHoc = khoaHoc.empty() || current->data.getKhoaHoc().empty() ||
                                current->data.getKhoaHoc() == "Chung" || current->data.getKhoaHoc() == khoaHoc;
            bool matchNganh = nganh.empty() || current->data.getNganhHoc().empty() ||
                              current->data.getNganhHoc() == "Chung" || current->data.getNganhHoc() == nganh;
            bool matchHocKy = (hocKy == 0) || (current->data.getHocKy() == hocKy);

            if (matchKhoa && matchKhoaHoc && matchNganh && matchHocKy && !current->data.isBatBuoc()) {
                countTC++;
                cout << "[" << countTC << "]\n";
                current->data.xuat();
                cout << "----------------------\n";
            }
            current = current->next;
        }
        if (countTC == 0) {
            cout << "(Khong co mon tu chon phu hop)\n";
        }
        cout << "======================================================\n";
    }

    void displayCoursesByKhoaAndKhoaHoc(const string& khoa, const string& khoaHoc) const {
        displayCoursesByKhoaKhoaHocAndNganh(khoa, khoaHoc, "", 0);
    }

    bool addCourse(const Course& newCourse) {
        if (newCourse.getMaMon().empty() || newCourse.getSoTinChi() <= 0 ||
            findCourse(newCourse.getMaMon()) != nullptr) {
            return false;
        }
        courseList.pushBack(newCourse);
        return true;
    }

    Course* findCourse(const string& courseID) {
        auto current = courseList.getHead();
        while (current != nullptr) {
            if (current->data.getMaMon() == courseID) {
                return &current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool addClassSection(const ClassSection& newSection) {
        if (newSection.getMaLopHP().empty() ||
            findClassSection(newSection.getMaLopHP()) != nullptr ||
            findCourse(newSection.getMaMon()) == nullptr ||
            newSection.getSiSoToiDa() <= 0 ||
            newSection.getTietBatDau() <= 0 ||
            newSection.getTietKetThuc() <= newSection.getTietBatDau()) {
            return false;
        }
        sectionList.pushBack(newSection);
        return true;
    }

    ClassSection* findClassSection(const string& sectionID) {
        auto current = sectionList.getHead();
        while (current != nullptr) {
            if (current->data.getMaLopHP() == sectionID) {
                return &current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    void displayAllSections() const {
        auto current = sectionList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            cout << "----------------------\n";
            current = current->next;
        }
    }

    void displayAllRegistrations() const {
        auto current = registrationList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            current = current->next;
        }
    }

    bool isTimeConflict(const string& studentID, const string& newSectionID) {
        ClassSection* targetSection = findClassSection(newSectionID);
        if (targetSection == nullptr) {
            return false;
        }

        auto currentRegistration = registrationList.getHead();
        while (currentRegistration != nullptr) {
            if (currentRegistration->data.getMaSV() == studentID) {
                ClassSection* existingSection =
                    findClassSection(currentRegistration->data.getMaLopHP());
                if (existingSection != nullptr &&
                    existingSection->trungLichVoi(*targetSection)) {
                    return true;
                }
            }
            currentRegistration = currentRegistration->next;
        }
        return false;
    }

    bool registerClass(const string& studentID, const string& sectionID,
                       const string& registrationDate = "", bool checkSemester = true) {
        Student* student = findStudent(studentID);
        ClassSection* section = findClassSection(sectionID);
        if (student == nullptr || section == nullptr) {
            return false;
        }

        if (checkSemester && section->getHocKy() != currentSemester) {
            cout << "Dang ky that bai: Lop hoc phan '" << sectionID << "' mo cho Hoc ky "
                 << section->getHocKy() << ", trong khi he thong dang o Hoc ky "
                 << currentSemester << " do Admin quyet dinh.\n";
            return false;
        }

        Course* course = findCourse(section->getMaMon());
        if (course != nullptr) {
            if (checkSemester && course->getHocKy() != 0 && course->getHocKy() != currentSemester) {
                cout << "Dang ky that bai: Mon hoc '" << course->getTenMon() << "' thuoc chuong trinh Hoc ky "
                     << course->getHocKy() << ", trong khi he thong dang o Hoc ky "
                     << currentSemester << " do Admin quyet dinh.\n";
                return false;
            }
            if (!course->getKhoa().empty() && course->getKhoa() != "Chung") {
                if (student->getKhoa().empty() || student->getKhoa() != course->getKhoa()) {
                    cout << "Dang ky that bai: Mon hoc thuoc Khoa '" << course->getKhoa()
                         << "' (Khoa cua sinh vien: '" << (student->getKhoa().empty() ? "Chua cap nhat" : student->getKhoa()) << "').\n";
                    return false;
                }
            }
            if (!course->getKhoaHoc().empty() && course->getKhoaHoc() != "Chung") {
                if (student->getKhoaHoc().empty() || student->getKhoaHoc() != course->getKhoaHoc()) {
                    cout << "Dang ky that bai: Mon hoc ap dung cho Khoa hoc '" << course->getKhoaHoc()
                         << "' (Khoa hoc cua sinh vien: '" << (student->getKhoaHoc().empty() ? "Chua cap nhat" : student->getKhoaHoc()) << "').\n";
                    return false;
                }
            }
        }

        auto current = registrationList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID &&
                current->data.getMaLopHP() == sectionID) {
                cout << "Dang ky that bai: Sinh vien da dang ky lop hoc phan nay roi.\n";
                return false;
            }
            current = current->next;
        }

        if (isTimeConflict(studentID, sectionID)) {
            cout << "Dang ky that bai: Trung lich hoc voi lop hoc phan da dang ky.\n";
            return false;
        }

        if (!section->conCho()) {
            cout << "Dang ky that bai: Lop hoc phan da day.\n";
            return false;
        }

        const string date = registrationDate.empty() ? today() : registrationDate;
        if (date.empty() || !section->tangSoLuong()) {
            return false;
        }

        const string registrationID = "DK_" + studentID + "_" + sectionID;
        registrationList.pushBack(
            Registration(registrationID, studentID, sectionID, date));
        return true;
    }

    bool cancelRegistration(const string& studentID, const string& sectionID) {
        auto current = registrationList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID &&
                current->data.getMaLopHP() == sectionID) {
                ClassSection* section = findClassSection(sectionID);
                if (section != nullptr) {
                    section->giamSoLuong();
                }
                registrationList.eraseNode(current);
                return true;
            }
            current = current->next;
        }
        return false;
    }

    const DoubleLinkedList<Student>& getStudents() const { return studentList; }
    const DoubleLinkedList<Course>& getCourses() const { return courseList; }
    const DoubleLinkedList<ClassSection>& getSections() const { return sectionList; }
    const DoubleLinkedList<Registration>& getRegistrations() const {
        return registrationList;
    }
};

#endif
