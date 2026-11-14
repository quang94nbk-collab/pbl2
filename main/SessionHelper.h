#ifndef SESSION_HELPER_H
#define SESSION_HELPER_H

#include <iostream>
#include <string>

#include "../DSA/DoublyLinkedList.h"
#include "../manager/exception.h"
#include "../manager/system.h"
#include "../model/dtos.h"
#include "../ui/ConsoleView.h"

namespace SessionHelper {

inline void showSectionsByGroupView(const SystemManager& system) {
    std::cout << "  DANH SACH NHOM LOP HOC PHAN - HOC KY " << system.getCurrentSemester() << "\n";
    const int MAX_GROUPS = 100;
    std::string groupNames[MAX_GROUPS];
    int groupCount = system.getGroupsForCurrentSemester(groupNames, MAX_GROUPS);

    if (groupCount == 0) {
        std::cout << "(Khong co nhom lop hoc phan nao trong hoc ky " << system.getCurrentSemester() << ")\n";
    } else {
        for (int g = 0; g < groupCount; g++) {
            std::cout << "\n>>> NHOM: " << groupNames[g] << "\n";
            DoubleLinkedList<SectionDetailInfo> list;
            system.getSectionsByGroupInCurrentSemester(groupNames[g], list);
            ConsoleView::displayGroupSectionList(list, true);
        }
    }

    DoubleLinkedList<SectionDetailInfo> ungrouped;
    system.getUngroupedSectionsInCurrentSemester(ungrouped);
    if (!ungrouped.empty()) {
        std::cout << "\n>>> CHUA GAN NHOM:\n";
        ConsoleView::displayGroupSectionList(ungrouped, false);
    }
}

inline void tryRegisterWithFeedback(SystemManager& system, const std::string& studentID, const std::string& sectionID) {
    try {
        system.registerClass(studentID, sectionID);
        std::cout << ">> Dang ky lop hoc phan thanh cong!\n";
    } catch (const ClassFullException& e) {
        std::cout << ">> [Lop day] " << e.what() << "\n";
    } catch (const TimeConflictException& e) {
        std::cout << ">> [Trung lich] " << e.what() << "\n";
    } catch (const DuplicateException& e) {
        std::cout << ">> [Trung lap] " << e.what() << "\n";
    } catch (const WrongSemesterException& e) {
        std::cout << ">> [Sai hoc ky] " << e.what() << "\n";
    } catch (const NotEligibleException& e) {
        std::cout << ">> [Khong du dieu kien] " << e.what() << "\n";
    } catch (const AppException& e) {
        std::cout << ">> Dang ky that bai: " << e.what() << "\n";
    }
}

inline void showSemesterStatusView(const SystemManager& system, const std::string& studentID) {
    SemesterProgressSummary summary;
    DoubleLinkedList<CourseProgressItem> items;
    if (system.getSemesterRegistrationStatus(studentID, summary, items)) {
        ConsoleView::displaySemesterRegistrationStatus(summary, items);
    }
}

} // namespace SessionHelper

#endif // SESSION_HELPER_H
