#include "ClassSectionRepository.h"

ClassSectionRepository::ClassSectionRepository() {}

ClassSectionRepository::~ClassSectionRepository() {
    clear();
}

NodeDLL<ClassSection>* ClassSectionRepository::getLastNode() const {
    auto cur = sectionList.getHead();
    NodeDLL<ClassSection>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

string ClassSectionRepository::toLower(string s) {
    return utf8ToLower(s);
}

bool ClassSectionRepository::add(const ClassSection& section) {
    if (section.getMaLopHP().empty()           ||
        sectionTable.search(section.getMaLopHP()) ||
        section.getSiSoToiDa()   <= 0          ||
        section.getTietBatDau()  <= 0          ||
        section.getTietKetThuc() <= section.getTietBatDau())
        return false;
    sectionList.pushBack(section);
    sectionTable.insert(section.getMaLopHP(), getLastNode());
    return true;
}

bool ClassSectionRepository::remove(const string& maLopHP) {
    NodeDLL<ClassSection>* node = nullptr;
    if (!sectionTable.get(maLopHP, node)) return false;
    clearSectionWaitlist(maLopHP);
    sectionList.eraseNode(node);
    sectionTable.erase(maLopHP);
    return true;
}

ClassSection* ClassSectionRepository::findById(const string& maLopHP) {
    NodeDLL<ClassSection>* node = nullptr;
    if (sectionTable.get(maLopHP, node)) return &(node->data);
    return nullptr;
}

const ClassSection* ClassSectionRepository::findById(const string& maLopHP) const {
    NodeDLL<ClassSection>* node = nullptr;
    if (sectionTable.get(maLopHP, node)) return &(node->data);
    return nullptr;
}

bool ClassSectionRepository::exists(const string& maLopHP) const {
    return sectionTable.search(maLopHP);
}

void ClassSectionRepository::search(const string& keyword, DoubleLinkedList<ClassSection>& outResults) const {
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

int ClassSectionRepository::getGroupsForSemester(int hocKy, string groupNames[], int maxGroups) const {
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

const DoubleLinkedList<ClassSection>& ClassSectionRepository::getList() const {
    return sectionList;
}

void ClassSectionRepository::clear() {
    auto cur = sectionKeys.getHead();
    while (cur != nullptr) {
        Queue<string>* q = nullptr;
        if (waitlistTable.get(cur->data, q)) {
            delete q;
        }
        cur = cur->next;
    }
    sectionKeys.clear();
    waitlistTable.clear();

    sectionTable.clear();
    sectionList.clear();
}

Queue<string>* ClassSectionRepository::getWaitlist(const string& maLopHP) {
    Queue<string>* q = nullptr;
    waitlistTable.get(maLopHP, q);
    return q;
}

const Queue<string>* ClassSectionRepository::getWaitlist(const string& maLopHP) const {
    Queue<string>* q = nullptr;
    waitlistTable.get(maLopHP, q);
    return q;
}

Queue<string>* ClassSectionRepository::ensureWaitlist(const string& maLopHP) {
    Queue<string>* q = nullptr;
    if (waitlistTable.get(maLopHP, q)) return q;
    q = new Queue<string>();
    waitlistTable.insert(maLopHP, q);
    sectionKeys.pushBack(maLopHP);
    return q;
}

bool ClassSectionRepository::hasWaitlist(const string& maLopHP) const {
    const Queue<string>* q = getWaitlist(maLopHP);
    return q != nullptr && !q->empty();
}

int ClassSectionRepository::getWaitlistCount(const string& maLopHP) const {
    const Queue<string>* q = getWaitlist(maLopHP);
    return q ? q->size() : 0;
}

bool ClassSectionRepository::isInWaitlist(const string& maLopHP, const string& maSV) const {
    const Queue<string>* q = getWaitlist(maLopHP);
    return q != nullptr && q->contains(maSV);
}

bool ClassSectionRepository::pushToWaitlist(const string& maLopHP, const string& maSV) {
    Queue<string>* q = ensureWaitlist(maLopHP);
    if (q->contains(maSV)) return false;
    q->push(maSV);
    return true;
}

bool ClassSectionRepository::popFromWaitlist(const string& maLopHP, string& outSV) {
    Queue<string>* q = getWaitlist(maLopHP);
    if (!q || q->empty()) return false;
    return q->pop(outSV);
}

bool ClassSectionRepository::removeFromWaitlist(const string& maLopHP, const string& maSV) {
    Queue<string>* q = getWaitlist(maLopHP);
    if (!q) return false;
    return q->remove(maSV);
}

void ClassSectionRepository::clearSectionWaitlist(const string& maLopHP) {
    Queue<string>* q = nullptr;
    if (waitlistTable.get(maLopHP, q)) {
        delete q;
        waitlistTable.erase(maLopHP);
    }
    sectionKeys.eraseValue(maLopHP);
}

void ClassSectionRepository::removeStudentFromAllWaitlists(const string& maSV) {
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        Queue<string>* q = getWaitlist(cur->data.getMaLopHP());
        if (q != nullptr) q->remove(maSV);
        cur = cur->next;
    }
}

int ClassSectionRepository::removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                                            const string& exceptSectionID,
                                                            DoubleLinkedList<string>* outRemovedWaitlists) {
    int removedCount = 0;
    auto cur = sectionList.getHead();
    while (cur != nullptr) {
        const string& secID = cur->data.getMaLopHP();
        if (cur->data.getMaMon() == maMon && secID != exceptSectionID) {
            Queue<string>* q = getWaitlist(secID);
            if (q && q->contains(maSV)) {
                if (q->remove(maSV)) {
                    removedCount++;
                    if (outRemovedWaitlists != nullptr) {
                        outRemovedWaitlists->pushBack(secID);
                    }
                }
            }
        }
        cur = cur->next;
    }
    return removedCount;
}
