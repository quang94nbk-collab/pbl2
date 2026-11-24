#include "ClassSectionService.h"
#include "../dal/repositories/ClassSectionRepository.h"
#include "../dal/repositories/CourseRepository.h"
#include "../dal/repositories/RegistrationRepository.h"
#include "../dal/repositories/StudentRepository.h"
#include "../model/registration.h"

template <typename T>
void ClassSectionService::copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst) {
    dst.clear();
    auto cur = src.getHead();
    while (cur != nullptr) {
        dst.pushBack(cur->data);
        cur = cur->next;
    }
}

ClassSectionService::ClassSectionService(ClassSectionRepository& csRepo,
                                         CourseRepository& cRepo,
                                         RegistrationRepository& rRepo,
                                         StudentRepository& sRepo)
    : sectionRepo(csRepo), courseRepo(cRepo), registrationRepo(rRepo), studentRepo(sRepo) {}

SectionDetailInfo ClassSectionService::buildSectionDetail(const ClassSection& sec) const {
    SectionDetailInfo info;
    info.maLopHP = sec.getMaLopHP();
    info.maMon = sec.getMaMon();
    info.nhomLopHP = sec.getNhomLopHP();
    info.giangVien = sec.getGiangVien();
    // Lay thong tin thoi khoa bieu thong qua doi tuong Schedule (Composition)
    info.thu = sec.getSchedule().getThu();
    info.tietBatDau = sec.getSchedule().getTietBatDau();
    info.tietKetThuc = sec.getSchedule().getTietKetThuc();
    info.phong = sec.getSchedule().getPhong();
    info.siSoToiDa = sec.getSiSoToiDa();
    info.soLuongDaDangKy = sec.getSoLuongDaDangKy();
    info.hocKy = sec.getHocKy();
    // Uu tien lay thong tin mon hoc truc tiep qua con tro Course* (Association)
    const Course* c = sec.getMonHoc() ? sec.getMonHoc() : courseRepo.findById(sec.getMaMon());
    info.tenMon = c ? c->getTenMon() : sec.getMaMon();
    info.soTinChi = c ? c->getSoTinChi() : 0;
    return info;
}

bool ClassSectionService::addClassSection(const ClassSection& sec) {
    // Kiem tra FK: maMon phai ton tai qua CourseRepository
    if (!courseRepo.exists(sec.getMaMon())) return false;
    if (!sectionRepo.add(sec)) return false;

    // Tu dong thiet lap lien ket con tro (Huong 1 & Huong 2)
    ClassSection* addedSec = sectionRepo.findById(sec.getMaLopHP());
    Course* c = courseRepo.findById(sec.getMaMon());
    if (addedSec != nullptr && c != nullptr) {
        addedSec->setMonHoc(c);
        c->themLopHocPhan(addedSec);
    }
    return true;
}

bool ClassSectionService::deleteClassSection(const std::string& maLopHP) {
    ClassSection* sec = sectionRepo.findById(maLopHP);
    if (sec != nullptr) {
        // 1. Huy lien ket khoi Course (Huong 2)
        if (sec->getMonHoc() != nullptr) {
            sec->getMonHoc()->xoaLopHocPhan(sec);
        }
        // 2. Huy lien ket khoi cac Student da dang ky (Huong 1)
        auto rCur = sec->getDanhSachDangKy().getHead();
        while (rCur != nullptr) {
            Registration* r = rCur->data;
            if (r != nullptr && r->getSinhVien() != nullptr) {
                r->getSinhVien()->xoaDangKy(r);
            }
            rCur = rCur->next;
        }
        sec->clearDangKy();
    }

    // 3. Xoa het Registration cua lop nay (Cascade delete quan he)
    registrationRepo.removeAllBySection(maLopHP);
    // 4. Xoa waitlist cua lop nay
    sectionRepo.clearSectionWaitlist(maLopHP);
    // 5. Xoa lop HP
    return sectionRepo.remove(maLopHP);
}

ClassSection* ClassSectionService::findClassSection(const std::string& maLopHP) {
    return sectionRepo.findById(maLopHP);
}

const ClassSection* ClassSectionService::findClassSection(const std::string& maLopHP) const {
    return sectionRepo.findById(maLopHP);
}

void ClassSectionService::getAllSections(DoubleLinkedList<ClassSection>& outList) const {
    copyList(sectionRepo.getList(), outList);
}

void ClassSectionService::searchSections(const std::string& kw, DoubleLinkedList<ClassSection>& outList) const {
    sectionRepo.search(kw, outList);
}

bool ClassSectionService::getStudentsInSection(const std::string& maLopHP, ClassSection& outSection,
                                               DoubleLinkedList<SectionStudentInfo>& outStudents) const {
    outStudents.clear();
    const ClassSection* sec = findClassSection(maLopHP);
    if (!sec) return false;
    outSection = *sec;

    // Uu tien lay truc tiep tu danh sach con tro Registration cua ClassSection (Huong 1.2)
    if (sec->getDanhSachDangKy().getHead() != nullptr) {
        auto rCur = sec->getDanhSachDangKy().getHead();
        while (rCur != nullptr) {
            const Registration* reg = rCur->data;
            if (reg != nullptr) {
                const Student* sv = reg->getSinhVien() ? reg->getSinhVien() : studentRepo.findById(reg->getMaSV());
                SectionStudentInfo item;
                item.maSV = reg->getMaSV();
                item.hoTen = sv ? sv->getHoTen() : "(Chua cap nhat)";
                item.lop = sv ? sv->getLop() : "-";
                item.ngayDangKy = reg->getNgayDangKy();
                outStudents.pushBack(item);
            }
            rCur = rCur->next;
        }
        return true;
    }

    // Fallback: duyet qua registrationRepo
    auto cur = registrationRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getMaLopHP() == maLopHP) {
            SectionStudentInfo item;
            item.maSV = cur->data.getMaSV();
            const Student* sv = studentRepo.findById(item.maSV);
            item.hoTen = sv ? sv->getHoTen() : "(Chua cap nhat)";
            item.lop = sv ? sv->getLop() : "-";
            item.ngayDangKy = cur->data.getNgayDangKy();
            outStudents.pushBack(item);
        }
        cur = cur->next;
    }
    return true;
}

int ClassSectionService::getGroupsForCurrentSemester(int currentSemester, std::string groupNames[], int maxGroups) const {
    return sectionRepo.getGroupsForSemester(currentSemester, groupNames, maxGroups);
}

void ClassSectionService::getSectionsByGroupInCurrentSemester(int currentSemester, const std::string& groupName,
                                                             DoubleLinkedList<SectionDetailInfo>& outSections) const {
    outSections.clear();
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP() == groupName) {
            outSections.pushBack(buildSectionDetail(cur->data));
        }
        cur = cur->next;
    }
}

void ClassSectionService::getUngroupedSectionsInCurrentSemester(int currentSemester,
                                                               DoubleLinkedList<SectionDetailInfo>& outSections) const {
    outSections.clear();
    auto cur = sectionRepo.getList().getHead();
    while (cur != nullptr) {
        if (cur->data.getHocKy() == currentSemester && cur->data.getNhomLopHP().empty()) {
            outSections.pushBack(buildSectionDetail(cur->data));
        }
        cur = cur->next;
    }
}
