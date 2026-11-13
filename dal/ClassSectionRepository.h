#ifndef CLASSSECTION_REPOSITORY_H
#define CLASSSECTION_REPOSITORY_H

#include "IRepository.h"
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../DSA/queue.h"
#include "../model/classsection.h"
#include <string>
using namespace std;

// ============================================================
//  ClassSectionRepository - DAL tang du lieu cho ClassSection
//
//  Quan he:
//    - ClassSection thuoc ve Course (FK: maMon -> Course)
//      => Mot Course co nhieu ClassSection (1..n)
//    - ClassSection la trung gian giua Student va Course
//      thong qua Registration (bang lien ket M..N)
//    - Moi ClassSection co 1 hang doi (waitlist) rieng
//      luu trong waitlistTable
//
//  Chuc nang:
//    - add      : Them lop HP (kiem tra maMon ton tai, soLuong > 0,
//                 tiet hop le), tu dong tao hang doi waitlist
//    - remove   : Xoa lop HP (xoa ca waitlist), truoc khi xoa
//                 SystemManager phai don sach cac Registration
//    - findById : Tim nhanh qua HashTable
//    - getWaitlist: Truy cap hang doi sinh vien cho
//    - search / displayAll / displayByGroup
//      (da tach sang Presentation; DAL cung cap getList / search)
// ============================================================
class ClassSectionRepository : public IRepository<ClassSection, string> {
private:
    DoubleLinkedList<ClassSection>              sectionList;
    HashTable<string, NodeDLL<ClassSection>*>  sectionTable;
    HashTable<string, Queue<string>*>           waitlistTable;

    NodeDLL<ClassSection>* getLastNode() const;

    static string toLower(string s);

public:
    ~ClassSectionRepository();

    // ----- IRepository interface -----

    // Truyen vao CourseExists callback de kiem tra FK
    bool add(const ClassSection& section) override;

    // Luu y: truoc khi goi remove, SystemManager phai xoa het Registration cua lop nay
    bool remove(const string& maLopHP) override;

    ClassSection* findById(const string& maLopHP) override;
    const ClassSection* findById(const string& maLopHP) const;

    // ----- Chuc nang mo rong -----

    // Truy cap hang doi waitlist cua lop HP
    Queue<string>* getWaitlist(const string& maLopHP);
    const Queue<string>* getWaitlist(const string& maLopHP) const;

    bool exists(const string& maLopHP) const;

    // Tim kiem theo tu khoa (maLopHP, maMon, nhomLopHP, giangVien)
    void search(const string& keyword, DoubleLinkedList<ClassSection>& outResults) const;

    // Lay danh sach cac nhom lop HP duy nhat trong ki hoc cho truoc
    int getGroupsForSemester(int hocKy, string groupNames[], int maxGroups) const;

    // Duyet toan bo danh sach de loc theo ki hoc (dung cho timetable, ...)
    const DoubleLinkedList<ClassSection>& getList() const override;

    void clear();
};

inline NodeDLL<ClassSection>* ClassSectionRepository::getLastNode() const {
    auto cur = sectionList.getHead();
    NodeDLL<ClassSection>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

inline string ClassSectionRepository::toLower(string s) {
    return utf8ToLower(s);
}

inline ClassSectionRepository::~ClassSectionRepository() {
    // Giai phong tat ca hang doi waitlist
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        Queue<string>* q = nullptr;
        if (waitlistTable.get(cur->data.getMaLopHP(), q)) delete q;
        cur = cur->next;
    }
}

// ----- IRepository interface -----

// Truyen vao CourseExists callback de kiem tra FK
inline bool ClassSectionRepository::add(const ClassSection& section) {
    if (section.getMaLopHP().empty()           ||
        sectionTable.search(section.getMaLopHP()) ||
        section.getSiSoToiDa()   <= 0          ||
        section.getTietBatDau()  <= 0          ||
        section.getTietKetThuc() <= section.getTietBatDau())
        return false;
    sectionList.pushBack(section);
    sectionTable.insert(section.getMaLopHP(), getLastNode());
    waitlistTable.insert(section.getMaLopHP(), new Queue<string>());
    return true;
}

// Luu y: truoc khi goi remove, SystemManager phai xoa het Registration cua lop nay
inline bool ClassSectionRepository::remove(const string& maLopHP) {
    NodeDLL<ClassSection>* node = nullptr;
    if (!sectionTable.get(maLopHP, node)) return false;

    // Giai phong hang doi
    Queue<string>* q = nullptr;
    if (waitlistTable.get(maLopHP, q)) {
        delete q;
        waitlistTable.erase(maLopHP);
    }
    sectionList.eraseNode(node);
    sectionTable.erase(maLopHP);
    return true;
}

inline ClassSection* ClassSectionRepository::findById(const string& maLopHP) {
    NodeDLL<ClassSection>* node = nullptr;
    if (sectionTable.get(maLopHP, node)) return &(node->data);
    return nullptr;
}

inline const ClassSection* ClassSectionRepository::findById(const string& maLopHP) const {
    NodeDLL<ClassSection>* node = nullptr;
    if (sectionTable.get(maLopHP, node)) return &(node->data);
    return nullptr;
}

// ----- Chuc nang mo rong -----

// Truy cap hang doi waitlist cua lop HP
inline Queue<string>* ClassSectionRepository::getWaitlist(const string& maLopHP) {
    Queue<string>* q = nullptr;
    waitlistTable.get(maLopHP, q);
    return q;
}

inline const Queue<string>* ClassSectionRepository::getWaitlist(const string& maLopHP) const {
    Queue<string>* q = nullptr;
    waitlistTable.get(maLopHP, q);
    return q;
}

inline bool ClassSectionRepository::exists(const string& maLopHP) const {
    return sectionTable.search(maLopHP);
}

// Tim kiem theo tu khoa (maLopHP, maMon, nhomLopHP, giangVien)
inline void ClassSectionRepository::search(const string& keyword, DoubleLinkedList<ClassSection>& outResults) const {
    outResults.clear();
    string kw = toLower(keyword);
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        string id   = toLower(cur->data.getMaLopHP());
        string mon  = toLower(cur->data.getMaMon());
        string nhom = toLower(cur->data.getNhomLopHP());
        string gv   = toLower(cur->data.getGiangVien());
        if (id.find(kw)   != string::npos || mon.find(kw)  != string::npos ||
            nhom.find(kw) != string::npos || gv.find(kw)   != string::npos) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

// Lay danh sach cac nhom lop HP duy nhat trong ki hoc cho truoc
inline int ClassSectionRepository::getGroupsForSemester(int hocKy, string groupNames[], int maxGroups) const {
    int count = 0;
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        if (cur->data.getHocKy() == hocKy && !cur->data.getNhomLopHP().empty()) {
            string nhom = cur->data.getNhomLopHP();
            bool found = false;
            for (int i = 0; i < count; i++)
                if (groupNames[i] == nhom) { found = true; break; }
            if (!found && count < maxGroups)
                groupNames[count++] = nhom;
        }
        cur = cur->next;
    }
    return count;
}

// Duyet toan bo danh sach de loc theo ki hoc (dung cho timetable, ...)
inline const DoubleLinkedList<ClassSection>& ClassSectionRepository::getList() const {
    return sectionList;
}

inline void ClassSectionRepository::clear() {
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        Queue<string>* q = nullptr;
        if (waitlistTable.get(cur->data.getMaLopHP(), q)) delete q;
        cur = cur->next;
    }
    waitlistTable.clear();
    sectionTable.clear();
    sectionList.clear();
}

#endif // CLASSSECTION_REPOSITORY_H
