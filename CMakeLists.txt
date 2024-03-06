cmake_minimum_required(VERSION 3.21)

project(SimpleFileHash VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(cmake/get_cpm.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage(
    NAME            cryptopp-cmake
    GIT_REPOSITORY  "https://github.com/abdes/cryptopp-cmake.git"
    GIT_TAG         "CRYPTOPP_8_9_0"
    OPTIONS         "CRYPTOPP_BUILD_TESTING OFF" "CRYPTOPP_INSTALL OFF"
)

find_package(QT
    NAMES Qt6
    REQUIRED COMPONENTS
        Widgets
        Concurrent
        LinguistTools
        Test
)

find_package(Qt6
    REQUIRED COMPONENTS
        Widgets
        Concurrent
        LinguistTools
        Test
)

set(REQUIRED_INCLUDES
    ${cryptopp_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TS_FILES
    SimpleFileHash_en.ts
)

set(TRANSLATE_SOURCES
    main.cpp
    hashalgo.hpp
    filehashapplication.hpp
    filehashapplication.cpp
    hashtask.hpp
    hashtask.cpp
    hashprogressitemdelegate.hpp
    hashprogressitemdelegate.cpp
    hashtasksmodel.hpp
    hashtasksmodel.cpp
    hashmatchwindow.hpp
    hashmatchwindow.cpp
    hashmatchwindow.ui
    preferencesdialog.hpp
    preferencesdialog.cpp
    preferencesdialog.ui
    hashingjob.hpp
    hashingjob.cpp
    usersettings.hpp
    usersettings.cpp
    md5hash.hpp
    md5hash.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
)

set(PROJECT_SOURCES
    ${TRANSLATE_SOURCES}
    ${TS_FILES}
)

add_subdirectory(tests)

qt_add_executable(SimpleFileHash
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

qt_add_translations(SimpleFileHash
    TS_FILES ${TS_FILES}
    SOURCES ${TRANSLATE_SOURCES}
)

target_link_libraries(SimpleFileHash
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Concurrent
    PRIVATE cryptopp::cryptopp
)

set_target_properties(SimpleFileHash PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER simplefilehash.kirhut.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS SimpleFileHash
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_finalize_executable(SimpleFileHash)
