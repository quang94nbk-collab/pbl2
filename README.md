# Credit Registration Project

## Build and run

From the project root, compile with a C++14-compatible compiler:

```powershell
g++ -std=c++14 -Wall -Wextra -pedantic main\main.cpp -o pbl2.exe
.\pbl2.exe
```

The program creates its data files in the current working directory when you
save and exit. Keep `students.txt`, `courses.txt`, `sections.txt`,
`registrations.txt`, and `semester.txt` together when moving the saved data.

The initial menu supports adding students, courses, and class sections;
registering and cancelling course registrations; listing stored records;
and configuring the active academic semester (term 1 or 2).

## Authentication example

`AuthManager` currently keeps accounts in memory; it is an example API and is
not yet connected to the main menu or saved between runs. A small usage example:

```cpp
#include "manager/authmanager.h"

AuthManager auth;
auth.addAccount("admin", "demo-password", UserRole::Administrator);
auth.addAccount("sv001", "demo-password", UserRole::Student, "SV001");

if (auth.login("sv001", "demo-password")) {
    const UserAccount* user = auth.currentUser();
    // Check user->role and, for a student, use user->studentID.
    auth.logout();
}
```

This in-memory example compares the supplied passwords directly. Do not use
real passwords with it; a deployed application needs secure password hashing,
account persistence, and role checks at each protected operation.
