cmake_minimum_required(VERSION 3.10)
project(Tamerlane-Chess)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect operating system
if(WIN32)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Windows/lib/cmake/SFML")
elseif(APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Mac/lib/cmake/SFML")
elseif(UNIX AND NOT APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Linux/lib/cmake/SFML")
endif()

# Find SFML packages
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

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

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/SFML-Linux/include
)

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()