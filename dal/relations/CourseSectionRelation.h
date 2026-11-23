#ifndef COURSE_SECTION_RELATION_H
#define COURSE_SECTION_RELATION_H

#include "../repositories/CourseRepository.h"
#include "../repositories/ClassSectionRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include <string>
using namespace std;

// ============================================================
//  CourseSectionRelation - Quan ly quan he 1-N giua Course va ClassSection
//  Chiu trach nhiem:
//    - Kiem tra tinh hop le khi gan lop hoc phan vao mon hoc (FK)
//    - Kiem tra rang buoc toan ven khi xoa mon hoc (co con lop HP khong)
//    - Truy van cac lop hoc phan thuoc ve mot mon hoc
// ============================================================
class CourseSectionRelation {
public:
    // Kiem tra maMon cua lop hoc phan co ton tai trong CourseRepository hay khong
    bool isValidSectionCourse(const string& maMon, const CourseRepository& courseRepo) const {
        return courseRepo.exists(maMon);
    }

    // Kiem tra mon hoc co the xoa duoc khong (true neu khong con lop HP nao thuoc mon nay)
    bool canRemoveCourse(const string& maMon, const ClassSectionRepository& sectionRepo, string* outMessage = nullptr) const {
        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaMon() == maMon) {
                if (outMessage != nullptr) {
                    *outMessage = "Khong the xoa: Lop hoc phan '" + cur->data.getMaLopHP() +
                                  "' dang thuoc mon nay. Vui long xoa lop hoc phan truoc!";
                }
                return false;
            }
            cur = cur->next;
        }
        return true;
    }

    // Dem so lop hoc phan thuoc mot mon hoc
    int countSectionsByCourse(const string& maMon, const ClassSectionRepository& sectionRepo) const {
        int count = 0;
        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaMon() == maMon) count++;
            cur = cur->next;
        }
        return count;
    }

    // Lay danh sach tat ca cac lop hoc phan thuoc ve mot mon hoc
    void getSectionsByCourse(const string& maMon, const ClassSectionRepository& sectionRepo,
                             DoubleLinkedList<ClassSection>& outList) const {
        outList.clear();
        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaMon() == maMon) {
                outList.pushBack(cur->data);
            }
            cur = cur->next;
        }
    }
};

#endif // COURSE_SECTION_RELATION_H
