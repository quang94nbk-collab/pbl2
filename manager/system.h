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
                       const string& registrationDate = "") {
        ClassSection* section = findClassSection(sectionID);
        if (findStudent(studentID) == nullptr || section == nullptr) {
            return false;
        }

        auto current = registrationList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID &&
                current->data.getMaLopHP() == sectionID) {
                return false;
            }
            current = current->next;
        }

        if (!section->conCho() || isTimeConflict(studentID, sectionID)) {
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
