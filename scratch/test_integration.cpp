#include <iostream>
#include <cassert>
#include "manager/system.h"

using namespace std;

void testSystemManagerFlow() {
    cout << "=== RUNNING INTEGRATION TEST FOR MERGED ARCHITECTURE ===" << endl;
    SystemManager sm;
    sm.setCurrentSemester(1);

    // 1. Add Students: Student(maSV, ten, sdt, mail, lop, nganh, khoa, khoaHoc)
    Student s1("SV01", "Nguyen Van A", "0123456789", "a@test.com", "21TCLC_DT1", "CNTT", "CNTT", "K21");
    Student s2("SV02", "Tran Thi B",   "0987654321", "b@test.com", "21TCLC_DT1", "CNTT", "CNTT", "K21");
    assert(sm.addStudent(s1));
    assert(sm.addStudent(s2));
    assert(sm.findStudent("SV01") != nullptr);
    assert(sm.findStudent("SV02") != nullptr);
    cout << "[PASS] Add Students" << endl;

    // 2. Add Course: Course(ma, ten, tinChi, k, kh, ng, hk)
    Course c1("CS101", "Lap trinh C++", 3, "CNTT", "K21", "CNTT", 1);
    assert(sm.addCourse(c1));
    assert(sm.findCourse("CS101") != nullptr);
    cout << "[PASS] Add Course" << endl;

    // 3. Add ClassSection with capacity = 1
    // ClassSection(maLop, maMonHoc, gv, ngayHoc, start, end, phong, maxSV, hk, group)
    ClassSection sec1("HP01", "CS101", "Thay An", "Hai", 1, 3, "A101", 1, 1);
    assert(sm.addClassSection(sec1));
    ClassSection* foundSec = sm.findClassSection("HP01");
    assert(foundSec != nullptr);
    // Verify OOP link: Section has MonHoc pointer
    assert(foundSec->getMonHoc() != nullptr);
    assert(foundSec->getMonHoc()->getMaMon() == "CS101");
    // Verify OOP link: Course has Section pointer
    Course* foundCourse = sm.findCourse("CS101");
    assert(!foundCourse->getDanhSachLopHP().empty());
    assert(foundCourse->getDanhSachLopHP().getHead()->data->getMaLopHP() == "HP01");
    cout << "[PASS] Add ClassSection & OOP Object Linking (Course <-> Section)" << endl;

    // 4. Test Course deletion restriction (cannot delete course if section exists)
    string errReason;
    assert(!sm.deleteCourse("CS101", &errReason));
    cout << "[PASS] Course deletion prevented when class section exists: " << errReason << endl;

    // 5. Register SV01 to HP01 (succeeds)
    assert(sm.registerClass("SV01", "HP01", "2026-11-24"));
    assert(sm.isSectionRegistered("SV01", "HP01"));
    assert(foundSec->getSoLuongDaDangKy() == 1);
    assert(!foundSec->conCho());
    cout << "[PASS] Register SV01 to HP01 (Class is now full)" << endl;

    // 6. Register SV02 to HP01 (Class is full -> Waitlist exception)
    bool threwFull = false;
    try {
        sm.registerClass("SV02", "HP01", "2026-11-24");
    } catch (const ClassFullException& e) {
        threwFull = true;
    }
    assert(threwFull);
    assert(sm.isStudentInWaitlist("SV02", "HP01"));
    assert(sm.hasWaitlist("HP01"));
    assert(sm.getWaitlistCount("HP01") == 1);
    cout << "[PASS] SV02 automatically placed into Waitlist" << endl;

    // 7. Cancel SV01 -> SV02 automatically promoted from waitlist!
    DoubleLinkedList<string> notices;
    assert(sm.cancelRegistration("SV01", "HP01", &notices));
    assert(!sm.isSectionRegistered("SV01", "HP01"));
    assert(sm.isSectionRegistered("SV02", "HP01")); // SV02 auto-promoted!
    assert(!sm.hasWaitlist("HP01")); // Waitlist is now empty
    cout << "[PASS] SV01 canceled, SV02 auto-promoted from waitlist" << endl;

    // 8. Delete Section cascades registrations & waitlists
    assert(sm.deleteClassSection("HP01"));
    assert(sm.findClassSection("HP01") == nullptr);
    assert(!sm.isSectionRegistered("SV02", "HP01"));
    assert(foundCourse->getDanhSachLopHP().empty());
    cout << "[PASS] ClassSection deleted & cascaded registrations/waitlists" << endl;

    // 9. Course can now be safely deleted
    assert(sm.deleteCourse("CS101"));
    assert(sm.findCourse("CS101") == nullptr);
    cout << "[PASS] Course safely deleted when no sections remain" << endl;

    // 10. Delete student
    assert(sm.deleteStudent("SV01"));
    assert(sm.deleteStudent("SV02"));
    assert(sm.findStudent("SV01") == nullptr);
    assert(sm.findStudent("SV02") == nullptr);
    cout << "[PASS] Students deleted cleanly" << endl;

    // 11. Regression tests for fixed logic bugs
    // 11a. Rule of Three deep copy on Queue and HashTable
    {
        Queue<string> q1;
        q1.push("A");
        q1.push("B");
        Queue<string> q2(q1);
        Queue<string> q3;
        q3 = q1;
        string popped;
        assert(q1.pop(popped) && popped == "A");
        assert(q2.size() == 2 && q3.size() == 2);

        HashTable<string, int> ht1;
        ht1.insert("k1", 10);
        ht1.insert("k2", 20);
        HashTable<string, int> ht2(ht1);
        HashTable<string, int> ht3;
        ht3 = ht1;
        ht1.erase("k1");
        int v = 0;
        assert(ht2.get("k1", v) && v == 10);
        assert(ht3.get("k1", v) && v == 10);
    }
    cout << "[PASS] Queue & HashTable Rule of Three deep-copy" << endl;

    // 11b. Inclusive period conflict [1,3] vs [3,5], single-period [4,4], and day normalization
    {
        Student svA("102250001", "SV A", "0900000001", "a@sv.vn", "25T1", "CNTT", "CNTT", "K25");
        Student svB("102250002", "SV B", "0900000002", "b@sv.vn", "25T1", "CNTT", "CNTT", "K25");
        Student svC("102250003", "SV C", "0900000003", "c@sv.vn", "25T1", "CNTT", "CNTT", "K25");
        assert(svA.getHocKyTienTrinh(1) == 1 && svA.getHocKyTienTrinh(2) == 2);
        Student svK23("102230099", "SV K23", "0900000099", "k23@sv.vn", "23T1", "CNTT", "CNTT", "K23");
        assert(svK23.getHocKyTienTrinh(1) == 5 && svK23.getHocKyTienTrinh(2) == 6);

        assert(sm.addStudent(svA));
        assert(sm.addStudent(svB));
        assert(sm.addStudent(svC));
        assert(sm.addCourse(Course("M1", "Mon 1", 3, "CNTT", "K25", "CNTT", 1)));
        assert(sm.addCourse(Course("M2", "Mon 2", 3, "CNTT", "K25", "CNTT", 1)));
        assert(sm.addCourse(Course("M3", "Mon 3", 1, "CNTT", "K25", "CNTT", 1)));

        // Single-period section [4,4] allowed, day "Thứ 2" normalized to "2"
        assert(sm.addClassSection(ClassSection("M1_L1", "M1", "GV1", "Thứ 2", 1, 3, "F101", 1, 1)));
        assert(sm.addClassSection(ClassSection("M2_L1", "M2", "GV2", "2",     3, 5, "F102", 5, 1)));
        assert(sm.addClassSection(ClassSection("M3_L1", "M3", "GV3", "2",     4, 4, "F103", 5, 1)));

        assert(sm.registerClass("102250001", "M1_L1", "2026-11-25"));
        // Boundary period 3 must conflict!
        bool conflictCaught = false;
        try {
            sm.registerClass("102250001", "M2_L1", "2026-11-25");
        } catch (const TimeConflictException&) {
            conflictCaught = true;
        }
        assert(conflictCaught);
        // Period 4 does NOT conflict with [1,3]
        assert(sm.registerClass("102250001", "M3_L1", "2026-11-25"));

        // 11c. SystemManager::removeStudentFromWaitlist parameter order & waitlist retention on conflict
        // Put svB and svC into waitlist of M1_L1 (full, cap=1)
        try { sm.registerClass("102250002", "M1_L1", "2026-11-25"); } catch (const ClassFullException&) {}
        try { sm.registerClass("102250003", "M1_L1", "2026-11-25"); } catch (const ClassFullException&) {}
        assert(sm.getWaitlistCount("M1_L1") == 2);

        // Give svB a conflicting class M2_L1 (Thu 2, Tiet 3-5) AFTER joining waitlist
        assert(sm.registerClass("102250002", "M2_L1", "2026-11-25"));

        // Cancel svA from M1_L1 -> svB has conflict so stays in waitlist, svC gets promoted!
        DoubleLinkedList<string> wNotices;
        assert(sm.cancelRegistration("102250001", "M1_L1", &wNotices));
        assert(sm.isSectionRegistered("102250003", "M1_L1"));
        assert(sm.isStudentInWaitlist("102250002", "M1_L1"));

        // Now test SystemManager::removeStudentFromWaitlist(maSV, maLopHP)
        assert(sm.removeStudentFromWaitlist("102250002", "M1_L1"));
        assert(!sm.isStudentInWaitlist("102250002", "M1_L1"));
    }
    cout << "[PASS] Inclusive schedule conflict, 1-period section, semester progress & waitlist fixes" << endl;

    cout << "\n>>> ALL REPOSITORY & BUSINESS LOGIC TESTS PASSED PERFECTLY! <<<" << endl;
}

int main() {
    testSystemManagerFlow();
    return 0;
}
