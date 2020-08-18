cmake_minimum_required(VERSION 3.5)
project(zenoh_minimal_cpp)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_executable(session_test_string src/session_test_string.cpp)
target_link_libraries(session_test_string ${CMAKE_SOURCE_DIR}/lib/libzenoh_ffi.so)

add_executable(test_sub src/test_sub.cpp)
target_link_libraries(test_sub ${CMAKE_SOURCE_DIR}/lib/libzenoh_ffi.so)

add_executable(test_pub src/test_pub.cpp)
target_link_libraries(test_pub ${CMAKE_SOURCE_DIR}/lib/libzenoh_ffi.so)

add_executable(test_pubsub src/test_pubsub.cpp)
target_link_libraries(test_pubsub ${CMAKE_SOURCE_DIR}/lib/libzenoh_ffi.so)
