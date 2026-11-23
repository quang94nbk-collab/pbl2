#ifndef CLASSSECTION_REPOSITORY_H
#define CLASSSECTION_REPOSITORY_H

#include "IRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include "../../DSA/hashtable.h"
#include "../../model/classsection.h"
#include <string>
using namespace std;

// ============================================================
//  ClassSectionRepository - Pure Repository cho ClassSection
//  (Hang doi waitlist da duoc tach sang dal/relations/SectionWaitlistRelation)
// ============================================================
class ClassSectionRepository : public IRepository<ClassSection, string> {
private:
    DoubleLinkedList<ClassSection>             sectionList;
    HashTable<string, NodeDLL<ClassSection>*> sectionTable;

    NodeDLL<ClassSection>* getLastNode() const;
    static string toLower(string s);

public:
    bool add(const ClassSection& section) override;
    bool remove(const string& maLopHP) override;
    ClassSection* findById(const string& maLopHP) override;
    const ClassSection* findById(const string& maLopHP) const;
    bool exists(const string& maLopHP) const override;

    void search(const string& keyword, DoubleLinkedList<ClassSection>& outResults) const;
    int getGroupsForSemester(int hocKy, string groupNames[], int maxGroups) const;
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

inline bool ClassSectionRepository::add(const ClassSection& section) {
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

inline bool ClassSectionRepository::remove(const string& maLopHP) {
    NodeDLL<ClassSection>* node = nullptr;
    if (!sectionTable.get(maLopHP, node)) return false;
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

inline bool ClassSectionRepository::exists(const string& maLopHP) const {
    return sectionTable.search(maLopHP);
}

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

inline const DoubleLinkedList<ClassSection>& ClassSectionRepository::getList() const {
    return sectionList;
}

inline void ClassSectionRepository::clear() {
    sectionTable.clear();
    sectionList.clear();
}

#endif // CLASSSECTION_REPOSITORY_H
