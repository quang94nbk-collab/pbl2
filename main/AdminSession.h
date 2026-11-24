#ifndef ADMIN_SESSION_H
#define ADMIN_SESSION_H

class SystemManager;
class AuthManager;

class AdminSession {
public:
    static void run(SystemManager& system, AuthManager& auth);
};

#endif // ADMIN_SESSION_H
