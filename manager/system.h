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

struct GroupInfo {
    string groupName;
    string khoa;
    string khoaHoc;
    string nganhHoc;
    int hocKy;
    int totalSections;
    int totalCredits;
    string sectionIDs[50];

    GroupInfo() : groupName(""), khoa(""), khoaHoc(""), nganhHoc(""),
                  hocKy(1), totalSections(0), totalCredits(0) {}
};

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
    int currentSemester;

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

    static string toLower(string s) {
        for (char &c : s) {
            if (c >= 'A' && c <= 'Z') c = c + 32;
        }
        return s;
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

    void searchStudents(const string& keyword) const {
        string kw = toLower(keyword);
        cout << "  KET QUA TIM KIEM SINH VIEN (Tu khoa: \"" << keyword << "\")\n";
        int count = 0;
        auto current = studentList.getHead();
        while (current != nullptr) {
            string id = toLower(current->data.getMaSV());
            string name = toLower(current->data.getHoTen());
            string lop = toLower(current->data.getLop());
            string khoa = toLower(current->data.getKhoa());
            if (id.find(kw) != string::npos || name.find(kw) != string::npos ||
                lop.find(kw) != string::npos || khoa.find(kw) != string::npos) {
                count++;
                current->data.xuat();
            }
            current = current->next;
        }
        if (count == 0) {
            cout << "(Khong tim thay sinh vien nao phu hop)\n";
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
                return false; // Ðã t?n t?i môn trong CTÐT c?a khoa này
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
            // Môn h?c h?p l? n?u thu?c Khoa c?a SV HO?C thu?c khoa Co b?n/Ð?i cuong ("100") HO?C "ALL"
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

    // Hien thi mon hoc theo Khoa, Khoa hoc, Nganh & Ki hoc (khong phan biet bat buoc/tu chon)
    void displayCoursesByFilter(const string& khoa, const string& khoaHoc, const string& nganh = "", int hocKy = 0) const {
        cout << "  DANH SACH MON HOC\n";
        cout << "  - Khoa:      " << (khoa.empty() ? "TAT CA" : khoa) << "\n";
        cout << "  - Khoa hoc:  " << (khoaHoc.empty() ? "TAT CA" : khoaHoc) << "\n";
        cout << "  - Nganh hoc: " << (nganh.empty() ? "TAT CA" : nganh) << "\n";
        cout << "  - Hoc ky:    " << (hocKy == 0 ? "TAT CA (1 va 2)" : to_string(hocKy)) << "\n";
        int count = 0;
        auto current = courseList.getHead();
        while (current != nullptr) {
            bool matchKhoa = khoa.empty() || current->data.getKhoa().empty() ||
                             current->data.getKhoa() == "Chung" || current->data.getKhoa() == khoa;
            bool matchKhoaHoc = khoaHoc.empty() || current->data.getKhoaHoc().empty() ||
                                current->data.getKhoaHoc() == "Chung" || current->data.getKhoaHoc() == khoaHoc;
            bool matchNganh = nganh.empty() || current->data.getNganhHoc().empty() ||
                              current->data.getNganhHoc() == "Chung" || current->data.getNganhHoc() == nganh;
            bool matchHocKy = (hocKy == 0) || (current->data.getHocKy() == hocKy);

            if (matchKhoa && matchKhoaHoc && matchNganh && matchHocKy) {
                count++;
                current->data.xuat();
            }
            current = current->next;
        }
        if (count == 0) {
            cout << "(Khong co mon hoc phu hop)\n";
        }
    }

    bool deleteCourse(const string& courseID) {
        NodeDLL<Course>* node = nullptr;
        if (!courseTable.get(courseID, node)) {
            return false;
        }
        auto curSec = sectionList.getHead();
        while (curSec != nullptr) {
            if (curSec->data.getMaMon() == courseID) {
                cout << "Khong the xoa: Lop hoc phan '" << curSec->data.getMaLopHP()
                     << "' dang thuoc mon nay. Vui long xoa lop hoc phan truoc!\n";
                return false;
            }
            curSec = curSec->next;
        }

        courseList.eraseNode(node);
        courseTable.erase(courseID);
        return true;
    }

    void searchCourses(const string& keyword) const {
        string kw = toLower(keyword);
        cout << "  KET QUA TIM KIEM MON HOC (Tu khoa: \"" << keyword << "\")\n";
        int count = 0;
        auto current = courseList.getHead();
        while (current != nullptr) {
            string id = toLower(current->data.getMaMon());
            string name = toLower(current->data.getTenMon());
            string khoa = toLower(current->data.getKhoa());
            string nganh = toLower(current->data.getNganhHoc());
            if (id.find(kw) != string::npos || name.find(kw) != string::npos ||
                khoa.find(kw) != string::npos || nganh.find(kw) != string::npos) {
                count++;
                current->data.xuat();
            }
            current = current->next;
        }
        if (count == 0) {
            cout << "(Khong tim thay mon hoc nao phu hop)\n";
        }
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

    bool deleteClassSection(const string& sectionID) {
        NodeDLL<ClassSection>* node = nullptr;
        if (!sectionTable.get(sectionID, node)) {
            return false;
        }
        auto curReg = registrationList.getHead();
        while (curReg != nullptr) {
            auto nextReg = curReg->next;
            if (curReg->data.getMaLopHP() == sectionID) {
                registrationTable.erase(curReg->data.getMaDangKy());
                registrationList.eraseNode(curReg);
            }
            curReg = nextReg;
        }
        Queue<string>* q = nullptr;
        if (waitlistTable.get(sectionID, q)) {
            delete q;
            waitlistTable.erase(sectionID);
        }

        sectionList.eraseNode(node);
        sectionTable.erase(sectionID);
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
            current = current->next;
        }
    }

    void searchSections(const string& keyword) const {
        string kw = toLower(keyword);
        cout << "  KET QUA TIM KIEM LOP HOC PHAN (Tu khoa: \"" << keyword << "\")\n";
        int count = 0;
        auto current = sectionList.getHead();
        while (current != nullptr) {
            string id = toLower(current->data.getMaLopHP());
            string mon = toLower(current->data.getMaMon());
            string nhom = toLower(current->data.getNhomLopHP());
            string gv = toLower(current->data.getGiangVien());
            if (id.find(kw) != string::npos || mon.find(kw) != string::npos ||
                nhom.find(kw) != string::npos || gv.find(kw) != string::npos) {
                count++;
                current->data.xuat();
            }
            current = current->next;
        }
        if (count == 0) {
            cout << "(Khong tim thay lop hoc phan nao phu hop)\n";
        }
    }

    void displayStudentsInSection(const string& sectionID) {
        ClassSection* sec = findClassSection(sectionID);
        if (sec == nullptr) {
            cout << "Khong tim thay lop hoc phan: " << sectionID << "\n";
            return;
        }
        cout << "  DANH SACH SINH VIEN DANG KY LOP HOC PHAN: " << sectionID << "\n";
        cout << "  Mon: " << sec->getMaMon() << " | Nhom: " << (sec->getNhomLopHP().empty() ? "(Chua gan nhom)" : sec->getNhomLopHP())
             << " | Thu: " << sec->getThu() << " | Tiet: " << sec->getTietBatDau() << "-" << sec->getTietKetThuc() << "\n";
        cout << "  Si so hien tai: " << sec->getSoLuongDaDangKy() << "/" << sec->getSiSoToiDa() << "\n";
        int count = 0;
        auto cur = registrationList.getHead();
        while (cur != nullptr) {
            if (cur->data.getMaLopHP() == sectionID) {
                count++;
                string svID = cur->data.getMaSV();
                Student* sv = findStudent(svID);
                cout << left << setw(4) << count 
                     << "MSSV: " << setw(12) << svID
                     << " | Ho ten: " << setw(22) << (sv ? sv->getHoTen() : "(Chua cap nhat)")
                     << " | Lop: " << setw(10) << (sv ? sv->getLop() : "-")
                     << " | Ngay DK: " << cur->data.getNgayDangKy() << "\n";
            }
            cur = cur->next;
        }
        if (count == 0) {
            cout << "(Chua co sinh vien nao dang ky vao lop hoc phan nay)\n";
        }
    }
    // Thu thap tat ca cac nhom lop HP duy nhat trong ki hoc hien tai
    // Tra ve so nhom tim duoc, luu ten nhom vao mang groupNames (toi da maxGroups)
    int getGroupsForSemester(string groupNames[], int maxGroups) const {
        int count = 0;
        auto current = sectionList.getHead();
        while (current != nullptr) {
            if (current->data.getHocKy() == currentSemester &&
                !current->data.getNhomLopHP().empty()) {
                string nhom = current->data.getNhomLopHP();
                bool found = false;
                for (int i = 0; i < count; i++) {
                    if (groupNames[i] == nhom) {
                        found = true;
                        break;
                    }
                }
                if (!found && count < maxGroups) {
                    groupNames[count] = nhom;
                    count++;
                }
            }
            current = current->next;
        }
        return count;
    }
    // Hien thi cac lop HP theo nhom trong ki hoc hien tai
    void displaySectionsByGroup() const {
        cout << "  DANH SACH NHOM LOP HOC PHAN - HOC KY " << currentSemester << "\n";
        const int MAX_GROUPS = 100;
        string groupNames[MAX_GROUPS];
        int groupCount = getGroupsForSemester(groupNames, MAX_GROUPS);

        if (groupCount == 0) {
            cout << "(Khong co nhom lop hoc phan nao trong hoc ky " << currentSemester << ")\n";
            return;
        }

        for (int g = 0; g < groupCount; g++) {
            cout << "\n>>> NHOM: " << groupNames[g] << "\n";
            int sectionCount = 0;
            auto current = sectionList.getHead();
            while (current != nullptr) {
                if (current->data.getHocKy() == currentSemester &&
                    current->data.getNhomLopHP() == groupNames[g]) {
                    sectionCount++;
                    string tenMon = current->data.getMaMon();
                    auto c = courseList.getHead();
                    while (c != nullptr) {
                        if (c->data.getMaMon() == current->data.getMaMon()) {
                            tenMon = c->data.getTenMon();
                            break;
                        }
                        c = c->next;
                    }
                    cout << "  [" << sectionCount << "] " << current->data.getMaLopHP()
                         << " | Mon: " << tenMon
                         << " | GV: " << current->data.getGiangVien()
                         << " | Thu " << current->data.getThu()
                         << " | Tiet " << current->data.getTietBatDau() << "-" << current->data.getTietKetThuc()
                         << " | Phong: " << current->data.getPhong()
                         << " | SS: " << current->data.getSoLuongDaDangKy() << "/" << current->data.getSiSoToiDa()
                         << "\n";
                }
                current = current->next;
            }
            if (sectionCount == 0) {
                cout << "  (Khong co lop hoc phan nao trong nhom nay)\n";
            }
        }
        // Hien thi cac lop HP chua gan nhom
        bool hasUngrouped = false;
        auto current = sectionList.getHead();
        while (current != nullptr) {
            if (current->data.getHocKy() == currentSemester &&
                current->data.getNhomLopHP().empty()) {
                if (!hasUngrouped) {
                    cout << "\n>>> CHUA GAN NHOM:\n";
                    hasUngrouped = true;
                }
                string tenMon = current->data.getMaMon();
                auto c = courseList.getHead();
                while (c != nullptr) {
                    if (c->data.getMaMon() == current->data.getMaMon()) {
                        tenMon = c->data.getTenMon();
                        break;
                    }
                    c = c->next;
                }
                cout << "  " << current->data.getMaLopHP()
                     << " | Mon: " << tenMon
                     << " | GV: " << current->data.getGiangVien()
                     << " | Thu " << current->data.getThu()
                     << " | Tiet " << current->data.getTietBatDau() << "-" << current->data.getTietKetThuc()
                     << " | SS: " << current->data.getSoLuongDaDangKy() << "/" << current->data.getSiSoToiDa()
                     << "\n";
            }
            current = current->next;
        }
    }
    // Tim ma lop hoc phan ma sinh vien da dang ky cho 1 mon hoc cu the (neu co)
    string getRegisteredSectionForCourse(const string& studentID, const string& courseID) const {
        auto cur = registrationList.getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == studentID) {
                ClassSection* sec = const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
                if (sec != nullptr && sec->getMaMon() == courseID) {
                    return sec->getMaLopHP();
                }
            }
            cur = cur->next;
        }
        return "";
    }
    // Kiem tra sinh vien da dang ky mot lop hoc phan cu the chua
    bool isSectionRegistered(const string& studentID, const string& sectionID) const {
        const string regID = "DK_" + studentID + "_" + sectionID;
        return registrationTable.search(regID);
    }
    // Lay danh sach cac ma lop HP ma sinh vien da dang ky
    int getStudentRegisteredSections(const string& studentID, string outSectionIDs[], int maxCount) const {
        int count = 0;
        auto cur = registrationList.getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == studentID) {
                if (count < maxCount) {
                    outSectionIDs[count] = cur->data.getMaLopHP();
                    count++;
                }
            }
            cur = cur->next;
        }
        return count;
    }
    // Lay danh sach cac nhom lop HP phu hop voi sinh vien theo Khoa, Khoa hoc, Nganh hoc va Hoc ky
    int getEligibleGroupsForStudent(const string& studentID, GroupInfo eligibleGroups[], int maxGroups) const {
        Student* student = const_cast<SystemManager*>(this)->findStudent(studentID);
        if (student == nullptr) {
            return 0;
        }
        const int MAX_ALL_GROUPS = 100;
        string allGroupNames[MAX_ALL_GROUPS];
        int allGroupCount = getGroupsForSemester(allGroupNames, MAX_ALL_GROUPS);
        int count = 0;
        for (int i = 0; i < allGroupCount; i++) {
            string gName = allGroupNames[i];
            GroupInfo g;
            g.groupName = gName;
            g.hocKy = currentSemester;
            g.totalSections = 0;
            g.totalCredits = 0;
            bool groupMatchesStudent = true;
            bool hasMatchingSection = false;
            auto cur = sectionList.getHead();
            while (cur != nullptr) {
                if (cur->data.getHocKy() == currentSemester &&
                    cur->data.getNhomLopHP() == gName) {
                    if (g.totalSections < 50) {
                        g.sectionIDs[g.totalSections] = cur->data.getMaLopHP();
                        g.totalSections++;
                    }
                    Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
                    if (c != nullptr) {
                        g.totalCredits += c->getSoTinChi();
                        if (g.khoa.empty() && !c->getKhoa().empty() && c->getKhoa() != "Chung") g.khoa = c->getKhoa();
                        if (g.khoaHoc.empty() && !c->getKhoaHoc().empty() && c->getKhoaHoc() != "Chung") g.khoaHoc = c->getKhoaHoc();
                        if (g.nganhHoc.empty() && !c->getNganhHoc().empty() && c->getNganhHoc() != "Chung") g.nganhHoc = c->getNganhHoc();
                        bool matchK = c->getKhoa().empty() || c->getKhoa() == "Chung" || student->getKhoa().empty() || student->getKhoa() == c->getKhoa();
                        bool matchKH = c->getKhoaHoc().empty() || c->getKhoaHoc() == "Chung" || student->getKhoaHoc().empty() || student->getKhoaHoc() == c->getKhoaHoc();
                        bool matchNg = c->getNganhHoc().empty() || c->getNganhHoc() == "Chung" || student->getNganhHoc().empty() || student->getNganhHoc() == c->getNganhHoc();
                        if (matchK && matchKH && matchNg) {
                            hasMatchingSection = true;
                        } else {
                            groupMatchesStudent = false;
                        }
                    }
                }
                cur = cur->next;
            }
            if (groupMatchesStudent && hasMatchingSection) {
                if (count < maxGroups) {
                    eligibleGroups[count] = g;
                    count++;
                }
            }
        }
        // Neu khong tim thay nhom nao (do sinh vien chua co du Khoa/Nganh), tra ve tat ca cac nhom trong ky
        if (count == 0) {
            for (int i = 0; i < allGroupCount && count < maxGroups; i++) {
                string gName = allGroupNames[i];
                GroupInfo g;
                g.groupName = gName;
                g.hocKy = currentSemester;
                g.totalSections = 0;
                g.totalCredits = 0;

                auto cur = sectionList.getHead();
                while (cur != nullptr) {
                    if (cur->data.getHocKy() == currentSemester &&
                        cur->data.getNhomLopHP() == gName) {
                        if (g.totalSections < 50) {
                            g.sectionIDs[g.totalSections] = cur->data.getMaLopHP();
                            g.totalSections++;
                        }
                        Course* c = const_cast<SystemManager*>(this)->findCourse(cur->data.getMaMon());
                        if (c != nullptr) {
                            g.totalCredits += c->getSoTinChi();
                        }
                    }
                    cur = cur->next;
                }
                eligibleGroups[count] = g;
                count++;
            }
        }
        return count;
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

        // KIEM TRA CHUONG TRINH DAO TAO KHOA
        if (!isCourseAllowedForStudent(studentID, section->getMaMon())) {
            cout << "Loi: Mon hoc " << section->getMaMon()
                 << " khong thuoc chuong trinh dao tao cua khoa ban!\n";
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
        // RANG BUOC: Neu sinh vien da dang ky mon nay o lop khac -> thong bao can huy lop cu truoc khi dang ky
        string oldSecID = getRegisteredSectionForCourse(studentID, section->getMaMon());
        if (!oldSecID.empty() && oldSecID != sectionID) {
            Course* c = findCourse(section->getMaMon());
            cout << "Dang ky that bai: Ban da dang ky mon '" << (c ? c->getTenMon() : section->getMaMon())
                 << "' o lop '" << oldSecID << "'.\n";
            cout << ">> Vui long HUY lop hoc phan '" << oldSecID << "' truoc khi dang ky lop hoc phan moi cua mon nay!\n";
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
                        cout << "Sinh vien " << nextStudent << " bi trung lich hoac khong hop le, chuyen sang ban tiep theo.\n";
                    }
                }
            }
        }
        return true;
    }

    // Hien thi dang ky cua 1 sinh vien cu the
    void displayStudentRegistrations(const string& studentID) const {
        cout << "  DANH SACH LOP DA DANG KY - SV: " << studentID << "\n";
        int count = 0;
        auto current = registrationList.getHead();
        while (current != nullptr) {
            if (current->data.getMaSV() == studentID) {
                count++;
                cout << "[" << count << "] Lop HP: " << current->data.getMaLopHP()
                     << " | Ngay DK: " << current->data.getNgayDangKy() << "\n";
                auto sec = sectionList.getHead();
                while (sec != nullptr) {
                    if (sec->data.getMaLopHP() == current->data.getMaLopHP()) {
                        string tenMon = sec->data.getMaMon();
                        auto c = courseList.getHead();
                        while (c != nullptr) {
                            if (c->data.getMaMon() == sec->data.getMaMon()) {
                                tenMon = c->data.getTenMon();
                                break;
                            }
                            c = c->next;
                        }
                        cout << "    Mon: " << tenMon
                             << " | Nhom: " << (sec->data.getNhomLopHP().empty() ? "(Khong)" : sec->data.getNhomLopHP())
                             << " | Thu " << sec->data.getThu()
                             << " | Tiet " << sec->data.getTietBatDau() << "-" << sec->data.getTietKetThuc()
                             << " | GV: " << sec->data.getGiangVien()
                             << "\n";
                        break;
                    }
                    sec = sec->next;
                }
            }
            current = current->next;
        }
        if (count == 0) {
            cout << "(Chua dang ky lop hoc phan nao)\n";
        }
    }

    void checkTimeConflictInteractive(const string& studentID, const string& sectionID) const {
        ClassSection* target = const_cast<SystemManager*>(this)->findClassSection(sectionID);
        if (target == nullptr) {
            cout << "Khong tim thay lop hoc phan: " << sectionID << "\n";
            return;
        }
        cout << "\n--- KIEM TRA TRUNG LICH HOC ---\n";
        cout << "Lop can kiem tra: " << target->getMaLopHP()
             << " | Thu " << target->getThu()
             << " | Tiet " << target->getTietBatDau() << "-" << target->getTietKetThuc()
             << " | Phong: " << target->getPhong() << "\n";

        bool conflict = false;
        auto cur = registrationList.getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == studentID) {
                ClassSection* existing = const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
                if (existing != nullptr && existing->trungLichVoi(*target)) {
                    conflict = true;
                    cout << ">> [CANH BAO] TRUNG LICH voi lop da dang ky: " << existing->getMaLopHP()
                         << " (Mon: " << existing->getMaMon()
                         << ", Thu " << existing->getThu()
                         << ", Tiet " << existing->getTietBatDau() << "-" << existing->getTietKetThuc()
                         << ", Phong: " << existing->getPhong() << ")\n";
                }
            }
            cur = cur->next;
        }
        if (!conflict) {
            cout << ">> [HOP LE] Lop hoc phan KHONG BI TRUNG LICH voi bat ky lop nao ban da dang ky!\n";
        }
    }

    int calculateTotalCredits(const string& studentID) const {
        int totalCredits = 0;
        cout << "  TONG KET TIN CHI DA DANG KY - MSSV: " << studentID << "\n";
        int count = 0;
        auto cur = registrationList.getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == studentID) {
                ClassSection* sec = const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
                if (sec != nullptr) {
                    Course* c = const_cast<SystemManager*>(this)->findCourse(sec->getMaMon());
                    int tc = (c != nullptr) ? c->getSoTinChi() : 0;
                    totalCredits += tc;
                    count++;
                    cout << "[" << count << "] Lop HP: " << setw(22) << left << sec->getMaLopHP()
                         << " | Mon: " << setw(20) << (c ? c->getTenMon() : sec->getMaMon())
                         << " | So tin chi: " << tc << "\n";
                }
            }
            cur = cur->next;
        }
        cout << ">> TONG SO TIN CHI DA DANG KY: " << totalCredits << " tin chi (" << count << " lop hoc phan)\n";
        return totalCredits;
    }

    void displayStudentTimetable(const string& studentID) const {
        cout << "                   THOI KHOA BIEU SINH VIEN: " << studentID << "\n";
        cout << "                   (Hoc ky he thong: Hoc ky " << currentSemester << ")\n";
        const string days[] = {"2", "3", "4", "5", "6", "7", "CN"};
        const string dayNames[] = {"THU HAI (Thu 2)", "THU BA (Thu 3)", "THU TU (Thu 4)", 
                                   "THU NAM (Thu 5)", "THU SAU (Thu 6)", "THU BAY (Thu 7)", "CHU NHAT"};

        int totalClasses = 0;
        for (int d = 0; d < 7; d++) {
            bool hasClass = false;
            auto cur = registrationList.getHead();
            while (cur != nullptr) {
                if (cur->data.getMaSV() == studentID) {
                    ClassSection* sec = const_cast<SystemManager*>(this)->findClassSection(cur->data.getMaLopHP());
                    if (sec != nullptr && sec->getThu() == days[d]) {
                        if (!hasClass) {
                            cout << "\n>>> " << dayNames[d] << ":\n";
                            hasClass = true;
                        }
                        Course* c = const_cast<SystemManager*>(this)->findCourse(sec->getMaMon());
                        string tenMon = (c != nullptr) ? c->getTenMon() : sec->getMaMon();
                        cout << "  - Tiet " << setw(2) << right << sec->getTietBatDau() << " -> " << setw(2) << left << sec->getTietKetThuc()
                             << " | Lop HP: " << setw(22) << sec->getMaLopHP()
                             << " | Mon: " << setw(20) << tenMon
                             << " | Phong: " << setw(8) << sec->getPhong()
                             << " | GV: " << sec->getGiangVien() << "\n";
                        totalClasses++;
                    }
                }
                cur = cur->next;
            }
        }
        if (totalClasses == 0) {
            cout << "\n(Ban chua dang ky lop hoc phan nao trong thoi khoa bieu)\n";
        }
    }
    // Lay tat ca cac mon hoc ma sinh vien can dang ky trong hoc ky hien tai
    int getSemesterRequiredCourses(const string& studentID, string courseIDs[], int maxCourses) const {
        Student* student = const_cast<SystemManager*>(this)->findStudent(studentID);
        if (student == nullptr) return 0;

        int count = 0;
        auto cur = courseList.getHead();
        while (cur != nullptr) {
            const Course& c = cur->data;
            if (c.getHocKy() == currentSemester || c.getHocKy() == 0) {
                bool matchK = c.getKhoa().empty() || c.getKhoa() == "Chung" || student->getKhoa().empty() || student->getKhoa() == c.getKhoa();
                bool matchKH = c.getKhoaHoc().empty() || c.getKhoaHoc() == "Chung" || student->getKhoaHoc().empty() || student->getKhoaHoc() == c.getKhoaHoc();
                bool matchNg = c.getNganhHoc().empty() || c.getNganhHoc() == "Chung" || student->getNganhHoc().empty() || student->getNganhHoc() == c.getNganhHoc();
                if (matchK && matchKH && matchNg) {
                    if (count < maxCourses) {
                        courseIDs[count] = c.getMaMon();
                        count++;
                    }
                }
            }
            cur = cur->next;
        }
        return count;
    }

    // Kiem tra sinh vien con thieu bao nhieu mon trong hoc ky nay
    int getMissingCoursesCount(const string& studentID, string missingIDs[], int maxMissing) const {
        const int MAX_REQ = 50;
        string reqIDs[MAX_REQ];
        int reqCount = getSemesterRequiredCourses(studentID, reqIDs, MAX_REQ);

        int missing = 0;
        for (int i = 0; i < reqCount; i++) {
            string registeredSec = getRegisteredSectionForCourse(studentID, reqIDs[i]);
            if (registeredSec.empty()) {
                if (missing < maxMissing && missingIDs != nullptr) {
                    missingIDs[missing] = reqIDs[i];
                }
                missing++;
            }
        }
        return missing;
    }

    // Hien thi tinh trang va rang buoc dang ky du mon trong hoc ky
    void displaySemesterRegistrationStatus(const string& studentID) const {
        Student* student = const_cast<SystemManager*>(this)->findStudent(studentID);
        if (student == nullptr) return;

        cout << "  RANG BUOC & TIEN DO DANG KY TAT CA MON HOC - HOC KY " << currentSemester << "\n";
        cout << "  Sinh vien: " << student->getHoTen() << " (MSSV: " << student->getMaSV()
             << " | Lop: " << student->getLop() << ")\n";

        const int MAX_REQ = 50;
        string reqIDs[MAX_REQ];
        int reqCount = getSemesterRequiredCourses(studentID, reqIDs, MAX_REQ);

        if (reqCount == 0) {
            cout << "(Khong tim thay mon hoc nao duoc mo cho chuong trinh cua ban trong hoc ky " << currentSemester << ")\n";
            return;
        }

        int registeredCount = 0;
        int totalCredits = 0;
        int registeredCredits = 0;

        for (int i = 0; i < reqCount; i++) {
            Course* c = const_cast<SystemManager*>(this)->findCourse(reqIDs[i]);
            string tenMon = (c ? c->getTenMon() : reqIDs[i]);
            int tc = (c ? c->getSoTinChi() : 0);
            totalCredits += tc;

            string regSec = getRegisteredSectionForCourse(studentID, reqIDs[i]);
            cout << "[" << (i + 1) << "] [" << reqIDs[i] << "] " << setw(26) << left << tenMon
                 << " (" << tc << " TC) -> ";

            if (!regSec.empty()) {
                registeredCount++;
                registeredCredits += tc;
                ClassSection* s = const_cast<SystemManager*>(this)->findClassSection(regSec);
                cout << "[DA DANG KY: " << regSec;
                if (s) {
                    cout << " | Thu " << s->getThu() << " Tiet " << s->getTietBatDau() << "-" << s->getTietKetThuc();
                }
                cout << "]\n";
            } else {
                cout << "[CHUA DANG KY - CAN BO SUNG]\n";
            }
        }

        cout << "Tong so mon can hoc trong ky: " << reqCount << " mon (" << totalCredits << " tin chi)\n";
        cout << "So mon da dang ky:           " << registeredCount << "/" << reqCount
             << " mon (" << registeredCredits << "/" << totalCredits << " tin chi)\n";

        if (registeredCount == reqCount) {
            cout << ">> [TIEN DO HOC TAP]: HOAN THANH! Ban da dang ky DAY DU tat ca cac mon trong Hoc ky "
                 << currentSemester << "!\n";
        } else {
            int missing = reqCount - registeredCount;
            cout << ">> [CANH BAO TIEN DO]: CHUA HOAN THANH! Ban con thieu " << missing
                 << " mon trong Hoc ky " << currentSemester << ".\n";
            cout << ">> Canh bao: Dang ky thieu mon se lam CHAM TIEN DO HOC TAP cua ban!\n";
            cout << ">> Cac mon con thieu:\n";
            for (int i = 0; i < reqCount; i++) {
                if (getRegisteredSectionForCourse(studentID, reqIDs[i]).empty()) {
                    Course* c = const_cast<SystemManager*>(this)->findCourse(reqIDs[i]);
                    cout << "   - [" << reqIDs[i] << "] " << (c ? c->getTenMon() : reqIDs[i])
                         << " (" << (c ? c->getSoTinChi() : 0) << " TC)\n";
                }
            }
            cout << ">> Khuyen nghi: Ban nen dang ky bo sung day du cac mon de dam bao dung tien do chuong trinh dao tao!\n";
        }
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
