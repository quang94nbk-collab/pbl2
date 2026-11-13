cmake_minimum_required(VERSION 3.16)
project(PBL2_QtGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Tìm Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Source files trong qt_ui/ directory
set(SOURCES
    qt_main.cpp
    LoginDialog.cpp
    MainWindow.cpp
    AdminWindow.cpp
    StudentWindow.cpp
    StudentManagementWidget.cpp
    CourseManagementWidget.cpp
    SectionManagementWidget.cpp
    RegistrationWidget.cpp
    TimetableWidget.cpp
)

set(HEADERS
    LoginDialog.h
    MainWindow.h
    AdminWindow.h
    StudentWindow.h
    StudentManagementWidget.h
    CourseManagementWidget.h
    SectionManagementWidget.h
    RegistrationWidget.h
    TimetableWidget.h
)

add_executable(PBL2_QtGUI ${SOURCES} ${HEADERS})

# Include path lên thư mục cha để dùng manager/, model/, dal/, DSA/
target_include_directories(PBL2_QtGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(PBL2_QtGUI PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Windows: không hiện console
if(WIN32)
    set_target_properties(PBL2_QtGUI PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
