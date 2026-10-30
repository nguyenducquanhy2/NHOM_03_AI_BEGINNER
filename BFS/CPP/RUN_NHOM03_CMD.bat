@echo off
setlocal
chcp 65001 >nul
cd /d "%~dp0"
title BREADTH-FIRST SEARCH - NHOM 3
set "CXX=g++"
where g++ >nul 2>nul
if not errorlevel 1 goto compile
for %%P in ("C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe" "C:\Program Files\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe" "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe" "C:\Dev-Cpp\MinGW64\bin\g++.exe") do if exist %%P (set "CXX=%%~P" & goto compile)
echo [LOI] Khong tim thay g++. Cai Dev-C++/MinGW hoac them g++.exe vao PATH.
pause
exit /b 1
:compile
"%CXX%" -std=c++11 bfs.cpp -o bfs.exe
if errorlevel 1 (echo [LOI] Bien dich that bai.& pause & exit /b 1)
echo [OK] Bien dich thanh cong.
bfs.exe input.txt output.txt
