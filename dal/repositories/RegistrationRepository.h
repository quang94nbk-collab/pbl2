#ifndef REGISTRATION_REPOSITORY_H
#define REGISTRATION_REPOSITORY_H

#include "IRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include "../../DSA/hashtable.h"
#include "../../model/registration.h"
#include <string>
using namespace std;

// ============================================================
//  RegistrationRepository - Pure Repository cho thuc the Registration
// ============================================================
class RegistrationRepository : public IRepository<Registration, string> {
private:
    DoubleLinkedList<Registration>              registrationList;
    HashTable<string, NodeDLL<Registration>*>   registrationTable;

    NodeDLL<Registration>* getLastNode() const;

public:
    static string makeRegID(const string& maSV, const string& maLopHP);

    bool add(const Registration& reg) override;
    bool remove(const string& maDangKy) override;
    Registration* findById(const string& maDangKy) override;
    const Registration* findById(const string& maDangKy) const;
    bool exists(const string& maDangKy) const override;

    bool isRegistered(const string& maSV, const string& maLopHP) const;
    NodeDLL<Registration>* getNode(const string& maDangKy);
    string getRegID(const string& maSV, const string& maLopHP) const;

    int getSectionsByStudent(const string& maSV, string outSectionIDs[], int maxCount) const;
    int getStudentsBySection(const string& maLopHP, string outStudentIDs[], int maxCount) const;
    void getByStudent(const string& maSV, DoubleLinkedList<Registration>& outResults) const;
    void getBySection(const string& maLopHP, DoubleLinkedList<Registration>& outResults) const;
    void removeAllBySection(const string& maLopHP);
    void removeAllByStudent(const string& maSV);

    const DoubleLinkedList<Registration>& getList() const override;
    void clear();
};

inline NodeDLL<Registration>* RegistrationRepository::getLastNode() const {
    auto cur = registrationList.getHead();
    NodeDLL<Registration>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

inline string RegistrationRepository::makeRegID(const string& maSV, const string& maLopHP) {
    return "DK_" + maSV + "_" + maLopHP;
}

inline bool RegistrationRepository::add(const Registration& reg) {
    if (reg.getMaDangKy().empty() || registrationTable.search(reg.getMaDangKy()))
        return false;
    registrationList.pushBack(reg);
    registrationTable.insert(reg.getMaDangKy(), getLastNode());
    return true;
}

inline bool RegistrationRepository::remove(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    if (!registrationTable.get(maDangKy, node)) return false;
    registrationList.eraseNode(node);
    registrationTable.erase(maDangKy);
    return true;
}

inline Registration* RegistrationRepository::findById(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    if (registrationTable.get(maDangKy, node)) return &(node->data);
    return nullptr;
}

inline const Registration* RegistrationRepository::findById(const string& maDangKy) const {
    NodeDLL<Registration>* node = nullptr;
    if (registrationTable.get(maDangKy, node)) return &(node->data);
    return nullptr;
}

inline bool RegistrationRepository::exists(const string& maDangKy) const {
    return registrationTable.search(maDangKy);
}

inline bool RegistrationRepository::isRegistered(const string& maSV, const string& maLopHP) const {
    return registrationTable.search(makeRegID(maSV, maLopHP));
}

inline NodeDLL<Registration>* RegistrationRepository::getNode(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    registrationTable.get(maDangKy, node);
    return node;
}

inline string RegistrationRepository::getRegID(const string& maSV, const string& maLopHP) const {
    return makeRegID(maSV, maLopHP);
}

inline int RegistrationRepository::getSectionsByStudent(const string& maSV, string outSectionIDs[], int maxCount) const {
    int count = 0;
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV && count < maxCount)
            outSectionIDs[count++] = cur->data.getMaLopHP();
        cur = cur->next;
    }
    return count;
}

inline int RegistrationRepository::getStudentsBySection(const string& maLopHP, string outStudentIDs[], int maxCount) const {
    int count = 0;
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP && count < maxCount)
            outStudentIDs[count++] = cur->data.getMaSV();
        cur = cur->next;
    }
    return count;
}

inline void RegistrationRepository::getByStudent(const string& maSV, DoubleLinkedList<Registration>& outResults) const {
    outResults.clear();
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

inline void RegistrationRepository::getBySection(const string& maLopHP, DoubleLinkedList<Registration>& outResults) const {
    outResults.clear();
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

inline void RegistrationRepository::removeAllBySection(const string& maLopHP) {
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        auto next = cur->next;
        if (cur->data.getMaLopHP() == maLopHP) {
            registrationTable.erase(cur->data.getMaDangKy());
            registrationList.eraseNode(cur);
        }
        cur = next;
    }
}

inline void RegistrationRepository::removeAllByStudent(const string& maSV) {
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        auto next = cur->next;
        if (cur->data.getMaSV() == maSV) {
            registrationTable.erase(cur->data.getMaDangKy());
            registrationList.eraseNode(cur);
        }
        cur = next;
    }
}

inline const DoubleLinkedList<Registration>& RegistrationRepository::getList() const {
    return registrationList;
}

inline void RegistrationRepository::clear() {
    registrationTable.clear();
    registrationList.clear();
}

#endif // REGISTRATION_REPOSITORY_H
