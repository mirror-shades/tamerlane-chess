cmake_minimum_required(VERSION 3.10)
project(Tamerlane-Chess)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect operating system and set compiler flags
if(APPLE)
    # Add MacOS specific compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    
    # If using ARM64 (M1/M2 Macs)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Mac-ARM64/lib/cmake/SFML")
    else()
        # For Intel Macs
        set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Mac/lib/cmake/SFML")
    endif()
    
    # Add rpath for SFML libraries
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
elseif(WIN32)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Windows/lib/cmake/SFML")
elseif(UNIX AND NOT APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Linux/lib/cmake/SFML")
endif()

# Find SFML packages
find_package(SFML 2.6.1 COMPONENTS system window graphics audio REQUIRED)

# Collect all source files
set(SOURCES 
    src/main.cpp
    src/ai.cpp
    src/chessboard.cpp
    src/gameLogic.cpp
    src/pieceLogic.cpp
    src/render.cpp
    src/state.cpp
    src/utility.cpp
)

# Add executable with all source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
)

# Add Windows-specific DLL copying
if(WIN32)
    # Get the configuration type (Debug/Release)
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
    endif()

    # Determine which DLLs to copy based on configuration
    if(CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
        set(SFML_DLL_SUFFIX "-d-2.dll")
    else()
        set(SFML_DLL_SUFFIX "-2.dll")
    endif()

    # Add commands to copy DLLs to output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-system${SFML_DLL_SUFFIX}"
            "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-window${SFML_DLL_SUFFIX}"
            "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-graphics${SFML_DLL_SUFFIX}"
            "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-audio${SFML_DLL_SUFFIX}"
            "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/openal32.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Copy SFML DLLs for MinGW
if(WIN32 AND MINGW)
    # Copy the required DLLs to the build directory
    file(COPY 
        "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-system-2.dll"
        "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-window-2.dll"
        "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-graphics-2.dll"
        "${CMAKE_SOURCE_DIR}/external/SFML-Windows/bin/sfml-audio-2.dll"
        DESTINATION ${CMAKE_BINARY_DIR}
    )
endif()

# Add include directories - corrected for each platform
if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/SFML-Windows/include
    )
elseif(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        target_include_directories(${PROJECT_NAME} PRIVATE 
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/external/SFML-Mac-ARM64/include
        )
    else()
        target_include_directories(${PROJECT_NAME} PRIVATE 
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/external/SFML-Mac/include
        )
    endif()
else()
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/SFML-Linux/include
    )
endif()

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()