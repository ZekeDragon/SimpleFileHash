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
    SimpleFileHash_es.ts
    SimpleFileHash_fr.ts
    SimpleFileHash_pl.ts
    SimpleFileHash_ko.ts
    SimpleFileHash_sv.ts
)

set(TRANSLATE_SOURCES
    sfhbase.hpp
    hashalgo.hpp
    hashalgo.cpp
    contextinstall.hpp
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
    algocheckbox.hpp
    algocheckbox.cpp
    preferencesdialog.hpp
    preferencesdialog.cpp
    preferencesdialog.ui
    hashingjob.hpp
    hashingjob.cpp
    usersettings.hpp
    usersettings.cpp
    md5hash.hpp
    md5hash.cpp
    md6/md6.hpp
    md6/md6_compress.cpp
    md6/md6_mode.cpp
    mainwindow.hpp
    mainwindow.cpp
    mainwindow.ui
)

if(WIN32)
    list(APPEND TRANSLATE_SOURCES wincontextinstall.cpp)
else()
    list(APPEND TRANSLATE_SOURCES contextinstall.cpp)
endif()

set(PROJECT_SOURCES
    ${TRANSLATE_SOURCES}
    ${TS_FILES}
)

qt_add_library(libSimpleFileHash
    STATIC
    ${PROJECT_SOURCES}
)

target_link_libraries(libSimpleFileHash
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Concurrent
    PRIVATE cryptopp::cryptopp
)

add_subdirectory(tests)

qt_add_executable(SimpleFileHash
    MANUAL_FINALIZATION
    main.cpp
)

target_link_libraries(SimpleFileHash
    PRIVATE libSimpleFileHash
)

qt_add_translations(libSimpleFileHash
    TS_FILES ${TS_FILES}
    SOURCES ${TRANSLATE_SOURCES}
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
