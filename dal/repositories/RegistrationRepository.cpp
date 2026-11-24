#include "RegistrationRepository.h"

NodeDLL<Registration>* RegistrationRepository::getLastNode() const {
    auto cur = registrationList.getHead();
    NodeDLL<Registration>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

string RegistrationRepository::makeRegID(const string& maSV, const string& maLopHP) {
    return "DK_" + maSV + "_" + maLopHP;
}

bool RegistrationRepository::add(const Registration& reg) {
    if (reg.getMaDangKy().empty() || registrationTable.search(reg.getMaDangKy()))
        return false;
    registrationList.pushBack(reg);
    registrationTable.insert(reg.getMaDangKy(), getLastNode());
    return true;
}

bool RegistrationRepository::remove(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    if (!registrationTable.get(maDangKy, node)) return false;
    registrationList.eraseNode(node);
    registrationTable.erase(maDangKy);
    return true;
}

Registration* RegistrationRepository::findById(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    if (registrationTable.get(maDangKy, node)) return &(node->data);
    return nullptr;
}

const Registration* RegistrationRepository::findById(const string& maDangKy) const {
    NodeDLL<Registration>* node = nullptr;
    if (registrationTable.get(maDangKy, node)) return &(node->data);
    return nullptr;
}

bool RegistrationRepository::exists(const string& maDangKy) const {
    return registrationTable.search(maDangKy);
}

bool RegistrationRepository::isRegistered(const string& maSV, const string& maLopHP) const {
    return registrationTable.search(makeRegID(maSV, maLopHP));
}

NodeDLL<Registration>* RegistrationRepository::getNode(const string& maDangKy) {
    NodeDLL<Registration>* node = nullptr;
    registrationTable.get(maDangKy, node);
    return node;
}

string RegistrationRepository::getRegID(const string& maSV, const string& maLopHP) const {
    return makeRegID(maSV, maLopHP);
}

int RegistrationRepository::getSectionsByStudent(const string& maSV, string outSectionIDs[], int maxCount) const {
    int count = 0;
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV && count < maxCount)
            outSectionIDs[count++] = cur->data.getMaLopHP();
        cur = cur->next;
    }
    return count;
}

int RegistrationRepository::getStudentsBySection(const string& maLopHP, string outStudentIDs[], int maxCount) const {
    int count = 0;
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP && count < maxCount)
            outStudentIDs[count++] = cur->data.getMaSV();
        cur = cur->next;
    }
    return count;
}

void RegistrationRepository::getByStudent(const string& maSV, DoubleLinkedList<Registration>& outResults) const {
    outResults.clear();
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

void RegistrationRepository::getBySection(const string& maLopHP, DoubleLinkedList<Registration>& outResults) const {
    outResults.clear();
    auto cur = registrationList.getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

void RegistrationRepository::removeAllBySection(const string& maLopHP) {
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

void RegistrationRepository::removeAllByStudent(const string& maSV) {
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

const DoubleLinkedList<Registration>& RegistrationRepository::getList() const {
    return registrationList;
}

void RegistrationRepository::clear() {
    registrationTable.clear();
    registrationList.clear();
}
