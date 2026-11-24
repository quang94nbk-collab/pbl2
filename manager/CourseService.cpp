#include "CourseService.h"
#include "../dal/repositories/CourseRepository.h"
#include "../dal/repositories/ClassSectionRepository.h"

template <typename T>
void CourseService::copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst) {
    dst.clear();
    auto cur = src.getHead();
    while (cur != nullptr) {
        dst.pushBack(cur->data);
        cur = cur->next;
    }
}

CourseService::CourseService(CourseRepository& cRepo, ClassSectionRepository& csRepo)
    : courseRepo(cRepo), sectionRepo(csRepo) {}

bool CourseService::addCourse(const Course& c) {
    return courseRepo.add(c);
}

bool CourseService::deleteCourse(const std::string& maMon, std::string* outMessage) {
    Course* c = findCourse(maMon);
    if (c != nullptr) {
        if (!courseRepo.canRemoveCourse(c, outMessage)) {
            return false;
        }
    } else {
        if (!courseRepo.canRemoveCourse(maMon, sectionRepo, outMessage)) {
            return false;
        }
    }
    return courseRepo.remove(maMon);
}

Course* CourseService::findCourse(const std::string& maMon) {
    return courseRepo.findById(maMon);
}

const Course* CourseService::findCourse(const std::string& maMon) const {
    return courseRepo.findById(maMon);
}

void CourseService::getAllCourses(DoubleLinkedList<Course>& outList) const {
    copyList(courseRepo.getList(), outList);
}

void CourseService::searchCourses(const std::string& kw, DoubleLinkedList<Course>& outList) const {
    courseRepo.search(kw, outList);
}

void CourseService::filterCourses(const std::string& khoa, const std::string& khoaHoc,
                                  const std::string& nganh, int hocKy,
                                  DoubleLinkedList<Course>& outList) const {
    courseRepo.filter(khoa, khoaHoc, nganh, hocKy, outList);
}
