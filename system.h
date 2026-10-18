#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <iostream>
#include "DoublyLinkedList.h" 
#include "student.h"
#include "course.h"
#include "classsection.h"
#include "registration.h"

using namespace std;

class SystemManager {
private:
    DoublyLinkedList<Student> studentList;
    DoublyLinkedList<Course> courseList;
    DoublyLinkedList<ClassSection> sectionList;
    DoublyLinkedList<Registration> registrationList;

public:
    bool addStudent(const Student& newStudent) {
        if (findStudent(newStudent.getMaSV()) != nullptr) {
            cout << "Loi: Ma sinh vien da ton tai!\n";
            return false;
        }
        studentList.pushBack(newStudent);
        return true;
    }

    bool deleteStudent(const string& studentID) {
        Student* st = findStudent(studentID);
        if (st == nullptr) return false;
        return studentList.remove(*st);
    }

    Student* findStudent(const string& studentID) {
        auto current = studentList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID) {
                return &(current->data);
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

    bool addCourse(const Course& newCourse) {
        courseList.pushBack(newCourse);
        return true;
    }

    Course* findCourse(const string& courseID) {
        auto current = courseList.getHead();
        while (current != nullptr) {
            if (current->data.getMaMon() == courseID) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool addClassSection(const ClassSection& newSection) {
        sectionList.pushBack(newSection);
        return true;
    }

    ClassSection* findClassSection(const string& sectionID) {
        auto current = sectionList.getHead();
        while (current != nullptr) {
            if (current->data.getMaLopHP() == sectionID) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    // ================= NGHIỆP VỤ ĐĂNG KÝ TÍN CHỈ =================
    bool isTimeConflict(const string& studentID, const string& newSectionID) {
        ClassSection* targetSec = findClassSection(newSectionID);
        if (!targetSec) return false;

        auto currentReg = registrationList.getHead();
        while (currentReg != nullptr) {
            if (currentReg->data.getMaSV() == studentID) {
                auto currentSec = sectionList.getHead();
                while (currentSec != nullptr) {
                    if (currentSec->data.getMaLopHP() == currentReg->data.getMaLopHP()) {
                        if (currentSec->data.trungLichVoi(*targetSec)) {
                            return true;
                        }
                    }
                    currentSec = currentSec->next;
                }
            }
            currentReg = currentReg->next;
        }
        return false;
    }

    bool registerClass(const string& studentID, const string& sectionID) {
        Student* st = findStudent(studentID);
        ClassSection* sec = findClassSection(sectionID);

        if (!st || !sec) {
            cout << "Loi: Sinh vien hoac Lop hoc phan khong ton tai!\n";
            return false;
        }

        if (!sec->conCho()) {
            cout << "Loi: Lop hoc phan da het cho!\n";
            return false;
        }

        if (isTimeConflict(studentID, sectionID)) {
            cout << "Loi: Lop hoc phan bi trung lich hoc!\n";
            return false;
        }

        sec->tangSoLuong();
        string maDK = "DK_" + studentID + "_" + sectionID;
        Registration reg(maDK, studentID, sectionID, "2026-10-19");
        registrationList.pushBack(reg);

        cout << "Dang ky thanh cong!\n";
        return true;
    }

    bool cancelRegistration(const string& studentID, const string& sectionID) {
        auto current = registrationList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID && current->data.getMaLopHP() == sectionID) {
                ClassSection* sec = findClassSection(sectionID);
                if (sec) sec->giamSoLuong();
                
                registrationList.remove(current->data);
                cout << "Huy dang