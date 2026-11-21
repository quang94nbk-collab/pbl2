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

/**
 * @brief Thuc hien dang ky lop hoc phan cho sinh vien tren Console kem theo phan hoi chi tiet va tu dong luu du lieu.
 * @param system Doi tuong quan ly he thong SystemManager.
 * @param studentID Ma sinh vien dang ky.
 * @param sectionID Ma lop hoc phan can dang ky.
 */
inline void tryRegisterWithFeedback(SystemManager& system, const std::string& studentID, const std::string& sectionID) {
    try {
        DoubleLinkedList<std::string> removedWaitlists;
        system.registerClass(studentID, sectionID, "", true, &removedWaitlists);
        std::string err;
        system.saveData(err);
        std::cout << ">> Dang ky lop hoc phan thanh cong!\n";
        auto* wNode = removedWaitlists.getHead();
        while (wNode) {
            std::cout << ">> [Hang cho] Da tu dong out khoi danh sach cho cua lop: " << wNode->data << "\n";
            wNode = wNode->next;
        }
    } catch (const ClassFullException& e) {
        std::string err;
        system.saveData(err);
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

/**
 * @brief Chuc nang tra cuu chuyen biet Khung chuong trinh dao tao theo Khoa, Khóa hoc, Nganh va Hoc ky tren Console.
 * @param system Doi tuong quan ly he thong SystemManager.
 */
inline void searchCoursesSpecialized(SystemManager& system) {
    using namespace std;
    cout << "\n============================================================\n";
    cout << "        KHUNG CHUONG TRINH DAO TAO\n";
    cout << "  (Nhan Enter de trong neu muon bo qua tieu chi)\n";
    cout << "============================================================\n";
    string khoa, khoaHoc, nganh, hkStr;
    cout << "Nhap Khoa (VD: CNTT, DTVT, CoKhi): ";
    getline(cin, khoa);
    cout << "Nhap Khoa hoc (VD: K23, K22): ";
    getline(cin, khoaHoc);
    cout << "Nhap Nganh hoc (VD: CNTT, DTVT): ";
    getline(cin, nganh);
    cout << "Nhap Hoc ky (1 hoac 2, de trong/0 de tim tat ca cac ky): ";
    getline(cin, hkStr);
    int hk = 0;
    try {
        if (!hkStr.empty()) hk = stoi(hkStr);
    } catch (...) {
        hk = 0;
    }

    DoubleLinkedList<Course> results;
    system.filterCourses(khoa, khoaHoc, nganh, hk, results);
    ConsoleView::displayFilteredCourses(khoa, khoaHoc, nganh, hk, results);
}

} // namespace SessionHelper

#endif // SESSION_HELPER_H
