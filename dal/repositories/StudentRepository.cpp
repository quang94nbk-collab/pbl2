#include "StudentRepository.h"

NodeDLL<Student>* StudentRepository::getLastNode() const {
    auto cur = studentList.getHead();
    NodeDLL<Student>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

string StudentRepository::toLower(string s) {
    return utf8ToLower(s);
}

bool StudentRepository::add(const Student& student) {
    if (student.getMaSV().empty() || studentTable.search(student.getMaSV()))
        return false;
    studentList.pushBack(student);
    studentTable.insert(student.getMaSV(), getLastNode());
    return true;
}

bool StudentRepository::remove(const string& maSV) {
    NodeDLL<Student>* node = nullptr;
    if (!studentTable.get(maSV, node)) return false;
    studentList.eraseValue(node->data);
    studentTable.erase(maSV);
    return true;
}

Student* StudentRepository::findById(const string& maSV) {
    NodeDLL<Student>* node = nullptr;
    if (studentTable.get(maSV, node)) return &(node->data);
    return nullptr;
}

const Student* StudentRepository::findById(const string& maSV) const {
    NodeDLL<Student>* node = nullptr;
    if (studentTable.get(maSV, node)) return &(node->data);
    return nullptr;
}

bool StudentRepository::exists(const string& maSV) const {
    return studentTable.search(maSV);
}

void StudentRepository::search(const string& keyword, DoubleLinkedList<Student>& outResults) const {
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

const DoubleLinkedList<Student>& StudentRepository::getList() const {
    return studentList;
}

void StudentRepository::clear() {
    studentTable.clear();
    studentList.clear();
}
