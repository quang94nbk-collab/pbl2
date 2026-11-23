#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include "IRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include "../../DSA/hashtable.h"
#include "../../model/student.h"
#include <string>
using namespace std;

// ============================================================
//  StudentRepository - Pure Repository cho thuc the Student
// ============================================================
class StudentRepository : public IRepository<Student, string> {
private:
    DoubleLinkedList<Student>              studentList;
    HashTable<string, NodeDLL<Student>*>   studentTable;

    NodeDLL<Student>* getLastNode() const;
    static string toLower(string s);

public:
    bool add(const Student& student) override;
    bool remove(const string& maSV) override;
    Student* findById(const string& maSV) override;
    const Student* findById(const string& maSV) const;
    bool exists(const string& maSV) const override;

    void search(const string& keyword, DoubleLinkedList<Student>& outResults) const;
    const DoubleLinkedList<Student>& getList() const override;
    void clear();
};

inline NodeDLL<Student>* StudentRepository::getLastNode() const {
    auto cur = studentList.getHead();
    NodeDLL<Student>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

inline string StudentRepository::toLower(string s) {
    return utf8ToLower(s);
}

inline bool StudentRepository::add(const Student& student) {
    if (student.getMaSV().empty() || studentTable.search(student.getMaSV()))
        return false;
    studentList.pushBack(student);
    studentTable.insert(student.getMaSV(), getLastNode());
    return true;
}

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

inline bool StudentRepository::exists(const string& maSV) const {
    return studentTable.search(maSV);
}

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

inline const DoubleLinkedList<Student>& StudentRepository::getList() const {
    return studentList;
}

inline void StudentRepository::clear() {
    studentTable.clear();
    studentList.clear();
}

#endif // STUDENT_REPOSITORY_H
