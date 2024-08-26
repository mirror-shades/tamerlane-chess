if "%1"=="" (
    set BUILD_TYPE=debug
) else (
    set BUILD_TYPE=%1
)

echo Building with configuration: %BUILD_TYPE%

:: Clean the build directory
mingw32-make -f Makefile BUILD_TYPE=%BUILD_TYPE% clean

:: Execute the build
mingw32-make -f Makefile BUILD_TYPE=%BUILD_TYPE%

:: Check if the build was successful
if %ERRORLEVEL% EQU 0 (
    .\build\main.exe
) else (
    echo Build failed.
)

endlocal