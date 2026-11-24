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
                UserRole accountRole, const string& linkedStudentID);
};

class AuthManager {
private:
    vector<UserAccount> accounts;
    int currentAccountIndex;

public:
    AuthManager();

    bool addAccount(const string& username, const string& password,
                    UserRole role, const string& studentID = "");

    bool removeAccount(const string& username);

    bool login(const string& username, const string& password);

    void logout();

    bool hasAccount(const string& username) const;

    bool isLoggedIn() const;

    const UserAccount* currentUser() const;
};

#endif // AUTHMANAGER_H
