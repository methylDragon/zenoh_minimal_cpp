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

find_package(ament_cmake REQUIRED)
find_package(zenoh_ament REQUIRED)

include_directories(include)

add_executable(session_test src/session_test.cpp)
ament_target_dependencies(session_test zenoh_ament)

add_executable(session_test_string src/session_test_string.cpp)
ament_target_dependencies(session_test_string zenoh_ament)

add_executable(test_sub src/test_sub.cpp)
ament_target_dependencies(test_sub zenoh_ament)

add_executable(test_sub_bytes src/test_sub_bytes.cpp)
ament_target_dependencies(test_sub_bytes zenoh_ament)

add_executable(test_pub src/test_pub.cpp)
ament_target_dependencies(test_pub zenoh_ament)

add_executable(test_pubsub src/test_pubsub.cpp)
ament_target_dependencies(test_pubsub zenoh_ament)

add_executable(test_queryable src/test_queryable.cpp)
ament_target_dependencies(test_queryable zenoh_ament)

add_executable(test_query src/test_query.cpp)
ament_target_dependencies(test_query zenoh_ament)

install(TARGETS
  session_test_string
  session_test
  test_sub
  test_sub_bytes
  test_pub
  test_pubsub
  test_query
  test_queryable
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
