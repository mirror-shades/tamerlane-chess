@echo off

:: Store the current directory
set CUR_DIR=%CD%

:: Navigate to the project root directory (tamerlane-cpp)
cd /d %~dp0
cd ..
echo Current directory: %CD%

:: Run the build script with release flag
call scripts\run.bat release

:: Check if the build was successful
if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting...
    cd /d "%CUR_DIR%"
    exit /b %ERRORLEVEL%
)

:: Set the release directory
set RELEASE_DIR=C:\dev\c++\Tamerlane Chess

:: Create the release directory if it doesn't exist
if not exist "%RELEASE_DIR%" mkdir "%RELEASE_DIR%"

:: Copy the executable
copy /Y build\main.exe "%RELEASE_DIR%\"

:: Copy the assets folder
xcopy /E /I /Y assets "%RELEASE_DIR%\assets"

echo Release files have been copied to %RELEASE_DIR%

:: Return to the scripts directory
cd /d "%CUR_DIR%"