#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include <string>

#include "../manager/authmanager.h"
#include "../manager/system.h"

class ConsoleApp {
private:
    SystemManager system;
    AuthManager auth;
    std::string error;

    bool saveAndExit();
    bool initAuthAccounts();

public:
    int run();
};

#endif // CONSOLE_APP_H
