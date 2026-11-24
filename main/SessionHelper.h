#ifndef SESSION_HELPER_H
#define SESSION_HELPER_H

#include <string>

class SystemManager;

namespace SessionHelper {

void showSectionsByGroupView(const SystemManager& system);

/**
 * @brief Thuc hien dang ky lop hoc phan cho sinh vien tren Console kem theo phan hoi chi tiet va tu dong luu du lieu.
 * @param system Doi tuong quan ly he thong SystemManager.
 * @param studentID Ma sinh vien dang ky.
 * @param sectionID Ma lop hoc phan can dang ky.
 */
void tryRegisterWithFeedback(SystemManager& system, const std::string& studentID, const std::string& sectionID);

void showSemesterStatusView(const SystemManager& system, const std::string& studentID);

/**
 * @brief Chuc nang tra cuu chuyen biet Khung chuong trinh dao tao theo Khoa, Khoa hoc, Nganh va Hoc ky tren Console.
 * @param system Doi tuong quan ly he thong SystemManager.
 */
void searchCoursesSpecialized(SystemManager& system);

} // namespace SessionHelper

#endif // SESSION_HELPER_H
