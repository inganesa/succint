cmake_minimum_required(VERSION 2.6)

project(tfidf)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(.)
include_directories(../src)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(manifestTests manifest_test.cc)
target_compile_options (manifestTests PRIVATE -std=c++11)
target_link_libraries(manifestTests ${GTEST_LIBRARIES} pthread)