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

#endif // STUDENT_REPOSITORY_H
