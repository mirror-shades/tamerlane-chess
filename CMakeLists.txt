cmake_minimum_required(VERSION 3.10)
project(SFMLGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect operating system
if(WIN32)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Windows/lib/cmake/SFML")
elseif(APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Mac/lib/cmake/SFML")
elseif(UNIX AND NOT APPLE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-Linux/lib/cmake/SFML")
endif()

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)