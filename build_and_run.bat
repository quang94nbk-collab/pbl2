@echo off
chcp 65001 > nul
echo ========================================================
echo   HE THONG DANG KY HOC PHAN - BIEN DICH VA CHAY (PBL)
echo ========================================================

set PATH=C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;%PATH%

REM Xoa file cache gay loi o thu muc goc
if exist CMakeCache.txt del /f /q CMakeCache.txt > nul 2>&1
if exist CMakeFiles rmdir /s /q CMakeFiles > nul 2>&1

if not exist build (
    mkdir build
)

REM Dong bo 5 file du lieu vao thu muc build neu chua co
for %%f in (curriculums.txt courses.txt students.txt sections.txt registrations.txt) do (
    if exist "%%f" (
        if not exist "build\%%f" copy "%%f" "build\" > nul 2>&1
    )
)

cd build

REM Dam bao khong con CMakeCache.txt o thu muc cha
if exist ..\CMakeCache.txt del /f /q ..\CMakeCache.txt > nul 2>&1

echo.
echo [1/2] Dang cau hinh CMake voi Qt6 va Ninja...
cmake -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -DCMAKE_C_COMPILER="C:/Qt/Tools/mingw1310_64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" ..
if %errorlevel% neq 0 (
    echo [LOI] Khong the cau hinh du an!
    pause
    exit /b %errorlevel%
)

echo.
echo [2/2] Dang bien dich chuong trinh...
ninja
if %errorlevel% neq 0 (
    echo [LOI] Bien dich that bai!
    pause
    exit /b %errorlevel%
)

echo.
echo ========================================================
echo   BIEN DICH THANH CONG! DANG KHOI CHAY UNG DUNG...
echo ========================================================
PBL_StudentRegistration.exe

REM Chi dong bo 5 file du lieu ra thu muc goc, khong sao chep CMakeCache
for %%f in (curriculums.txt courses.txt students.txt sections.txt registrations.txt) do (
    if exist "%%f" copy /y "%%f" "..\%%f" > nul 2>&1
)
