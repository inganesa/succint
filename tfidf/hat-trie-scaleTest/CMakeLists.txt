cmake_minimum_required(VERSION 3.1)
# Example where the hat-trie project is stored in a third-party directory
add_subdirectory(hat-trie)
project (test)
#FIND_PACKAGE(Boost 1.54.0 COMPONENTS beast REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
#add_library(boost_beast INTERFACE)

#target_include_directories(boost_beast SYSTEM PUBLIC
#                           "${CMAKE_CURRENT_LIST_DIR}/beast/include")
set(SOURCES test.cpp base64.hpp)
add_executable(test ${SOURCES})
target_link_libraries(test PRIVATE tsl::hat_trie)
#target_link_libraries(test boost_beast )