cmake_minimum_required(VERSION 3.16)

project(ServerArm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIR "C:/Appli/OpenCV/opencv/build/x64/vc16/lib")

set(ASTRA_SDK_PATH  ${PROJECT_SOURCE_DIR}/includes/AstraSDK/)
set(ASTRA_SDK_LIBS
        astra.lib
        astra_core.lib
        astra_core_api.lib
        astra_jni.lib
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(OpenCV REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/server.cpp
        src/computervision.cpp
)

set(PROJECT_HEADERS
        includes/mainwindow.h
        includes/computervision.h
        includes/msg_Structure.h
        includes/server.h
)

set(PROJECT_FORMS
       src/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ServerArm
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_FORMS}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ServerArm APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ServerArm SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_FORMS}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ServerArm
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_FORMS}
        )
    endif()
endif()

target_include_directories(ServerArm PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${ASTRA_SDK_PATH}/include

        ${PROJECT_SOURCE_DIR}/includes
)

target_link_directories(ServerArm PRIVATE ${ASTRA_SDK_PATH}/lib)

target_link_libraries(ServerArm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network)
target_link_libraries(ServerArm PRIVATE
        ${OpenCV_LIBS}
        ${ASTRA_SDK_LIBS}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ServerArm)
endif()
set_target_properties(ServerArm PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ServerArm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ServerArm)
endif()
