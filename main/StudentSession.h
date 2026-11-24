#ifndef STUDENT_SESSION_H
#define STUDENT_SESSION_H

#include <string>

class SystemManager;

class StudentSession {
public:
    static void run(SystemManager& system, const std::string& studentID);
};

#endif // STUDENT_SESSION_H
