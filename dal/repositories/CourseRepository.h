#ifndef COURSE_REPOSITORY_H
#define COURSE_REPOSITORY_H

#include "IRepository.h"
#include "../../DSA/DoublyLinkedList.h"
#include "../../DSA/hashtable.h"
#include "../../model/course.h"
#include <string>
using namespace std;

class ClassSectionRepository;

// ============================================================
//  CourseRepository - Pure Repository cho thuc the Course
// ============================================================
class CourseRepository : public IRepository<Course, string> {
private:
    DoubleLinkedList<Course>             courseList;
    HashTable<string, NodeDLL<Course>*>  courseTable;

    NodeDLL<Course>* getLastNode() const;
    static string toLower(string s);

public:
    bool add(const Course& course) override;
    bool remove(const string& maMon) override;
    Course* findById(const string& maMon) override;
    const Course* findById(const string& maMon) const;
    bool exists(const string& maMon) const override;

    void filter(const string& khoa, const string& khoaHoc,
                const string& nganh, int hocKy,
                DoubleLinkedList<Course>& outResults) const;
    void search(const string& keyword, DoubleLinkedList<Course>& outResults) const;
    int getMatchingCourses(const string& svKhoa, const string& svKhoaHoc,
                           const string& svNganh, int hocKy,
                           string outIDs[], int maxOut) const;

    // Kiem tra rang buoc toan ven khi xoa mon hoc (co con lop hoc phan truc thuoc khong)
    bool canRemoveCourse(const Course* course, string* outMessage = nullptr) const;
    bool canRemoveCourse(const string& maMon, const ClassSectionRepository& sectionRepo, string* outMessage = nullptr) const;

    const DoubleLinkedList<Course>& getList() const override;
    void clear();
};

#endif // COURSE_REPOSITORY_H
