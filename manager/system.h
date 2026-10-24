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
#include "../model/curriculum.h"
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

    DoubleLinkedList<CurriculumItem> curriculumList;

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

    // --- QUAN LY CHUONG TRINH DAO TAO ---
    bool addCurriculumItem(const CurriculumItem& item) {
        if (item.getMaKhoa().empty() || item.getMaMon().empty() ||
            !courseTable.search(item.getMaMon())) {
            return false;
        }

        auto current = curriculumList.getHead();
        while (current != nullptr) {
            if (current->data == item) {
                return false; // Đã tồn tại môn trong CTĐT của khoa này
            }
            current = current->next;
        }

        curriculumList.pushBack(item);
        return true;
    }

    bool isCourseAllowedForStudent(const string& studentID, const string& courseID) const {
        // Neu chua co du lieu CTDT, cho phep dang ky mac dinh (tuong thich nguoc)
        if (curriculumList.getHead() == nullptr) {
            return true;
        }

        NodeDLL<Student>* node = nullptr;
        if (!studentTable.get(studentID, node)) {
            return false;
        }

        string maKhoa = node->data.getMaKhoa();

        auto current = curriculumList.getHead();
        while (current != nullptr) {
            // Môn học hợp lệ nếu thuộc Khoa của SV HOẶC thuộc khoa Cơ bản/Đại cương ("100") HOẶC "ALL"
            if (current->data.getMaMon() == courseID &&
                (current->data.getMaKhoa() == maKhoa ||
                 current->data.getMaKhoa() == "100" ||
                 current->data.getMaKhoa() == "ALL")) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void displayCurriculumByFaculty(const string& maKhoa) const {
        cout << "=== CHUONG TRINH DAO TAO KHOA " << maKhoa << " ===\n";
        auto current = curriculumList.getHead();
        bool found = false;
        while (current != nullptr) {
            if (current->data.getMaKhoa() == maKhoa ||
                current->data.getMaKhoa() == "100" ||
                current->data.getMaKhoa() == "ALL") {
                found = true;
                NodeDLL<Course>* cNode = nullptr;
                string tenMon = "";
                int tinChi = 0;
                if (courseTable.get(current->data.getMaMon(), cNode)) {
                    tenMon = cNode->data.getTenMon();
                    tinChi = cNode->data.getSoTinChi();
                }
                cout << "- [Ky " << current->data.getHocKy() << "] "
                     << current->data.getMaMon() << " - " << tenMon
                     << " (" << tinChi << " TC) | "
                     << (current->data.isBatBuoc() ? "Bat buoc" : "Tu chon")
                     << " | Thuoc: " << current->data.getTenKhoa() << '\n';
            }
            current = current->next;
        }
        if (!found) {
            cout << "Chua co mon hoc nao trong chuong trinh dao tao cua khoa nay.\n";
        }
    }

    void displayCurriculumForStudent(const string& studentID) const {
        NodeDLL<Student>* node = nullptr;
        if (!studentTable.get(studentID, node)) {
            cout << "Khong tim thay sinh vien: " << studentID << '\n';
            return;
        }
        cout << "Sinh vien: " << node->data.getHoTen() << " (" << node->data.getMaSV()
             << ") - " << node->data.getTenKhoa() << '\n';
        displayCurriculumByFaculty(node->data.getMaKhoa());
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
        ClassSection* section = findClassSection(sectionID);
        if (!studentTable.search(studentID) || section == nullptr) {
            return false;
        }

        // KIEM TRA CHUONG TRINH DAO TAO KHOA
        if (!isCourseAllowedForStudent(studentID, section->getMaMon())) {
            cout << "Loi: Mon hoc " << section->getMaMon()
                 << " khong thuoc chuong trinh dao tao cua khoa ban!\n";
            return false;
        }

        const string regID = "DK_" + studentID + "_" + sectionID;

        if (registrationTable.search(regID)) {
            return false;
        }

        if (isTimeConflict(studentID, sectionID)) {
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
                    if (!isTimeConflict(nextStudent, sectionID) &&
                        isCourseAllowedForStudent(nextStudent, section->getMaMon())) {
                        section->tangSoLuong();
                        const string date = today();
                        const string newRegID = "DK_" + nextStudent + "_" + sectionID;
                        
                        registrationList.pushBack(Registration(newRegID, nextStudent, sectionID, date));
                        registrationTable.insert(newRegID, getLastNode(registrationList));
                        
                        cout << "Tu dong dang ky cho sinh vien " << nextStudent 
                             << " tu hang doi vao lop " << sectionID << ".\n";
                        break;
                    } else {
                        cout << "Sinh vien " << nextStudent << " bi trung lich hoac khong hop le, chuyen sang ban tiep theo.\n";
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
    const DoubleLinkedList<CurriculumItem>& getCurriculums() const { return curriculumList; }
};

#endif
