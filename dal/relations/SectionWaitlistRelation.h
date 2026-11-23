#ifndef SECTION_WAITLIST_RELATION_H
#define SECTION_WAITLIST_RELATION_H

#include "../repositories/ClassSectionRepository.h"
#include "../../DSA/hashtable.h"
#include "../../DSA/queue.h"
#include "../../DSA/DoublyLinkedList.h"
#include <string>
using namespace std;

// ============================================================
//  SectionWaitlistRelation - Quan ly quan he Cho (Waitlist)
//  Anh xa 1 ClassSection -> 1 Queue<string> chua cac maSV dang cho
//  Chiu trach nhiem:
//    - Quan ly hang doi cho (FIFO) cua tung lop hoc phan
//    - Them, rut, kiem tra sinh vien trong danh sach cho
//    - Tu dong rut sinh vien khoi cac lop cung mon khi da dang ky thanh cong
//    - Don dep hang doi khi lop hoc phan bi xoa
// ============================================================
class SectionWaitlistRelation {
private:
    HashTable<string, Queue<string>*> waitlistTable;
    DoubleLinkedList<string>          sectionKeys;

public:
    SectionWaitlistRelation() {}

    ~SectionWaitlistRelation() {
        clear();
    }

    // Lay con tro hang doi cua mot lop hoc phan
    Queue<string>* getWaitlist(const string& maLopHP) {
        Queue<string>* q = nullptr;
        waitlistTable.get(maLopHP, q);
        return q;
    }

    const Queue<string>* getWaitlist(const string& maLopHP) const {
        Queue<string>* q = nullptr;
        waitlistTable.get(maLopHP, q);
        return q;
    }

    // Khoi tao hoac lay hang doi cho cua mot lop hoc phan
    Queue<string>* ensureWaitlist(const string& maLopHP) {
        Queue<string>* q = nullptr;
        if (waitlistTable.get(maLopHP, q)) return q;
        q = new Queue<string>();
        waitlistTable.insert(maLopHP, q);
        sectionKeys.pushBack(maLopHP);
        return q;
    }

    bool hasWaitlist(const string& maLopHP) const {
        const Queue<string>* q = getWaitlist(maLopHP);
        return q != nullptr && !q->empty();
    }

    int getWaitlistCount(const string& maLopHP) const {
        const Queue<string>* q = getWaitlist(maLopHP);
        return q ? q->size() : 0;
    }

    bool contains(const string& maLopHP, const string& maSV) const {
        const Queue<string>* q = getWaitlist(maLopHP);
        return q != nullptr && q->contains(maSV);
    }

    bool push(const string& maLopHP, const string& maSV) {
        Queue<string>* q = ensureWaitlist(maLopHP);
        if (q->contains(maSV)) return false;
        q->push(maSV);
        return true;
    }

    bool pop(const string& maLopHP, string& outSV) {
        Queue<string>* q = getWaitlist(maLopHP);
        if (!q || q->empty()) return false;
        return q->pop(outSV);
    }

    bool removeFromWaitlist(const string& maLopHP, const string& maSV) {
        Queue<string>* q = getWaitlist(maLopHP);
        if (!q) return false;
        return q->remove(maSV);
    }

    // Xoa hang doi cho cua mot lop khi lop bi xoa
    void clearSectionWaitlist(const string& maLopHP) {
        Queue<string>* q = nullptr;
        if (waitlistTable.get(maLopHP, q)) {
            delete q;
            waitlistTable.erase(maLopHP);
        }
        sectionKeys.eraseValue(maLopHP);
    }

    // Xoa sinh vien khoi tat ca danh sach cho cua moi lop
    void removeStudentFromAll(const string& maSV, const ClassSectionRepository& sectionRepo) {
        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            Queue<string>* q = getWaitlist(cur->data.getMaLopHP());
            if (q != nullptr) q->remove(maSV);
            cur = cur->next;
        }
    }

    // Rut sinh vien khoi tat ca waitlist cua cac lop hoc phan thuoc cung mot mon hoc
    int removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                        const ClassSectionRepository& sectionRepo,
                                        const string& exceptSectionID = "",
                                        DoubleLinkedList<string>* outRemovedWaitlists = nullptr) {
        int removedCount = 0;
        auto cur = sectionRepo.getList().getHead();
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

    void clear() {
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
    }
};

#endif // SECTION_WAITLIST_RELATION_H
