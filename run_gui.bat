@echo off
:: ============================================================
::  run_gui.bat - Chay PBL2 Qt GUI
::  Phai chay bat file nay tu thu muc pbl2/ (hoac double-click
::  neu bat file nam trong pbl2/)
:: ============================================================

:: Chuyen ve thu muc chua bat file (= thu muc pbl2/)
cd /d "%~dp0"

:: Them duong dan Qt vao PATH de tim DLL
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.12.0\mingw_64\bin;%PATH%

echo Khoi dong He thong Dang ky Tin chi - PBL2 (Qt GUI)...
echo Thu muc lam viec: %CD%
echo.

start "" "qt_ui\build\PBL2_QtGUI.exe"

