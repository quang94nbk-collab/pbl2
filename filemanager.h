#include "SystemManager.h"
#include <string>
class FileManager {
private:
    string studentFile = "data/students.txt";
    string courseFile = "data/courses.txt";
    string sectionFile = "data/sections.txt";
    string registrationFile = "data/registrations.txt";

public:
    void loadAllData(SystemManager& sys);
    void saveAllData(SystemManager& sys);
};
