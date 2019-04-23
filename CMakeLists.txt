cmake_minimum_required(VERSION 3.8)


# Download and unpack googletest at configure time
configure_file(gtest/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

  if(ENABLE_COVERAGE)
    message("Enable coverage")
    add_compile_options(-lgcov --coverage -O0)
  endif()
endif()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(   -Wall
                         -Wextra # reasonable and standard
                         -Wshadow # warn the user if a variable declaration shadows one from a
                                  # parent context
                         -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                                            # non-virtual destructor. This helps catch hard to
                                            # track down memory errors
                         -Wold-style-cast # warn for c-style casts
                         -Wcast-align # warn for potential performance problem casts
                         -Wunused # warn on anything being unused
                         -Woverloaded-virtual # warn if you overload (not override) a virtual
                                              # function
                         -Wpedantic # warn if non-standard C++ is used
                         -Wconversion # warn on type conversions that may lose data
                         -Wsign-conversion # warn on sign conversions
                         -Wmisleading-indentation # warn if identation implies blocks where blocks
                                                  # do not exist
                         -Wduplicated-cond # warn if if / else chain has duplicated conditions
                         -Wduplicated-branches # warn if if / else branches have duplicated code
                         -Wlogical-op # warn about logical operations being used where bitwise were
                                      # probably wanted
                         -Wnull-dereference # warn if a null dereference is detected
                         -Wuseless-cast # warn if you perform a cast to the same type
                         -Wdouble-promotion # warn if float is implicit promoted to double
                         -Wformat=2 # warn on security issues around functions that format output
                                    # (ie printf)
                         )
endif()

add_executable(intro main.cpp)
target_compile_features(intro PRIVATE cxx_std_17)
target_link_libraries(intro --coverage)

enable_testing()

add_executable(tester tester.cpp)
target_link_libraries(tester --coverage)
add_test(Tester tester)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/gtest/*.cc)

add_executable(example ${TEST_SRC_FILES})
target_link_libraries(example gtest_main --coverage)
add_test(NAME example_test COMMAND example)

