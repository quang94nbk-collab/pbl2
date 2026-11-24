#include "RegistrationService.h"
#include <ctime>
#include <sstream>
#include <string>
#include "exception.h"
#include "../dal/repositories/StudentRepository.h"
#include "../dal/repositories/CourseRepository.h"
#include "../dal/repositories/ClassSectionRepository.h"
#include "../dal/repositories/RegistrationRepository.h"

using namespace std;

template <typename T>
void RegistrationService::copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst) {
    dst.clear();
    auto cur = src.getHead();
    while (cur != nullptr) {
        dst.pushBack(cur->data);
        cur = cur->next;
    }
}

string RegistrationService::today() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    if (localTime == nullptr) return "";
    char buf[16];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d", localTime) == 0) return "";
    return string(buf);
}

RegistrationService::RegistrationService(StudentRepository& sRepo,
                                         CourseRepository& cRepo,
                                         ClassSectionRepository& csRepo,
                                         RegistrationRepository& rRepo,
                                         int& curSem)
    : studentRepo(sRepo), courseRepo(cRepo), sectionRepo(csRepo),
      registrationRepo(rRepo), currentSemester(curSem) {}

string RegistrationService::getRegisteredSectionForCourse(const string& maSV, const string& maMon) const {
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* sec = sectionRepo.findById(cur->data.getMaLopHP());
            if (sec && sec->getMaMon() == maMon)
                return sec->getMaLopHP();
        }
        cur = cur->next;
    }
    return "";
}

bool RegistrationService::isSectionRegistered(const string& maSV, const string& maLopHP) const {
    return registrationRepo.isRegistered(maSV, maLopHP);
}

bool RegistrationService::isStudentInWaitlist(const string& maSV, const string& maLopHP) const {
    return sectionRepo.isInWaitlist(maLopHP, maSV);
}

int RegistrationService::getStudentRegisteredSections(const string& maSV, string outSectionIDs[], int maxCount) const {
    return registrationRepo.getSectionsByStudent(maSV, outSectionIDs, maxCount);
}

bool RegistrationService::isTimeConflict(const string& maSV, const string& newSectionID) const {
    const ClassSection* target = sectionRepo.findById(newSectionID);
    if (!target) return false;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* existing = sectionRepo.findById(cur->data.getMaLopHP());
            if (existing && existing->getHocKy() == target->getHocKy() &&
                existing->trungLichVoi(*target)) {
                return true;
            }
        }
        cur = cur->next;
    }
    return false;
}

bool RegistrationService::registerClass(const string& maSV, const string& maLopHP,
                                        const string& registrationDate, bool checkSemester,
                                        DoubleLinkedList<string>* outRemovedWaitlists) {
    Student*      student = studentRepo.findById(maSV);
    ClassSection* section = sectionRepo.findById(maLopHP);

    if (!student)
        throw NotFoundException("Khong tim thay sinh vien voi ma: " + maSV);
    if (!section)
        throw NotFoundException("Khong tim thay lop hoc phan voi ma: " + maLopHP);

    if (checkSemester && section->getHocKy() != 0 &&
        (section->getHocKy() != currentSemester && (section->getHocKy() % 2) != (currentSemester % 2)))
        throw WrongSemesterException(
            "Lop hoc phan '" + maLopHP + "' mo cho Hoc ky " +
            to_string(section->getHocKy()) +
            ", trong khi he thong dang o Hoc ky " + to_string(currentSemester));

    Course* course = courseRepo.findById(section->getMaMon());
    if (course) {
        if (checkSemester && course->getHocKy() != 0 && section->getHocKy() != 0) {
            bool matchesSem = (course->getHocKy() == section->getHocKy()) ||
                              ((course->getHocKy() % 2) == (section->getHocKy() % 2));
            if (!matchesSem)
                throw WrongSemesterException(
                    "Mon hoc '" + course->getTenMon() + "' thuoc chuong trinh Hoc ky " +
                    to_string(course->getHocKy()) +
                    ", trong khi he thong dang o Hoc ky " + to_string(currentSemester));
        }

        if (!course->getKhoa().empty() && course->getKhoa() != "Chung") {
            if (student->getKhoa().empty() || student->getKhoa() != course->getKhoa())
                throw NotEligibleException(
                    "Mon hoc thuoc Khoa '" + course->getKhoa() +
                    "' (Khoa cua sinh vien: '" +
                    (student->getKhoa().empty() ? "Chua cap nhat" : student->getKhoa()) + "')");
        }
        if (!course->getKhoaHoc().empty() && course->getKhoaHoc() != "Chung") {
            if (student->getKhoaHoc().empty() || student->getKhoaHoc() != course->getKhoaHoc())
                throw NotEligibleException(
                    "Mon hoc ap dung cho Khoa hoc '" + course->getKhoaHoc() +
                    "' (Khoa hoc cua sinh vien: '" +
                    (student->getKhoaHoc().empty() ? "Chua cap nhat" : student->getKhoaHoc()) + "')");
        }
        if (!course->getNganhHoc().empty() && course->getNganhHoc() != "Chung") {
            if (student->getNganhHoc().empty() || student->getNganhHoc() != course->getNganhHoc())
                throw NotEligibleException(
                    "Mon hoc ap dung cho Nganh '" + course->getNganhHoc() +
                    "' (Nganh cua sinh vien: '" +
                    (student->getNganhHoc().empty() ? "Chua cap nhat" : student->getNganhHoc()) + "')");
        }
    }

    if (registrationRepo.isRegistered(maSV, maLopHP))
        throw DuplicateException("Sinh vien da dang ky lop hoc phan '" + maLopHP + "' roi");

    string oldSecID = getRegisteredSectionForCourse(maSV, section->getMaMon());
    if (!oldSecID.empty() && oldSecID != maLopHP) {
        Course* c = courseRepo.findById(section->getMaMon());
        throw DuplicateException(
            "Ban da dang ky mon '" +
            (c ? c->getTenMon() : section->getMaMon()) +
            "' o lop '" + oldSecID +
            "'. Vui long HUY lop cu truoc khi dang ky lop moi");
    }

    if (isTimeConflict(maSV, maLopHP))
        throw TimeConflictException("Trung lich hoc voi mot lop hoc phan da dang ky");

    if (!section->conCho()) {
        Queue<string>* q = sectionRepo.ensureWaitlist(maLopHP);
        if (q) {
            if (q->contains(maSV)) {
                throw DuplicateException(
                    "Sinh vien '" + maSV + "' da nam trong hang doi cho cua lop '" + maLopHP + "' roi");
            }
            // Neu sinh vien dang o danh sach cho cua mot lop khac cung mon -> tu dong out khoi lop cu
            removeStudentFromCourseWaitlists(maSV, section->getMaMon(), maLopHP, outRemovedWaitlists);
            q->push(maSV);
            string msg = "Lop '" + maLopHP + "' da het cho. Sinh vien '" + maSV + "' duoc them vao hang doi cho";
            if (outRemovedWaitlists && !outRemovedWaitlists->empty()) {
                msg += " (da tu dong out khoi danh sach cho lop: ";
                auto* n = outRemovedWaitlists->getHead();
                while (n) {
                    msg += n->data;
                    if (n->next) msg += ", ";
                    n = n->next;
                }
                msg += ")";
            }
            throw ClassFullException(msg);
        }
        throw ClassFullException("Lop '" + maLopHP + "' da het cho");
    }

    const string date = registrationDate.empty() ? today() : registrationDate;
    if (date.empty() || !section->tangSoLuong())
        throw AppException("Loi noi bo khi thuc hien dang ky");

    const string regID = registrationRepo.getRegID(maSV, maLopHP);
    Registration newReg(student, section, date, regID);
    registrationRepo.add(newReg);

    // Thiet lap lien ket con tro 2 chieu thuan OOP (Huong 1 & Huong 1.2)
    Registration* addedReg = registrationRepo.findById(regID);
    if (addedReg != nullptr && student != nullptr && section != nullptr) {
        addedReg->setSinhVien(student);
        addedReg->setLopHocPhan(section);
        student->themDangKy(addedReg);
        section->themDangKy(addedReg);
    }

    // TU DONG OUT KHOI TAT CA DANH SACH CHO CUA MON HOC NAY
    removeStudentFromCourseWaitlists(maSV, section->getMaMon(), "", outRemovedWaitlists);

    return true;
}

bool RegistrationService::cancelRegistration(const string& maSV, const string& maLopHP,
                                            DoubleLinkedList<string>* outWaitlistNotices) {
    const string regID = registrationRepo.getRegID(maSV, maLopHP);
    Registration* reg = registrationRepo.findById(regID);
    if (!reg) return false;

    Student* student = studentRepo.findById(maSV);
    ClassSection* section = sectionRepo.findById(maLopHP);

    // Huy lien ket con tro giua Student, ClassSection va Registration
    if (student != nullptr) student->xoaDangKy(reg);
    if (section != nullptr) section->xoaDangKy(reg);

    if (section) section->giamSoLuong();

    registrationRepo.remove(regID);

    // Tu dong dang ky tu hang doi (giu nguyen thu tu FIFO cho SV tam thoi trung lich)
    if (section) {
        Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
        if (q && !q->empty()) {
            DoubleLinkedList<string> candidates;
            q->copyTo(candidates);
            for (auto* node = candidates.getHead(); node != nullptr; node = node->next) {
                const string& nextSV = node->data;
                Student* s  = studentRepo.findById(nextSV);
                Course*  c  = section->getMonHoc() ? section->getMonHoc() : courseRepo.findById(section->getMaMon());
                bool match  = (s != nullptr);
                if (s && c) {
                    if (!c->getKhoa().empty()    && c->getKhoa()    != "Chung" && s->getKhoa()    != c->getKhoa())    match = false;
                    if (!c->getKhoaHoc().empty() && c->getKhoaHoc() != "Chung" && s->getKhoaHoc() != c->getKhoaHoc()) match = false;
                    if (!c->getNganhHoc().empty()&& c->getNganhHoc()!= "Chung" && s->getNganhHoc()!= c->getNganhHoc())match = false;
                }
                if (!match) {
                    q->remove(nextSV);
                    continue;
                }

                // Kiem tra xem sinh vien nay da dang ky mot lop khac cua mon nay chua!
                string alreadySec = getRegisteredSectionForCourse(nextSV, section->getMaMon());
                if (!alreadySec.empty()) {
                    q->remove(nextSV);
                    removeStudentFromCourseWaitlists(nextSV, section->getMaMon());
                    continue;
                }

                if (!isTimeConflict(nextSV, maLopHP)) {
                    q->remove(nextSV);
                    section->tangSoLuong();
                    const string newRegID = registrationRepo.getRegID(nextSV, maLopHP);
                    Registration autoReg(s, section, today(), newRegID);
                    registrationRepo.add(autoReg);

                    // Thiet lap lien ket con tro thuan OOP
                    Registration* addedReg = registrationRepo.findById(newRegID);
                    if (addedReg != nullptr && s != nullptr) {
                        addedReg->setSinhVien(s);
                        addedReg->setLopHocPhan(section);
                        s->themDangKy(addedReg);
                        section->themDangKy(addedReg);
                    }

                    // Tu dong out khoi cac waitlist con lai cua mon nay (neu co)
                    removeStudentFromCourseWaitlists(nextSV, section->getMaMon());
                    if (outWaitlistNotices != nullptr) {
                        outWaitlistNotices->pushBack("Tu dong dang ky cho sinh vien " + nextSV +
                                                     " tu hang doi vao lop " + maLopHP + ".");
                    }
                    break;
                } else {
                    if (outWaitlistNotices != nullptr) {
                        outWaitlistNotices->pushBack("Sinh vien " + nextSV +
                                                     " bi trung lich (giu nguyen trong hang doi), chuyen sang ban tiep theo.");
                    }
                }
            }
        }
    }
    return true;
}

void RegistrationService::getStudentRegistrations(const string& maSV,
                                                 DoubleLinkedList<StudentRegistrationDetail>& outDetails,
                                                 int semester) const {
    outDetails.clear();
    const Student* s = studentRepo.findById(maSV);

    // Uu tien duyet truc tiep qua dsDangKy cua Student (Huong 1)
    if (s != nullptr && s->getDanhSachDangKy().getHead() != nullptr) {
        auto rCur = s->getDanhSachDangKy().getHead();
        while (rCur != nullptr) {
            const Registration* reg = rCur->data;
            if (reg != nullptr) {
                const ClassSection* sec = reg->getLopHocPhan() ? reg->getLopHocPhan() : sectionRepo.findById(reg->getMaLopHP());
                if (sec) {
                    if (semester != 0 && sec->getHocKy() != semester) {
                        rCur = rCur->next;
                        continue;
                    }
                    StudentRegistrationDetail item;
                    item.maDangKy = reg->getMaDangKy();
                    item.maLopHP = sec->getMaLopHP();
                    item.ngayDangKy = reg->getNgayDangKy();
                    const Course* c = sec->getMonHoc() ? sec->getMonHoc() : courseRepo.findById(sec->getMaMon());
                    item.tenMon = c ? c->getTenMon() : sec->getMaMon();
                    item.nhomLopHP = sec->getNhomLopHP();
                    item.thu = sec->getSchedule().getThu();
                    item.tietBatDau = sec->getSchedule().getTietBatDau();
                    item.tietKetThuc = sec->getSchedule().getTietKetThuc();
                    item.giangVien = sec->getGiangVien();
                    item.hocKy = sec->getHocKy();
                    outDetails.pushBack(item);
                }
            }
            rCur = rCur->next;
        }
        return;
    }

    // Fallback: duyet qua registrationRepo
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* sec = sectionRepo.findById(cur->data.getMaLopHP());
            if (sec) {
                if (semester != 0 && sec->getHocKy() != semester) {
                    cur = cur->next;
                    continue;
                }
                StudentRegistrationDetail item;
                item.maDangKy = cur->data.getMaDangKy();
                item.maLopHP = cur->data.getMaLopHP();
                item.ngayDangKy = cur->data.getNgayDangKy();
                const Course* c = courseRepo.findById(sec->getMaMon());
                item.tenMon = c ? c->getTenMon() : sec->getMaMon();
                item.nhomLopHP = sec->getNhomLopHP();
                item.thu = sec->getThu();
                item.tietBatDau = sec->getTietBatDau();
                item.tietKetThuc = sec->getTietKetThuc();
                item.giangVien = sec->getGiangVien();
                item.hocKy = sec->getHocKy();
                outDetails.pushBack(item);
            }
        }
        cur = cur->next;
    }
}

void RegistrationService::getAllRegistrations(DoubleLinkedList<Registration>& outList) const {
    copyList(registrationRepo.getList(), outList);
}

// =========================================================
//  WAITLIST LOGIC
// =========================================================

bool RegistrationService::hasWaitlist(const string& maLopHP) const {
    return sectionRepo.hasWaitlist(maLopHP);
}

int RegistrationService::getWaitlistCount(const string& maLopHP) const {
    return sectionRepo.getWaitlistCount(maLopHP);
}

bool RegistrationService::canShowWaitlist(const string& maLopHP) const {
    const ClassSection* sec = sectionRepo.findById(maLopHP);
    if (!sec) return false;
    bool duSoLuong = !sec->conCho();
    bool coSinhVienCho = hasWaitlist(maLopHP);
    return duSoLuong && coSinhVienCho;
}

bool RegistrationService::getWaitlistStudents(const string& maLopHP, DoubleLinkedList<Student>& outStudents) const {
    outStudents.clear();
    const Queue<string>* q = sectionRepo.getWaitlist(maLopHP);
    if (!q) return false;
    DoubleLinkedList<string> ids;
    q->copyTo(ids);
    auto cur = ids.getHead();
    while (cur != nullptr) {
        const Student* sv = studentRepo.findById(cur->data);
        if (sv) {
            outStudents.pushBack(*sv);
        } else {
            Student temp;
            temp.setMaSV(cur->data);
            temp.setHoTen("(Chua cap nhat)");
            outStudents.pushBack(temp);
        }
        cur = cur->next;
    }
    return true;
}

int RegistrationService::removeStudentFromCourseWaitlists(const string& maSV, const string& maMon,
                                                         const string& exceptSectionID,
                                                         DoubleLinkedList<string>* outRemovedWaitlists) {
    return sectionRepo.removeStudentFromCourseWaitlists(maSV, maMon, exceptSectionID, outRemovedWaitlists);
}

bool RegistrationService::removeStudentFromWaitlist(const string& maSV, const string& maLopHP) {
    return sectionRepo.removeFromWaitlist(maLopHP, maSV);
}

// =========================================================
//  TINH TOAN & THONG KE
// =========================================================

int RegistrationService::calculateTotalCredits(const string& maSV,
                                              DoubleLinkedList<CreditSummaryItem>* outDetails,
                                              int semester) const {
    if (outDetails != nullptr) outDetails->clear();
    int totalCredits = 0;
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* sec = sectionRepo.findById(cur->data.getMaLopHP());
            if (sec) {
                if (semester != 0 && sec->getHocKy() != semester) {
                    cur = cur->next;
                    continue;
                }
                const Course* c = courseRepo.findById(sec->getMaMon());
                int tc = (c ? c->getSoTinChi() : 0);
                totalCredits += tc;
                if (outDetails != nullptr) {
                    CreditSummaryItem item;
                    item.maLopHP = sec->getMaLopHP();
                    item.tenMon = c ? c->getTenMon() : sec->getMaMon();
                    item.soTinChi = tc;
                    outDetails->pushBack(item);
                }
            }
        }
        cur = cur->next;
    }
    return totalCredits;
}

void RegistrationService::getStudentTimetable(const string& maSV,
                                             DoubleLinkedList<TimetableEntry>& outEntries) const {
    outEntries.clear();
    const string days[] = {"2", "3", "4", "5", "6", "7", "CN"};
    for (int d = 0; d < 7; d++) {
        auto cur = registrationRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getMaSV() == maSV) {
                const ClassSection* sec = sectionRepo.findById(cur->data.getMaLopHP());
                if (sec && sec->getHocKy() == currentSemester && sec->getThu() == days[d]) {
                    const Course* c = courseRepo.findById(sec->getMaMon());
                    TimetableEntry entry;
                    entry.thu = sec->getThu();
                    entry.tietBatDau = sec->getTietBatDau();
                    entry.tietKetThuc = sec->getTietKetThuc();
                    entry.maLopHP = sec->getMaLopHP();
                    entry.tenMon = c ? c->getTenMon() : sec->getMaMon();
                    entry.phong = sec->getPhong();
                    entry.giangVien = sec->getGiangVien();
                    outEntries.pushBack(entry);
                }
            }
            cur = cur->next;
        }
    }
}

bool RegistrationService::checkTimeConflictDetails(const string& maSV, const string& maLopHP,
                                                  ClassSection& outTarget,
                                                  DoubleLinkedList<ClassSection>& outConflicts) const {
    outConflicts.clear();
    const ClassSection* target = sectionRepo.findById(maLopHP);
    if (!target) return false;
    outTarget = *target;

    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaSV() == maSV) {
            const ClassSection* existing = sectionRepo.findById(cur->data.getMaLopHP());
            if (existing && existing->getHocKy() == target->getHocKy() &&
                existing->trungLichVoi(*target)) {
                outConflicts.pushBack(*existing);
            }
        }
        cur = cur->next;
    }
    return true;
}

int RegistrationService::getSemesterRequiredCourses(const string& maSV, string courseIDs[], int maxCourses) const {
    const Student* student = studentRepo.findById(maSV);
    if (!student) return 0;
    return courseRepo.getMatchingCourses(
        student->getKhoa(), student->getKhoaHoc(), student->getNganhHoc(),
        currentSemester, courseIDs, maxCourses);
}

int RegistrationService::getMissingCoursesCount(const string& maSV, string missingIDs[], int maxMissing) const {
    const int MAX_REQ = 50;
    string reqIDs[MAX_REQ];
    int reqCount = getSemesterRequiredCourses(maSV, reqIDs, MAX_REQ);
    int missing = 0;
    for (int i = 0; i < reqCount; i++) {
        if (getRegisteredSectionForCourse(maSV, reqIDs[i]).empty()) {
            if (missing < maxMissing && missingIDs) missingIDs[missing] = reqIDs[i];
            missing++;
        }
    }
    return missing;
}

bool RegistrationService::getSemesterRegistrationStatus(const string& maSV,
                                                       SemesterProgressSummary& outSummary,
                                                       DoubleLinkedList<CourseProgressItem>& outItems) const {
    outItems.clear();
    const Student* student = studentRepo.findById(maSV);
    if (!student) return false;

    outSummary.maSV = student->getMaSV();
    outSummary.hoTen = student->getHoTen();
    outSummary.lop = student->getLop();
    outSummary.currentSemester = currentSemester;
    outSummary.totalRequiredCourses = 0;
    outSummary.registeredCourses = 0;
    outSummary.totalCredits = 0;
    outSummary.registeredCredits = 0;

    const int MAX_REQ = 50;
    string reqIDs[MAX_REQ];
    int reqCount = getSemesterRequiredCourses(maSV, reqIDs, MAX_REQ);
    outSummary.totalRequiredCourses = reqCount;

    for (int i = 0; i < reqCount; i++) {
        const Course* c = courseRepo.findById(reqIDs[i]);
        CourseProgressItem item;
        item.maMon = reqIDs[i];
        item.tenMon = c ? c->getTenMon() : reqIDs[i];
        item.soTinChi = c ? c->getSoTinChi() : 0;
        outSummary.totalCredits += item.soTinChi;

        string regSec = getRegisteredSectionForCourse(maSV, reqIDs[i]);
        if (!regSec.empty()) {
            item.daDangKy = true;
            item.maLopHP = regSec;
            outSummary.registeredCourses++;
            outSummary.registeredCredits += item.soTinChi;
            const ClassSection* s = sectionRepo.findById(regSec);
            if (s) {
                item.thu = s->getThu();
                item.tietBatDau = s->getTietBatDau();
                item.tietKetThuc = s->getTietKetThuc();
            }
        }
        outItems.pushBack(item);
    }
    return true;
}

int RegistrationService::getEligibleGroupsForStudent(const string& maSV, GroupInfo eligibleGroups[], int maxGroups) const {
    const Student* student = studentRepo.findById(maSV);
    if (!student) return 0;

    const int MAX_ALL = 100;
    string allGroupNames[MAX_ALL];
    int allGroupCount = sectionRepo.getGroupsForSemester(currentSemester, allGroupNames, MAX_ALL);
    int count = 0;

    for (int i = 0; i < allGroupCount && count < maxGroups; i++) {
        string gName = allGroupNames[i];
        GroupInfo g;
        g.groupName = gName;
        g.hocKy = currentSemester;
        bool groupMatchesStudent = true;
        bool hasMatchingSection  = false;

        auto cur = sectionRepo.getList().getHead();
        while (cur != nullptr) {
            if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == gName) {
                if (g.totalSections < 50) g.sectionIDs[g.totalSections++] = cur->data.getMaLopHP();
                const Course* c = courseRepo.findById(cur->data.getMaMon());
                if (c) {
                    g.totalCredits += c->getSoTinChi();
                    if (g.khoa.empty()     && !c->getKhoa().empty()     && c->getKhoa()     != "Chung") g.khoa     = c->getKhoa();
                    if (g.khoaHoc.empty()  && !c->getKhoaHoc().empty()  && c->getKhoaHoc()  != "Chung") g.khoaHoc  = c->getKhoaHoc();
                    if (g.nganhHoc.empty() && !c->getNganhHoc().empty() && c->getNganhHoc() != "Chung") g.nganhHoc = c->getNganhHoc();
                    bool mK  = c->getKhoa().empty()    || c->getKhoa()    == "Chung" || student->getKhoa().empty()    || student->getKhoa()    == c->getKhoa();
                    bool mKH = c->getKhoaHoc().empty() || c->getKhoaHoc() == "Chung" || student->getKhoaHoc().empty() || student->getKhoaHoc() == c->getKhoaHoc();
                    bool mNg = c->getNganhHoc().empty()|| c->getNganhHoc()== "Chung" || student->getNganhHoc().empty()|| student->getNganhHoc()== c->getNganhHoc();
                    if (mK && mKH && mNg) hasMatchingSection = true;
                    else                  groupMatchesStudent = false;
                }
            }
            cur = cur->next;
        }
        if (groupMatchesStudent && hasMatchingSection) eligibleGroups[count++] = g;
    }

    if (count == 0) {
        for (int i = 0; i < allGroupCount && count < maxGroups; i++) {
            string gName = allGroupNames[i];
            GroupInfo g;
            g.groupName = gName;
            g.hocKy = currentSemester;
            auto cur = sectionRepo.getList().getHead();
            while (cur != nullptr) {
                if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == gName) {
                    if (g.totalSections < 50) g.sectionIDs[g.totalSections++] = cur->data.getMaLopHP();
                    const Course* c = courseRepo.findById(cur->data.getMaMon());
                    if (c) g.totalCredits += c->getSoTinChi();
                }
                cur = cur->next;
            }
            eligibleGroups[count++] = g;
        }
    }
    return count;
}
