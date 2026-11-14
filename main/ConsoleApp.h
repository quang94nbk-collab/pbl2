#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include <iostream>
#include <string>

#include "../manager/authmanager.h"
#include "../manager/system.h"
#include "../ui/ConsoleView.h"
#include "AdminSession.h"
#include "StudentSession.h"

class ConsoleApp {
private:
    SystemManager system;
    AuthManager auth;
    std::string error;

    bool saveAndExit() {
        if (!system.saveData(error)) {
            std::cerr << "Khong the luu du lieu: " << error << '\n';
            return false;
        }
        std::cout << "\n>> Da luu toan bo du lieu. Tam biet!\n";
        return true;
    }

    bool initAuthAccounts() {
        if (!auth.addAccount("admin", "admin", UserRole::Administrator)) {
            std::cerr << "Khong the tao tai khoan quan tri mac dinh.\n";
            return false;
        }
        DoubleLinkedList<Student> loadedStudents;
        system.getAllStudents(loadedStudents);
        auto studentNode = loadedStudents.getHead();
        while (studentNode != nullptr) {
            const std::string& studentID = studentNode->data.getMaSV();
            if (!auth.addAccount(studentID, studentID, UserRole::Student, studentID)) {
                std::cerr << "Khong the tao tai khoan cho sinh vien: " << studentID
                          << ". Ma sinh vien co the trung ten tai khoan.\n";
                return false;
            }
            studentNode = studentNode->next;
        }
        return true;
    }

public:
    int run() {
        if (!system.loadData(error)) {
            std::cerr << "Khong the nap du lieu: " << error << '\n';
            return 1;
        }

        if (!initAuthAccounts()) {
            return 1;
        }

        while (true) {
            if (!auth.isLoggedIn()) {
                ConsoleView::showMainMenu(system.getCurrentSemester());
                int menuChoice;
                if (!ConsoleView::readChoice(menuChoice)) {
                    if (std::cin.eof()) {
                        return saveAndExit() ? 0 : 1;
                    }
                    std::cout << "Lua chon khong hop le.\n";
                    continue;
                }
                if (menuChoice == 0) {
                    return saveAndExit() ? 0 : 1;
                }
                if (menuChoice != 1) {
                    std::cout << "Lua chon khong hop le.\n";
                    continue;
                }

                std::string username, password;
                std::cout << "\n=== DANG NHAP ===\nTai khoan: ";
                if (!getline(std::cin >> std::ws, username)) {
                    return saveAndExit() ? 0 : 1;
                }
                std::cout << "Mat khau: ";
                if (!getline(std::cin, password)) {
                    return saveAndExit() ? 0 : 1;
                }
                if (!auth.login(username, password)) {
                    std::cout << "Tai khoan hoac mat khau khong dung.\n";
                    continue;
                }
                std::cout << "Dang nhap thanh cong.\n";
            }

            const UserAccount* user = auth.currentUser();
            if (user == nullptr) {
                std::cerr << "Phien dang nhap khong hop le.\n";
                return 1;
            }
            const bool isAdministrator = (user->role == UserRole::Administrator);
            if (isAdministrator) {
                AdminSession::run(system, auth);
                auth.logout();
                continue;
            }
            StudentSession::run(system, user->studentID);
            auth.logout();
            continue;
        }
    }
};

#endif // CONSOLE_APP_H
