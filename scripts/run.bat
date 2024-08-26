@echo off
:: run with flag "release" for release build
if "%1"=="" (
    set BUILD_TYPE=development
) else (
    set BUILD_TYPE=%1
)

echo Building with configuration: %BUILD_TYPE%

:: Store the current directory
set CUR_DIR=%CD%

:: Navigate to the project root directory (tamerlane-cpp)
cd /d %~dp0
cd ..
echo Current directory: %CD%

:: Check if Makefile exists
if not exist Makefile (
    echo Error: Makefile not found in %CD%
    goto :error
)

:: Clean the build directory
mingw32-make -f Makefile BUILD_TYPE=%BUILD_TYPE% clean

:: Execute the build
mingw32-make -f Makefile BUILD_TYPE=%BUILD_TYPE%

:: Check if the build was successful
if %ERRORLEVEL% EQU 0 (
    if not "%BUILD_TYPE%"=="release" (
        .\build\main.exe
    ) else (
        echo Build successful. Executable not run in release mode.
    )
) else (
    echo Build failed.
)

:end
cd /d "%CUR_DIR%"
exit /b 0

:error
cd /d "%CUR_DIR%"
exit /b 1