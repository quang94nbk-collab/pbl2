#ifndef CLASSSECTION_REPOSITORY_H
#define CLASSSECTION_REPOSITORY_H

#include "IRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include "../../DSA/hashtable.h"
#include "../../DSA/queue.h"
#include "../../model/classsection.h"
#include <string>
using namespace std;

// ============================================================
//  ClassSectionRepository - Pure Repository cho ClassSection
//  (Quan ly thong tin Lop hoc phan va Hang doi cho Waitlist)
// ============================================================
class ClassSectionRepository : public IRepository<ClassSection, string> {
private:
    DoubleLinkedList<ClassSection>             sectionList;
    HashTable<string, NodeDLL<ClassSection>*> sectionTable;

    // Quan ly hang doi cho (FIFO Queue) cua tung lop hoc phan
    HashTable<string, Queue<string>*> waitlistTable;
    DoubleLinkedList<string>          sectionKeys;

    NodeDLL<ClassSection>* getLastNode() const;
    static string toLower(string s);

public:
    ClassSectionRepository();
    ~ClassSectionRepository();

    bool add(const ClassSection& section) override;
    bool remove(const string& maLopHP) override;
    ClassSection* findById(const string& maLopHP) override;
    const ClassSection* findById(const string& maLopHP) const;
    bool exists(const string& maLopHP) const override;

    void search(const string& keyword, DoubleLinkedList<ClassSection>& outResults) const;
    int getGroupsForSemester(int hocKy, string groupNames[], int maxGroups) const;
    const DoubleLinkedList<ClassSection>& getList() const override;
    void clear();

    // =========================================================
    //  Quan ly Hang doi cho (Waitlist)
    // =========================================================
    Queue<string>* getWaitlist(const string& maLopHP);
    const Queue<string>* getWaitlist(const string& maLopHP) const;
    Queue<string>* ensureWaitlist(const string& maLopHP);

    bool hasWaitlist(const string& maLopHP) const;
    int getWaitlistCount(const string& maLopHP) const;
    bool isInWaitlist(const string& maLopHP, const string& maSV) const;
    bool pushToWaitlist(const string& maLopHP, const string& maSV);
    bool popFromWaitlist(const string& maLopHP, string& outSV);
    bool removeFromWaitlist(const string& maLopHP, const string& maSV);

    void clearSectionWaitlist(const string& maLopHP);
    void removeStudentFromAllWaitlists(const string& maSV);
    int removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                        const string& exceptSectionID = "",
                                        DoubleLinkedList<string>* outRemovedWaitlists = nullptr);
};

#endif // CLASSSECTION_REPOSITORY_H
