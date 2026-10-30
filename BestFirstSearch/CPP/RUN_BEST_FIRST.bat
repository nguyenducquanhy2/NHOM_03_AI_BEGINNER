@echo off
setlocal
cd /d "%~dp0"
set "CXX=g++"
where g++ >nul 2>nul
if not errorlevel 1 goto compile
for %%P in ("C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe" "C:\Program Files\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe" "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe" "C:\Dev-Cpp\MinGW64\bin\g++.exe") do if exist %%P (set "CXX=%%~P" & goto compile)

echo [LOI] Khong tim thay g++. Cai Dev-C++/MinGW hoac them g++.exe vao PATH.
pause
exit /b 1
:compile
"%CXX%" -std=c++11 best_first.cpp -o best_first.exe
if errorlevel 1 (echo [LOI] Bien dich that bai.& pause & exit /b 1)
best_first.exe input.txt output.txt
pause
