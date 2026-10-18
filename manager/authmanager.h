#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>
#include <vector>

using namespace std;

enum class UserRole {
    Administrator,
    Student
};

class UserAccount {
private:
    string password;

    friend class AuthManager;

public:
    string username;
    UserRole role;
    string studentID;

    UserAccount(const string& accountName, const string& accountPassword,
                UserRole accountRole, const string& linkedStudentID)
        : password(accountPassword),
          username(accountName),
          role(accountRole),
          studentID(linkedStudentID) {}
};

class AuthManager {
private:
    vector<UserAccount> accounts;
    int currentAccountIndex;

public:
    AuthManager() : currentAccountIndex(-1) {}

    bool addAccount(const string& username, const string& password,
                    UserRole role, const string& studentID = "") {
        if (username.empty() || password.empty() ||
            (role == UserRole::Student && studentID.empty())) {
            return false;
        }

        for (const UserAccount& account : accounts) {
            if (account.username == username) {
                return false;
            }
        }

        accounts.push_back(UserAccount(username, password, role, studentID));
        return true;
    }

    bool login(const string& username, const string& password) {
        logout();
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].username == username &&
                accounts[i].password == password) {
                currentAccountIndex = static_cast<int>(i);
                return true;
            }
        }
        return false;
    }

    void logout() {
        currentAccountIndex = -1;
    }

    bool isLoggedIn() const {
        return currentAccountIndex >= 0 &&
               static_cast<size_t>(currentAccountIndex) < accounts.size();
    }

    const UserAccount* currentUser() const {
        if (!isLoggedIn()) {
            return nullptr;
        }
        return &accounts[static_cast<size_t>(currentAccountIndex)];
    }
};

#endif
