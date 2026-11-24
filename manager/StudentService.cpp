#include "StudentService.h"
#include "../dal/repositories/StudentRepository.h"
#include "../dal/repositories/RegistrationRepository.h"
#include "../dal/repositories/ClassSectionRepository.h"
#include "RegistrationService.h"

template <typename T>
void StudentService::copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst) {
    dst.clear();
    auto cur = src.getHead();
    while (cur != nullptr) {
        dst.pushBack(cur->data);
        cur = cur->next;
    }
}

StudentService::StudentService(StudentRepository& sRepo,
                               RegistrationRepository& rRepo,
                               ClassSectionRepository& csRepo,
                               RegistrationService* regService)
    : studentRepo(sRepo), registrationRepo(rRepo), sectionRepo(csRepo), registrationService(regService) {}

void StudentService::setRegistrationService(RegistrationService* regService) {
    registrationService = regService;
}

bool StudentService::addStudent(const Student& s) {
    return studentRepo.add(s);
}

bool StudentService::deleteStudent(const std::string& maSV,
                                   DoubleLinkedList<std::string>* outWaitlistNotices) {
    // 1. Xoa het dang ky cua sinh vien (cap nhat lai si so lop hoc phan)
    const int MAX = 100;
    std::string secIDs[MAX];
    int cnt = registrationRepo.getSectionsByStudent(maSV, secIDs, MAX);
    for (int i = 0; i < cnt; i++) {
        if (registrationService != nullptr) {
            registrationService->cancelRegistration(maSV, secIDs[i], outWaitlistNotices);
        } else {
            registrationRepo.removeAllByStudent(maSV);
        }
    }

    // 2. Xoa sinh vien khoi tat ca waitlist neu co
    sectionRepo.removeStudentFromAllWaitlists(maSV);

    // 3. Xoa sinh vien
    return studentRepo.remove(maSV);
}

Student* StudentService::findStudent(const std::string& maSV) {
    return studentRepo.findById(maSV);
}

const Student* StudentService::findStudent(const std::string& maSV) const {
    return studentRepo.findById(maSV);
}

void StudentService::getAllStudents(DoubleLinkedList<Student>& outList) const {
    copyList(studentRepo.getList(), outList);
}

void StudentService::searchStudents(const std::string& kw, DoubleLinkedList<Student>& outList) const {
    studentRepo.search(kw, outList);
}
