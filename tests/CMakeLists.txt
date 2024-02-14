## SimpleFileHash CMake Test Folder File
##
## Copyright (C) 2024 Ezekiel Oruven

cmake_minimum_required(VERSION 3.21)

enable_testing()

set(TESTS_TO_EXECUTE
    HashTasksModel
    HashThreadComm
    HashTask
)

set(TEST_SOURCE_FILES ${TRANSLATE_SOURCES})
list(TRANSFORM TEST_SOURCE_FILES PREPEND "${SimpleFileHash_SOURCE_DIR}/")

configure_file(TestBlock.txt TestBlock.txt COPYONLY)

add_library(libTestSimpleFileHash STATIC ${TEST_SOURCE_FILES})
target_include_directories(libTestSimpleFileHash PRIVATE ${REQUIRED_INCLUDES})
target_link_libraries(libTestSimpleFileHash
    PRIVATE Qt6::Widgets
    PRIVATE cryptopp::cryptopp
)

foreach(TESTNAME ${TESTS_TO_EXECUTE})
    add_executable(SFHTest${TESTNAME} Test${TESTNAME}.cpp)
    add_test(NAME ${TESTNAME} COMMAND SFHTest${TESTNAME})
    target_include_directories(SFHTest${TESTNAME} PRIVATE ${REQUIRED_INCLUDES})
    target_link_libraries(SFHTest${TESTNAME} PRIVATE libTestSimpleFileHash Qt6::Test)
endforeach()
