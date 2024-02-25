## SimpleFileHash CMake Test Folder File
##
## Copyright (C) 2024 Ezekiel Oruven

cmake_minimum_required(VERSION 3.21)

enable_testing()

set(TESTS_TO_EXECUTE
    HashTask
    HashingJob
    HashTasksModel
)

set(TEST_SOURCE_FILES ${TRANSLATE_SOURCES})
list(TRANSFORM TEST_SOURCE_FILES PREPEND "${SimpleFileHash_SOURCE_DIR}/")

configure_file(SmallBlock.txt SmallBlock.txt COPYONLY)
configure_file(TestBlock1.txt tfolder/TestBlock1.txt COPYONLY)
configure_file(TestBlock2.txt tfolder/TestBlock2.txt COPYONLY)
configure_file(TestBlock3.txt tfolder/TestBlock3.txt COPYONLY)
configure_file(TestBlock4.txt tfolder/TestBlock4.txt COPYONLY)
configure_file(TestBlock5.txt tfolder/TestBlock5.txt COPYONLY)
configure_file(LargeBlock1.txt lfolder/LargeBlock1.txt COPYONLY)
configure_file(LargeBlock2.txt lfolder/LargeBlock2.txt COPYONLY)
configure_file(LargeBlock3.txt lfolder/LargeBlock3.txt COPYONLY)

add_library(libTestSimpleFileHash STATIC ${TEST_SOURCE_FILES})
target_include_directories(libTestSimpleFileHash PRIVATE ${REQUIRED_INCLUDES})
target_compile_definitions(libTestSimpleFileHash PRIVATE SFH_TEST_BUILD)
target_link_libraries(libTestSimpleFileHash
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Concurrent
    PRIVATE cryptopp::cryptopp
)

foreach(TESTNAME ${TESTS_TO_EXECUTE})
    add_executable(SFHTest${TESTNAME} Test${TESTNAME}.cpp)
    add_test(NAME ${TESTNAME} COMMAND SFHTest${TESTNAME})
    target_include_directories(SFHTest${TESTNAME} PRIVATE ${REQUIRED_INCLUDES})
    target_link_libraries(SFHTest${TESTNAME} PRIVATE libTestSimpleFileHash Qt6::Test)
endforeach()
