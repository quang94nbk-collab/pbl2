#include "authmanager.h"

UserAccount::UserAccount(const string& accountName, const string& accountPassword,
                         UserRole accountRole, const string& linkedStudentID)
    : password(accountPassword),
      username(accountName),
      role(accountRole),
      studentID(linkedStudentID) {}

AuthManager::AuthManager() : currentAccountIndex(-1) {}

bool AuthManager::addAccount(const string& username, const string& password,
                             UserRole role, const string& studentID) {
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

bool AuthManager::removeAccount(const string& username) {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].username == username) {
            if (currentAccountIndex == static_cast<int>(i)) {
                logout();
            } else if (currentAccountIndex > static_cast<int>(i)) {
                --currentAccountIndex;
            }
            accounts.erase(accounts.begin() +
                           static_cast<vector<UserAccount>::difference_type>(i));
            return true;
        }
    }
    return false;
}

bool AuthManager::login(const string& username, const string& password) {
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

void AuthManager::logout() {
    currentAccountIndex = -1;
}

bool AuthManager::hasAccount(const string& username) const {
    for (const UserAccount& account : accounts) {
        if (account.username == username) {
            return true;
        }
    }
    return false;
}

bool AuthManager::isLoggedIn() const {
    return currentAccountIndex >= 0 &&
           static_cast<size_t>(currentAccountIndex) < accounts.size();
}

const UserAccount* AuthManager::currentUser() const {
    if (!isLoggedIn()) {
        return nullptr;
    }
    return &accounts[static_cast<size_t>(currentAccountIndex)];
}
