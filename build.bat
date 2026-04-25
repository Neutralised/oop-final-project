@echo off
echo =================================
echo Flight Reservation System Builder
echo =================================
echo.

REM Remove old CMake cache
if exist build\CMakeCache.txt del /q build\CMakeCache.txt

REM Remove CMakeFiles directory
if exist build\CMakeFiles rmdir /s /q build\CMakeFiles 2>nul

REM Remove Visual Studio files if they exist
if exist build\main.vcxproj rmdir /s /q build\main.vcxproj 2>nul
if exist build\main.vcxproj.filters rmdir /s /q build\main.vcxproj.filters 2>nul
if exist build\main.vcxproj.user rmdir /s /q build\main.vcxproj.user 2>nul
if exist build\FlightReservationSystem.sln del /q build\FlightReservationSystem.sln 2>nul
if exist build\FlightReservationSystem.vcxproj del /q build\FlightReservationSystem.vcxproj 2>nul

REM Create build directory if it doesn't exist
if not exist build mkdir build

echo.
echo [1] Configuring CMake with MinGW...
cd /d build
cmake -G "MinGW Makefiles" ..
if errorlevel 1 (
    echo Configuration failed!
    pause
    exit /b 1
)

echo.
echo [2] Building project with MinGW...
mingw32-make
if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo =================================
echo Build completed successfully!
echo Executable: main.exe
echo Test data generator: tests/generator.exe
echo =================================
echo.
pause