cmake_minimum_required(VERSION 2.6)

project(tfidf)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(.)
include_directories(..)
include_directories(../src)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(manifestTests manifest_test.cc)
target_compile_options (manifestTests PRIVATE -std=c++11)
target_link_libraries(manifestTests ${GTEST_LIBRARIES} pthread)

SET(OBJS ../src/manifest.o  ../src/util.o)

add_executable(buildIndexTests buildIndex_test.cc ${OBJS})

SET_SOURCE_FILES_PROPERTIES(
  ${OBJS}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

#FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_compile_options (buildIndexTests PRIVATE -std=c++11)
target_link_libraries(buildIndexTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(buildIndexTests boost_serialization)