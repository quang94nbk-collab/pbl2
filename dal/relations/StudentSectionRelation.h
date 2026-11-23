#ifndef STUDENT_SECTION_RELATION_H
#define STUDENT_SECTION_RELATION_H

#include "../repositories/StudentRepository.h"
#include "../repositories/ClassSectionRepository.h"
#include "../repositories/RegistrationRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include <string>
using namespace std;

// ============================================================
//  StudentSectionRelation - Quan ly quan he M-N giua Student va ClassSection
//  (thong qua Registration)
//  Chiu trach nhiem:
//    - Kiem tra trang thai dang ky
//    - Truy van dang ky theo sinh vien / theo lop hoc phan
//    - Xoa xep tang (Cascade delete) khi sinh vien hoac lop hoc phan bi xoa
// ============================================================
class StudentSectionRelation {
public:
    // Kiem tra sinh vien da dang ky vao lop hoc phan chua
    bool isRegistered(const string& maSV, const string& maLopHP,
                      const RegistrationRepository& regRepo) const {
        return regRepo.isRegistered(maSV, maLopHP);
    }

    // Lay danh sach ma lop hoc phan ma sinh vien da dang ky
    int getSectionsByStudent(const string& maSV, const RegistrationRepository& regRepo,
                             string outSectionIDs[], int maxCount) const {
        return regRepo.getSectionsByStudent(maSV, outSectionIDs, maxCount);
    }

    // Lay danh sach ma sinh vien da dang ky vao lop hoc phan
    int getStudentsBySection(const string& maLopHP, const RegistrationRepository& regRepo,
                             string outStudentIDs[], int maxCount) const {
        return regRepo.getStudentsBySection(maLopHP, outStudentIDs, maxCount);
    }

    // Lay danh sach doi tuong Registration theo sinh vien
    void getRegistrationsByStudent(const string& maSV, const RegistrationRepository& regRepo,
                                  DoubleLinkedList<Registration>& outResults) const {
        regRepo.getByStudent(maSV, outResults);
    }

    // Lay danh sach doi tuong Registration theo lop hoc phan
    void getRegistrationsBySection(const string& maLopHP, const RegistrationRepository& regRepo,
                                  DoubleLinkedList<Registration>& outResults) const {
        regRepo.getBySection(maLopHP, outResults);
    }

    // Xoa xep tang khi xoa lop hoc phan: Xoa tat ca ban ghi dang ky cua lop nay
    void cascadeDeleteSection(const string& maLopHP, RegistrationRepository& regRepo) const {
        regRepo.removeAllBySection(maLopHP);
    }

    // Xoa xep tang khi xoa sinh vien:
    // Hoan tra si so cho cac lop ma sinh vien da dang ky, sau do xoa cac ban ghi dang ky
    void cascadeDeleteStudent(const string& maSV, RegistrationRepository& regRepo,
                              ClassSectionRepository& sectionRepo) const {
        const int MAX = 100;
        string secIDs[MAX];
        int cnt = regRepo.getSectionsByStudent(maSV, secIDs, MAX);
        for (int i = 0; i < cnt; i++) {
            ClassSection* sec = sectionRepo.findById(secIDs[i]);
            if (sec != nullptr) {
                sec->giamSoLuong();
            }
        }
        regRepo.removeAllByStudent(maSV);
    }
};

#endif // STUDENT_SECTION_RELATION_H
