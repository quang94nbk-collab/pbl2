#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include <string>
#include "../DSA/DoublyLinkedList.h"
#include "../model/course.h"

class CourseRepository;
class ClassSectionRepository;

class CourseService {
private:
    CourseRepository&       courseRepo;
    ClassSectionRepository& sectionRepo;

    template <typename T>
    static void copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst);

public:
    CourseService(CourseRepository& cRepo, ClassSectionRepository& csRepo);

    bool addCourse(const Course& c);
    bool deleteCourse(const std::string& maMon, std::string* outMessage = nullptr);
    Course* findCourse(const std::string& maMon);
    const Course* findCourse(const std::string& maMon) const;
    void getAllCourses(DoubleLinkedList<Course>& outList) const;
    void searchCourses(const std::string& kw, DoubleLinkedList<Course>& outList) const;
    void filterCourses(const std::string& khoa, const std::string& khoaHoc,
                       const std::string& nganh, int hocKy,
                       DoubleLinkedList<Course>& outList) const;
};

#endif // COURSE_SERVICE_H
