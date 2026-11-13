#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include "IRepository.h"
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../model/student.h"
#include <string>
using namespace std;

// ============================================================
//  StudentRepository - DAL tang du lieu cho Student
//
//  Quan he:
//    - Student ke thua Person (quan he Generalization)
//    - StudentRepository quan ly DoubleLinkedList<Student>
//      + HashTable de truy cap O(1) theo maSV
//
//  Chuc nang:
//    - add      : Them sinh vien (kiem tra trung maSV)
//    - remove   : Xoa sinh vien theo maSV
//    - findById : Tim nhanh O(1) qua HashTable
//    - search   : Tim kiem theo tu khoa (ten, lop, khoa...)
//    - displayAll / displayFiltered
//      (da tach sang Presentation; DAL cung cap getList / search)
// ============================================================
class StudentRepository : public IRepository<Student, string> {
private:
    DoubleLinkedList<Student>              studentList;
    HashTable<string, NodeDLL<Student>*>   studentTable;

    // Lay node cuoi trong danh sach de luu vao hashtable
    NodeDLL<Student>* getLastNode() const;

    static string toLower(string s);

public:
    // ----- IRepository interface -----

    bool add(const Student& student) override;

    // Luu y: xoa sinh vien can kiem tra dang ky -> goi tu SystemManager
    bool remove(const string& maSV) override;

    Student* findById(const string& maSV) override;
    const Student* findById(const string& maSV) const;

    // ----- Chuc nang mo rong -----

    // Tim kiem theo tu khoa (maSV, hoTen, lop, khoa)
    void search(const string& keyword, DoubleLinkedList<Student>& outResults) const;

    // Truy cap danh sach de doc (dung trong SystemManager khi can duyet)
    const DoubleLinkedList<Student>& getList() const override;

    void clear();
};

// Lay node cuoi trong danh sach de luu vao hashtable
inline NodeDLL<Student>* StudentRepository::getLastNode() const {
    auto cur = studentList.getHead();
    NodeDLL<Student>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

inline string StudentRepository::toLower(string s) {
    return utf8ToLower(s);
}

// ----- IRepository interface -----

inline bool StudentRepository::add(const Student& student) {
    if (student.getMaSV().empty() || studentTable.search(student.getMaSV()))
        return false;
    studentList.pushBack(student);
    studentTable.insert(student.getMaSV(), getLastNode());
    return true;
}

// Luu y: xoa sinh vien can kiem tra dang ky -> goi tu SystemManager
inline bool StudentRepository::remove(const string& maSV) {
    NodeDLL<Student>* node = nullptr;
    if (!studentTable.get(maSV, node)) return false;
    studentList.eraseValue(node->data);
    studentTable.erase(maSV);
    return true;
}

inline Student* StudentRepository::findById(const string& maSV) {
    NodeDLL<Student>* node = nullptr;
    if (studentTable.get(maSV, node)) return &(node->data);
    return nullptr;
}

inline const Student* StudentRepository::findById(const string& maSV) const {
    NodeDLL<Student>* node = nullptr;
    if (studentTable.get(maSV, node)) return &(node->data);
    return nullptr;
}

// ----- Chuc nang mo rong -----

// Tim kiem theo tu khoa (maSV, hoTen, lop, khoa)
inline void StudentRepository::search(const string& keyword, DoubleLinkedList<Student>& outResults) const {
    outResults.clear();
    string kw = toLower(keyword);
    auto cur = studentList.getHead();
    while (cur != nullptr) {
        string id   = toLower(cur->data.getMaSV());
        string name = toLower(cur->data.getHoTen());
        string lop  = toLower(cur->data.getLop());
        string khoa = toLower(cur->data.getKhoa());
        if (id.find(kw)   != string::npos || name.find(kw) != string::npos ||
            lop.find(kw)  != string::npos || khoa.find(kw) != string::npos) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

// Truy cap danh sach de doc (dung trong SystemManager khi can duyet)
inline const DoubleLinkedList<Student>& StudentRepository::getList() const {
    return studentList;
}

inline void StudentRepository::clear() {
    studentTable.clear();
    studentList.clear();
}

#endif // STUDENT_REPOSITORY_H
