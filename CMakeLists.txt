cmake_minimum_required(VERSION 2.8.12)

project(DB)

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)
OPTION(ENABLE_GPROF "Enable gprof" OFF)
OPTION(BUILD_TEST "Build test (will download and build Google Test)" OFF)
OPTION(REBUILD_PARSER "Rebuild parser (Java runtime needed)" OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

IF (ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
ENDIF()

IF (ENABLE_GPROF)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")
ENDIF()

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(ANTLR4CPP_JAR_LOCATION ${CMAKE_BINARY_DIR}/third-party/antlr-4.7-complete.jar)
set(ANTLR4CPP_GENERATED_SRC_DIR ${CMAKE_BINARY_DIR}/src/parser)
configure_file(antlr4.CMakeLists.txt.in ${CMAKE_BINARY_DIR}/third-party/antlr4/download/CMakeLists.txt)
add_subdirectory(${CMAKE_BINARY_DIR}/third-party/antlr4/download)

# add external build for antlrcpp
# add antrl4cpp artifacts to project environment
include_directories(${CMAKE_BINARY_DIR}/third-party/antlr4/include/antlr4-runtime)
foreach(src_path misc atn dfa tree support)
    include_directories(${CMAKE_BINARY_DIR}/third-party/antlr4/include/antlr4-runtime/${src_path})
endforeach(src_path)
link_directories(${CMAKE_BINARY_DIR}/third-party/antlr4/lib)

if (REBUILD_PARSER)
    find_package(Java COMPONENTS Runtime REQUIRED)
    set(GRAMMAR_OUTPUT
        ${CMAKE_BINARY_DIR}/src/parser/SqlLexer.cpp
        ${CMAKE_BINARY_DIR}/src/parser/SqlLexer.h
        ${CMAKE_BINARY_DIR}/src/parser/SqlParser.cpp
        ${CMAKE_BINARY_DIR}/src/parser/SqlParser.h
    )
    set(GRAMMAR_LEXER ${CMAKE_BINARY_DIR}/src/parser/SqlLexer.g4)
    set(GRAMMAR_PARSER ${CMAKE_BINARY_DIR}/src/parser/SqlParser.g4)

    add_custom_command(
        OUTPUT ${GRAMMAR_OUTPUT}
        COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4CPP_JAR_LOCATION}" -Werror -Dlanguage=Cpp -no-listener -no-visitor -o "${ANTLR4CPP_GENERATED_SRC_DIR}" "${GRAMMAR_LEXER}" "${GRAMMAR_PARSER}"
        DEPENDS "${GRAMMAR_LEXER}" "${GRAMMAR_PARSER}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

file(GLOB_RECURSE SRCS ${CMAKE_BINARY_DIR}/src/*.cpp)
list(REMOVE_ITEM SRCS ${CMAKE_BINARY_DIR}/src/main.cpp)
if (REBUILD_PARSER)
    list(APPEND SRCS ${GRAMMAR_OUTPUT})
endif()

add_executable(db ${SRCS} ${CMAKE_BINARY_DIR}/src/main.cpp)
add_dependencies(db antlr4cpp)
target_link_libraries(db antlr4-runtime.a)

set_target_properties(
    db
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED true
)

if (BUILD_TEST)
    file(GLOB_RECURSE TESTS ${CMAKE_BINARY_DIR}/test/*.cpp)

    add_executable(db_test ${SRCS} ${TESTS})
    add_dependencies(db_test antlr4cpp)
    target_link_libraries(db_test antlr4-runtime)

    target_include_directories(db_test PRIVATE ${CMAKE_BINARY_DIR}/src)

    set_target_properties(
        db_test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED true
    )

    # Download and unpack googletest at configure time
    configure_file(gtest.CMakeLists.txt.in third-party/googletest/download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/third-party/googletest/download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/third-party/googletest/download )
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
endif()
