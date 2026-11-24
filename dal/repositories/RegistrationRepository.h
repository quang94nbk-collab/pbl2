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

#endif // REGISTRATION_REPOSITORY_H
