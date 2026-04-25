@echo off
echo =================================
echo Test Data Generator
echo =================================
echo.

REM 检查 generator.exe 是否存在
if not exist generator.exe (
    echo [ERROR] generator.exe not found!
    echo Please build the project first.
    pause
    exit /b 1
)

echo [INFO] Generating test2.csv with 10,000 flight records...

REM 运行生成器并输出到 test2.csv
generator.exe > test2.csv

if errorlevel 1 (
    echo [ERROR] Failed to generate test2.csv
    pause
    exit /b 1
)

echo [SUCCESS] test2.csv generated successfully!
for /f "tokens=3" %%a in ('dir /-C test2.csv ^| find "test2.csv"') do echo [INFO] File size: %%a bytes
echo.
echo =================================
echo Generation completed!
echo =================================
pause