#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h" 
#include "../DSA/queue.h"     
#include "../model/classsection.h"
#include "../model/course.h"
#include "../model/registration.h"
#include "../model/student.h"

using namespace std;

class SystemManager {
private:
    DoubleLinkedList<Student> studentList;
    HashTable<string, NodeDLL<Student>*> studentTable;

    DoubleLinkedList<Course> courseList;
    HashTable<string, NodeDLL<Course>*> courseTable;

    DoubleLinkedList<ClassSection> sectionList;
    HashTable<string, NodeDLL<ClassSection>*> sectionTable;

    DoubleLinkedList<Registration> registrationList;
    HashTable<string, NodeDLL<Registration>*> registrationTable;

    HashTable<string, Queue<string>*> waitlistTable;

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

    template <typename T>
    NodeDLL<T>* getLastNode(const DoubleLinkedList<T>& list) const {
        auto current = list.getHead();
        NodeDLL<T>* tail = nullptr;
        while (current != nullptr) {
            tail = current;
            current = current->next;
        }
        return tail;
    }

public:
    ~SystemManager() {
        auto current = sectionList.getHead();
        while (current != nullptr) {
            Queue<string>* q = nullptr;
            if (waitlistTable.get(current->data.getMaLopHP(), q)) {
                delete q;
            }
            current = current->next;
        }
    }

    bool addStudent(const Student& newStudent) {
        if (newStudent.getMaSV().empty() || studentTable.search(newStudent.getMaSV())) {
            return false;
        }
        
        studentList.pushBack(newStudent);
        studentTable.insert(newStudent.getMaSV(), getLastNode(studentList));
        return true;
    }

    bool deleteStudent(const string& studentID) {
        NodeDLL<Student>* node = nullptr;
        if (!studentTable.get(studentID, node)) {
            return false;
        }

        auto current = registrationList.getHead();
        while (current != nullptr) {
            auto next = current->next;
            if (current->data.getMaSV() == studentID) {
                cancelRegistration(studentID, current->data.getMaLopHP());
            }
            current = next;
        }

        studentList.eraseValue(node->data);
        studentTable.erase(studentID);
        return true;
    }

    Student* findStudent(const string& studentID) {
        NodeDLL<Student>* node = nullptr;
        if (studentTable.get(studentID, node)) {
            return &(node->data);
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
        if (newCourse.getMaMon().empty() || newCourse.getSoTinChi() <= 0 ||
            courseTable.search(newCourse.getMaMon())) {
            return false;
        }
        
        courseList.pushBack(newCourse);
        courseTable.insert(newCourse.getMaMon(), getLastNode(courseList));
        return true;
    }

    Course* findCourse(const string& courseID) {
        NodeDLL<Course>* node = nullptr;
        if (courseTable.get(courseID, node)) {
            return &(node->data);
        }
        return nullptr;
    }

    void displayAllCourses() const {
        auto current = courseList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            cout << "----------------------\n";
            current = current->next;
        }
    }

    void displayCoursesByKhoaKhoaHocAndNganh(const string& khoa, const string& khoaHoc, const string& nganh = "") const {
        cout << "\n======================================================\n";
        cout << "  DANH SACH MON HOC\n";
        cout << "  - Khoa:      " << (khoa.empty() ? "TAT CA" : khoa) << "\n";
        cout << "  - Khoa hoc:  " << (khoaHoc.empty() ? "TAT CA" : khoaHoc) << "\n";
        cout << "  - Nganh hoc: " << (nganh.empty() ? "TAT CA" : nganh) << "\n";
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

            if (matchKhoa && matchKhoaHoc && matchNganh && current->data.isBatBuoc()) {
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

            if (matchKhoa && matchKhoaHoc && matchNganh && !current->data.isBatBuoc()) {
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
        displayCoursesByKhoaKhoaHocAndNganh(khoa, khoaHoc, "");
    }

    bool addClassSection(const ClassSection& newSection) {
        if (newSection.getMaLopHP().empty() ||
            sectionTable.search(newSection.getMaLopHP()) ||
            !courseTable.search(newSection.getMaMon()) ||
            newSection.getSiSoToiDa() <= 0 ||
            newSection.getTietBatDau() <= 0 ||
            newSection.getTietKetThuc() <= newSection.getTietBatDau()) {
            return false;
        }
        
        sectionList.pushBack(newSection);
        sectionTable.insert(newSection.getMaLopHP(), getLastNode(sectionList));
        
        waitlistTable.insert(newSection.getMaLopHP(), new Queue<string>());
        return true;
    }

    ClassSection* findClassSection(const string& sectionID) {
        NodeDLL<ClassSection>* node = nullptr;
        if (sectionTable.get(sectionID, node)) {
            return &(node->data);
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
        Student* student = findStudent(studentID);
        ClassSection* section = findClassSection(sectionID);
        if (student == nullptr || section == nullptr) {
            return false;
        }

        Course* course = findCourse(section->getMaMon());
        if (course != nullptr) {
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
            if (!course->getNganhHoc().empty() && course->getNganhHoc() != "Chung") {
                if (student->getNganhHoc().empty() || student->getNganhHoc() != course->getNganhHoc()) {
                    cout << "Dang ky that bai: Mon hoc ap dung cho Nganh '" << course->getNganhHoc()
                         << "' (Nganh cua sinh vien: '" << (student->getNganhHoc().empty() ? "Chua cap nhat" : student->getNganhHoc()) << "').\n";
                    return false;
                }
            }
        }

        const string regID = "DK_" + studentID + "_" + sectionID;

        if (registrationTable.search(regID)) {
            cout << "Dang ky that bai: Sinh vien da dang ky lop hoc phan nay roi.\n";
            return false;
        }

        if (isTimeConflict(studentID, sectionID)) {
            cout << "Dang ky that bai: Trung lich hoc voi lop hoc phan da dang ky.\n";
            return false; 
        }

        if (!section->conCho()) {
            Queue<string>* q = nullptr;
            if (waitlistTable.get(sectionID, q)) {
                q->push(studentID);
                cout << "Lop da day. Sinh vien " << studentID << " duoc dua vao hang doi.\n";
            }
            return false; 
        }

        const string date = registrationDate.empty() ? today() : registrationDate;
        if (date.empty() || !section->tangSoLuong()) {
            return false;
        }

        registrationList.pushBack(Registration(regID, studentID, sectionID, date));
        registrationTable.insert(regID, getLastNode(registrationList));
        return true;
    }

    bool cancelRegistration(const string& studentID, const string& sectionID) {
        const string regID = "DK_" + studentID + "_" + sectionID;
        NodeDLL<Registration>* regNode = nullptr;

        if (!registrationTable.get(regID, regNode)) {
            return false;
        }

        ClassSection* section = findClassSection(sectionID);
        if (section != nullptr) {
            section->giamSoLuong();
        }
        
        registrationList.eraseNode(regNode);
        registrationTable.erase(regID);
        
        if (section != nullptr) {
            Queue<string>* q = nullptr;
            if (waitlistTable.get(sectionID, q)) {
                string nextStudent;
                while (q->pop(nextStudent)) {
                    Student* s = findStudent(nextStudent);
                    Course* c = findCourse(section->getMaMon());
                    bool match = true;
                    if (s != nullptr && c != nullptr) {
                        if (!c->getKhoa().empty() && c->getKhoa() != "Chung" && s->getKhoa() != c->getKhoa()) match = false;
                        if (!c->getKhoaHoc().empty() && c->getKhoaHoc() != "Chung" && s->getKhoaHoc() != c->getKhoaHoc()) match = false;
                        if (!c->getNganhHoc().empty() && c->getNganhHoc() != "Chung" && s->getNganhHoc() != c->getNganhHoc()) match = false;
                    }
                    if (!match) continue;

                    if (!isTimeConflict(nextStudent, sectionID)) {
                        section->tangSoLuong();
                        const string date = today();
                        const string newRegID = "DK_" + nextStudent + "_" + sectionID;
                        
                        registrationList.pushBack(Registration(newRegID, nextStudent, sectionID, date));
                        registrationTable.insert(newRegID, getLastNode(registrationList));
                        
                        cout << "Tu dong dang ky cho sinh vien " << nextStudent 
                             << " tu hang doi vao lop " << sectionID << ".\n";
                        break;
                    } else {
                        cout << "Sinh vien " << nextStudent << " bi trung lich, chuyen sang ban tiep theo.\n";
                    }
                }
            }
        }
        return true;
    }

    void displayAllRegistrations() const {
        auto current = registrationList.getHead();
        while (current != nullptr) {
            current->data.xuat();
            current = current->next;
        }
    }

    const DoubleLinkedList<Student>& getStudents() const { return studentList; }
    const DoubleLinkedList<Course>& getCourses() const { return courseList; }
    const DoubleLinkedList<ClassSection>& getSections() const { return sectionList; }
    const DoubleLinkedList<Registration>& getRegistrations() const { return registrationList; }
};

#endif