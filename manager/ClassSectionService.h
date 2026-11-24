#ifndef CLASS_SECTION_SERVICE_H
#define CLASS_SECTION_SERVICE_H

#include <string>
#include "../DSA/DoublyLinkedList.h"
#include "../model/classsection.h"
#include "../model/dtos.h"

class ClassSectionRepository;
class CourseRepository;
class RegistrationRepository;
class StudentRepository;

class ClassSectionService {
private:
    ClassSectionRepository& sectionRepo;
    CourseRepository&       courseRepo;
    RegistrationRepository& registrationRepo;
    StudentRepository&      studentRepo;

    template <typename T>
    static void copyList(const DoubleLinkedList<T>& src, DoubleLinkedList<T>& dst);

public:
    ClassSectionService(ClassSectionRepository& csRepo,
                        CourseRepository& cRepo,
                        RegistrationRepository& rRepo,
                        StudentRepository& sRepo);

    bool addClassSection(const ClassSection& sec);
    bool deleteClassSection(const std::string& maLopHP);
    ClassSection* findClassSection(const std::string& maLopHP);
    const ClassSection* findClassSection(const std::string& maLopHP) const;
    void getAllSections(DoubleLinkedList<ClassSection>& outList) const;
    void searchSections(const std::string& kw, DoubleLinkedList<ClassSection>& outList) const;

    bool getStudentsInSection(const std::string& maLopHP, ClassSection& outSection,
                              DoubleLinkedList<SectionStudentInfo>& outStudents) const;

    int getGroupsForCurrentSemester(int currentSemester, std::string groupNames[], int maxGroups) const;
    void getSectionsByGroupInCurrentSemester(int currentSemester, const std::string& groupName,
                                             DoubleLinkedList<SectionDetailInfo>& outSections) const;
    void getUngroupedSectionsInCurrentSemester(int currentSemester,
                                               DoubleLinkedList<SectionDetailInfo>& outSections) const;

    SectionDetailInfo buildSectionDetail(const ClassSection& sec) const;
};

#endif // CLASS_SECTION_SERVICE_H
