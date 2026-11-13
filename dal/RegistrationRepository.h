#ifndef REGISTRATION_REPOSITORY_H
#define REGISTRATION_REPOSITORY_H

#include "IRepository.h"
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../model/registration.h"
#include <string>
using namespace std;

// ============================================================
//  RegistrationRepository - DAL tang du lieu cho Registration
//
//  Quan he:
//    - Registration la bang lien ket M..N giua Student va ClassSection
//      FK: maSV -> Student, maLopHP -> ClassSection
//    - Ma khoa chinh (maDangKy) duoc sinh tu quy tac:
//        "DK_<maSV>_<maLopHP>"
//    - Mot sinh vien co the co nhieu Registration (1..n)
//    - Mot ClassSection co the co nhieu Registration (1..n)
//
//  Chuc nang:
//    - add      : Them phieu dang ky
//    - remove   : Huy phieu dang ky theo maDangKy
//    - findById : Tim phieu dang ky theo maDangKy
//    - findBySectionAndStudent: kiem tra dang ky cu the
//    - getByStudent : lay tat ca lop HP cua 1 sinh vien
//    - getBySection : lay tat ca sinh vien cua 1 lop HP
//    - getRegisteredSectionForCourse : tim lop HP da dang ky cho 1 mon
// ============================================================
class RegistrationRepository : public IRepository<Registration, string> {
private:
    DoubleLinkedList<Registration>              registrationList;
    HashTable<string, NodeDLL<Registration>*>   registrationTable;

    NodeDLL<Registration>* getLastNode() const;

    // Sinh ma dang ky chuan
    static string makeRegID(const string& maSV, const string& maLopHP);

public:
    // ----- IRepository interface -----

    bool add(const Registration& reg) override;

    bool remove(const string& maDangKy) override;

    Registration* findById(const string& maDangKy) override;
    const Registration* findById(const string& maDangKy) const;

    // ----- Chuc nang mo rong -----

    // Kiem tra sinh vien da dang ky lop HP nay chua
    bool isRegistered(const string& maSV, const string& maLopHP) const;

    // Lay node Registration theo maDangKy (dung khi can xoa)
    NodeDLL<Registration>* getNode(const string& maDangKy);

    // Lay ma dang ky tu maSV + maLopHP
    string getRegID(const string& maSV, const string& maLopHP) const;

    // Tim tat ca maLopHP ma 1 sinh vien da dang ky
    // Tra ve so luong tim duoc
    int getSectionsByStudent(const string& maSV, string outSectionIDs[], int maxCount) const;

    // Tim tat ca maSV da dang ky vao 1 lop HP
    int getStudentsBySection(const string& maLopHP, string outStudentIDs[], int maxCount) const;

    // Lay danh sach doi tuong Registration theo sinh vien / theo lop hoc phan
    void getByStudent(const string& maSV, DoubleLinkedList<Registration>& outResults) const;
    void getBySection(const string& maLopHP, DoubleLinkedList<Registration>& outResults) const;

    // Tim maLopHP sinh vien da dang ky cho 1 mon hoc cu the
    // (Can ClassSectionRepository de check maMon)
    // Tra ve "" neu chua dang ky
    // Ghi chu: ham nay duoc goi tu SystemManager vi can cross-reference
    // => De o day voi signature nhan them callback
    // Hoac de trong SystemManager goi thong qua 2 repo

    // Xoa toan bo dang ky theo maLopHP (dung khi xoa lop HP)
    void removeAllBySection(const string& maLopHP);

    // Xoa toan bo dang ky theo maSV (dung khi xoa sinh vien)
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

// Sinh ma dang ky chuan
inline string RegistrationRepository::makeRegID(const string& maSV, const string& maLopHP) {
    return "DK_" + maSV + "_" + maLopHP;
}

// ----- IRepository interface -----

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

// ----- Chuc nang mo rong -----

// Kiem tra sinh vien da dang ky lop HP nay chua
inline bool RegistrationRepository::isRegistered(const string& maSV, const string& maLopHP) const {
    return registrationTable.search(makeRegID(maSV, maLopHP));
}

// Lay node Registration theo maDangKy (dung khi can xoa)
inline NodeDLL<Registration>* RegistrationRepository::getNode(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    registrationTable.get(maDangKy, node);
    return node;
}

// Lay ma dang ky tu maSV + maLopHP
inline string RegistrationRepository::getRegID(const string& maSV, const string& maLopHP) const {
    return makeRegID(maSV, maLopHP);
}

// Tim tat ca maLopHP ma 1 sinh vien da dang ky
// Tra ve so luong tim duoc
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

// Tim tat ca maSV da dang ky vao 1 lop HP
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

// Xoa toan bo dang ky theo maLopHP (dung khi xoa lop HP)
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

// Xoa toan bo dang ky theo maSV (dung khi xoa sinh vien)
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
