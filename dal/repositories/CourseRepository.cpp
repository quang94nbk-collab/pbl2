#include "CourseRepository.h"
#include "ClassSectionRepository.h"

NodeDLL<Course>* CourseRepository::getLastNode() const {
    auto cur = courseList.getHead();
    NodeDLL<Course>* tail = nullptr;
    while (cur != nullptr) { tail = cur; cur = cur->next; }
    return tail;
}

string CourseRepository::toLower(string s) {
    return utf8ToLower(s);
}

bool CourseRepository::add(const Course& course) {
    if (course.getMaMon().empty() || course.getSoTinChi() < 0 ||
        courseTable.search(course.getMaMon()))
        return false;
    courseList.pushBack(course);
    courseTable.insert(course.getMaMon(), getLastNode());
    return true;
}

bool CourseRepository::remove(const string& maMon) {
    NodeDLL<Course>* node = nullptr;
    if (!courseTable.get(maMon, node)) return false;
    courseList.eraseNode(node);
    courseTable.erase(maMon);
    return true;
}

Course* CourseRepository::findById(const string& maMon) {
    NodeDLL<Course>* node = nullptr;
    if (courseTable.get(maMon, node)) return &(node->data);
    return nullptr;
}

const Course* CourseRepository::findById(const string& maMon) const {
    NodeDLL<Course>* node = nullptr;
    if (courseTable.get(maMon, node)) return &(node->data);
    return nullptr;
}

bool CourseRepository::exists(const string& maMon) const {
    return courseTable.search(maMon);
}

void CourseRepository::filter(const string& khoa, const string& khoaHoc,
                             const string& nganh, int hocKy,
                             DoubleLinkedList<Course>& outResults) const {
    outResults.clear();
    auto cur = courseList.getHead();
    while (cur != nullptr) {
        const Course& c = cur->data;
        bool mK  = khoa.empty()    || c.getKhoa().empty()    || c.getKhoa()    == "Chung" || toLower(c.getKhoa())    == toLower(khoa);
        bool mKH = khoaHoc.empty() || c.getKhoaHoc().empty() || c.getKhoaHoc() == "Chung" || toLower(c.getKhoaHoc()) == toLower(khoaHoc);
        bool mNg = nganh.empty()   || c.getNganhHoc().empty()|| c.getNganhHoc()== "Chung" || toLower(c.getNganhHoc())== toLower(nganh);
        bool mHK = (hocKy == 0)    || (c.getHocKy() == hocKy);
        if (mK && mKH && mNg && mHK) { outResults.pushBack(c); }
        cur = cur->next;
    }
}

void CourseRepository::search(const string& keyword, DoubleLinkedList<Course>& outResults) const {
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

int CourseRepository::getMatchingCourses(const string& svKhoa, const string& svKhoaHoc,
                                        const string& svNganh, int hocKy,
                                        string outIDs[], int maxOut) const {
    int count = 0;
    auto cur = courseList.getHead();
    while (cur != nullptr) {
        const Course& c = cur->data;
        bool semMatch = (hocKy == 0) || (c.getHocKy() == 0) || (c.getHocKy() == hocKy) ||
                        (hocKy == 1 && c.getHocKy() % 2 == 1) ||
                        (hocKy == 2 && c.getHocKy() % 2 == 0);
        if (semMatch) {
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

bool CourseRepository::canRemoveCourse(const Course* course, string* outMessage) const {
    if (!course) return true;
    if (course->getSoLopHocPhan() > 0) {
        if (outMessage != nullptr) {
            *outMessage = "Khong the xoa: Mon hoc '" + course->getTenMon() + "' dang co " +
                          to_string(course->getSoLopHocPhan()) + " lop hoc phan truc thuoc. Vui long xoa cac lop hoc phan truoc!";
        }
        return false;
    }
    return true;
}

bool CourseRepository::canRemoveCourse(const string& maMon, const ClassSectionRepository& sectionRepo, string* outMessage) const {
    const Course* c = findById(maMon);
    if (c != nullptr) {
        return canRemoveCourse(c, outMessage);
    }
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

const DoubleLinkedList<Course>& CourseRepository::getList() const {
    return courseList;
}

void CourseRepository::clear() {
    courseTable.clear();
    courseList.clear();
}
