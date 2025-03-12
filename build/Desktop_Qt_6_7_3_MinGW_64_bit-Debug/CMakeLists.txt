# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(YourProjectName VERSION 0.1 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Qt package (adjust Qt version if needed)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Add source files for your project
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Create the executable with the sources
qt_add_executable(YourProjectName
    ${PROJECT_SOURCES}
)

# Link the Qt Widgets library
target_link_libraries(YourProjectName PRIVATE Qt6::Widgets)

# If needed, include other libraries like QtSql
# find_package(Qt6 REQUIRED COMPONENTS Sql)
# target_link_libraries(YourProjectName PRIVATE Qt6::Sql)
