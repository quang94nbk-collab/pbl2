#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include <string>
#include "../DSA/DoublyLinkedList.h"
#include "../model/student.h"

class StudentRepository;
class RegistrationRepository;
class ClassSectionRepository;
class RegistrationService;

class StudentService {
private:
    StudentRepository&      studentRepo;
    RegistrationRepository& registrationRepo;
    ClassSectionRepository& sectionRepo;
    RegistrationService*    registrationService;

    template <typename T>
    static void copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst);

public:
    StudentService(StudentRepository& sRepo,
                   RegistrationRepository& rRepo,
                   ClassSectionRepository& csRepo,
                   RegistrationService* regService = nullptr);

    void setRegistrationService(RegistrationService* regService);

    bool addStudent(const Student& s);
    bool deleteStudent(const std::string& maSV, DoubleLinkedList<std::string>* outWaitlistNotices = nullptr);
    Student* findStudent(const std::string& maSV);
    const Student* findStudent(const std::string& maSV) const;
    void getAllStudents(DoubleLinkedList<Student>& outList) const;
    void searchStudents(const std::string& kw, DoubleLinkedList<Student>& outList) const;
};

#endif // STUDENT_SERVICE_H
