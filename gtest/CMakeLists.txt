add_executable(
    unit_tests
    example_add.cpp
    example_subtract.cpp
    example_multiply.cpp
    example_divide.cpp
    )

target_link_libraries(
    unit_tests
    gtest_main 
    --coverage
    example
    )

add_test(
  NAME
    unit
  COMMAND
    unit_tests
  )
