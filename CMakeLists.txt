cmake_minimum_required(VERSION 2.8.12)

project(DB)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$-Wall -DNDEBUG -O3")

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

aux_source_directory(src SRCS)
aux_source_directory(test TESTS)

add_executable(db ${SRCS})
add_executable(db_test ${TESTS})

set_target_properties(
    db
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED true
)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/third-party/googletest/src
                 ${CMAKE_BINARY_DIR}/third-party/googletest/build)

target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
    "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(db_test gmock_main)
add_test(NAME db_test COMMAND db_test)
