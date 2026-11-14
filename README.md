## Build and run

From the project root, compile with a C++14-compatible compiler:

```powershell
g++ -std=c++14 -Wall -Wextra -pedantic main\main.cpp -o pbl2.exe
.\pbl2.exe
```
----.\run_gui.bat chạy gui

The program creates its data files in the current working directory when you
save and exit. Keep `students.txt`, `courses.txt`, `sections.txt`, and
`registrations.txt` together when moving the saved data.

The application separates administrator and student menus. Administrators can
add students, courses, and class sections, manage registrations, and view all
records. Students can view courses and class sections, manage only their own
registrations, and view their own information.

## Login accounts

The default administrator account is `admin` with password `admin`. Each
student logs in with their student ID as both username and initial password.
When an administrator adds a student, the program creates that student's
account immediately. Student accounts are recreated from `students.txt` at
startup, so no separate account file is needed.

This is a simple local/demo login: passwords are compared directly and student
passwords are derived from their IDs. Do not use real passwords with it; a
deployed application needs password changes, secure password hashing, and
account persistence.
