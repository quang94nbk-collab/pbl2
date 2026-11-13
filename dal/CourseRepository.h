#ifndef COURSE_REPOSITORY_H
#define COURSE_REPOSITORY_H

#include "IRepository.h"
#include "../DSA/DoublyLinkedList.h"
#include "../DSA/hashtable.h"
#include "../model/course.h"
#include <string>
using namespace std;

// ============================================================
//  CourseRepository - DAL tang du lieu cho Course (Mon hoc)
//
//  Quan he:
//    - Course doc lap (khong ke thua)
//    - Mot Course co the co nhieu ClassSection (1..n)
//      => ClassSectionRepository giu FK: maMon -> Course
//
//  Chuc nang:
//    - add      : Them mon hoc (kiem tra trung maMon, soTinChi > 0)
//    - remove   : Xoa mon hoc (can kiem tra chua co LopHP nao thuoc)
//    - findById : Tim nhanh qua HashTable
//    - search   : Tim theo tu khoa (maMon, tenMon, khoa, nganh)
//    - displayAll / displayByFilter
//      (da tach sang Presentation; DAL cung cap getList / filter / search)
// ============================================================
class CourseRepository : public IRepository<Course, string> {
private:
    DoubleLinkedList<Course>             courseList;
    HashTable<string, NodeDLL<Course>*>  courseTable;

    NodeDLL<Course>* getLastNode() const;

    static string toLower(string s);

public:
    // ----- IRepository interface -----

    bool add(const Course& course) override;

    // Luu y: truoc khi goi remove, SystemManager phai kiem tra khong con ClassSection
    bool remove(const string& maMon) override;

    Course* findById(const string& maMon) override;
    const Course* findById(const string& maMon) const;

    // ----- Chuc nang mo rong -----

    // Hien thi mon hoc theo bo loc (khoa, khoaHoc, nganh, hocKy)
    // (DAL loc du lieu tra ve outResults)
    void filter(const string& khoa, const string& khoaHoc,
                const string& nganh, int hocKy,
                DoubleLinkedList<Course>& outResults) const;

    // Tim kiem theo tu khoa
    void search(const string& keyword, DoubleLinkedList<Course>& outResults) const;

    // Lay danh sach cac mon hoc phu hop voi sinh vien trong hoc ky
    // (khoa, khoaHoc, nganhHoc cua sinh vien, hocKy hien tai)
    int getMatchingCourses(const string& svKhoa, const string& svKhoaHoc,
                           const string& svNganh, int hocKy,
                           string outIDs[], int maxOut) const;

    bool exists(const string& maMon) const;

    const DoubleLinkedList<Course>& getList() const override;

    void clear();
};

inline NodeDLL<Course>* CourseRepository::getLastNode() const {
    auto cur = courseList.getHead();
    NodeDLL<Course>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

inline string CourseRepository::toLower(string s) {
    return utf8ToLower(s);
}

// ----- IRepository interface -----

inline bool CourseRepository::add(const Course& course) {
    if (course.getMaMon().empty() || course.getSoTinChi() <= 0 ||
        courseTable.search(course.getMaMon()))
        return false;
    courseList.pushBack(course);
    courseTable.insert(course.getMaMon(), getLastNode());
    return true;
}

// Luu y: truoc khi goi remove, SystemManager phai kiem tra khong con ClassSection
inline bool CourseRepository::remove(const string& maMon) {
    NodeDLL<Course>* node = nullptr;
    if (!courseTable.get(maMon, node)) return false;
    courseList.eraseNode(node);
    courseTable.erase(maMon);
    return true;
}

inline Course* CourseRepository::findById(const string& maMon) {
    NodeDLL<Course>* node = nullptr;
    if (courseTable.get(maMon, node)) return &(node->data);
    return nullptr;
}

inline const Course* CourseRepository::findById(const string& maMon) const {
    NodeDLL<Course>* node = nullptr;
    if (courseTable.get(maMon, node)) return &(node->data);
    return nullptr;
}

// ----- Chuc nang mo rong -----

// Hien thi mon hoc theo bo loc (khoa, khoaHoc, nganh, hocKy)
// (DAL loc du lieu tra ve outResults)
inline void CourseRepository::filter(const string& khoa, const string& khoaHoc,
                                     const string& nganh, int hocKy,
                                     DoubleLinkedList<Course>& outResults) const {
    outResults.clear();
    auto cur = courseList.getHead();
    while (cur != nullptr) {
        const Course& c = cur->data;
        bool mK  = khoa.empty()    || c.getKhoa().empty()    || c.getKhoa()    == "Chung" || c.getKhoa()    == khoa;
        bool mKH = khoaHoc.empty() || c.getKhoaHoc().empty() || c.getKhoaHoc() == "Chung" || c.getKhoaHoc() == khoaHoc;
        bool mNg = nganh.empty()   || c.getNganhHoc().empty()|| c.getNganhHoc()== "Chung" || c.getNganhHoc()== nganh;
        bool mHK = (hocKy == 0)    || (c.getHocKy() == hocKy);
        if (mK && mKH && mNg && mHK) { outResults.pushBack(c); }
        cur = cur->next;
    }
}

// Tim kiem theo tu khoa
inline void CourseRepository::search(const string& keyword, DoubleLinkedList<Course>& outResults) const {
    outResults.clear();
    string kw = toLower(keyword);
    auto cur = courseList.getHead();
    while (cur != nullptr) {
        string id   = toLower(cur->data.getMaMon());
        string ten  = toLower(cur->data.getTenMon());
        string khoa = toLower(cur->data.getKhoa());
        string ng   = toLower(cur->data.getNganhHoc());
        if (id.find(kw) != string::npos || ten.find(kw) != string::npos ||
            khoa.find(kw)!= string::npos || ng.find(kw) != string::npos) {
            outResults.pushBack(cur->data);
        }
        cur = cur->next;
    }
}

// Lay danh sach cac mon hoc phu hop voi sinh vien trong hoc ky
// (khoa, khoaHoc, nganhHoc cua sinh vien, hocKy hien tai)
inline int CourseRepository::getMatchingCourses(const string& svKhoa, const string& svKhoaHoc,
                                                const string& svNganh, int hocKy,
                                                string outIDs[], int maxOut) const {
    int count = 0;
    auto cur = courseList.getHead();
    while (cur != nullptr) {
        const Course& c = cur->data;
        if (c.getHocKy() == hocKy || c.getHocKy() == 0) {
            bool mK  = c.getKhoa().empty()    || c.getKhoa()    == "Chung" || svKhoa.empty()    || svKhoa    == c.getKhoa();
            bool mKH = c.getKhoaHoc().empty() || c.getKhoaHoc() == "Chung" || svKhoaHoc.empty() || svKhoaHoc == c.getKhoaHoc();
            bool mNg = c.getNganhHoc().empty()|| c.getNganhHoc()== "Chung" || svNganh.empty()   || svNganh   == c.getNganhHoc();
            if (mK && mKH && mNg && count < maxOut)
                outIDs[count++] = c.getMaMon();
        }
        cur = cur->next;
    }
    return count;
}

inline bool CourseRepository::exists(const string& maMon) const {
    return courseTable.search(maMon);
}

inline const DoubleLinkedList<Course>& CourseRepository::getList() const {
    return courseList;
}

inline void CourseRepository::clear() {
    courseTable.clear();
    courseList.clear();
}

#endif // COURSE_REPOSITORY_H
