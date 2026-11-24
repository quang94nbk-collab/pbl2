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

    cout << "\n>>> ALL REPOSITORY & BUSINESS LOGIC TESTS PASSED PERFECTLY! <<<" << endl;
}

int main() {
    testSystemManagerFlow();
    return 0;
}
