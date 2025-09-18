cmake_minimum_required(VERSION 3.20)
project(doubly_linked_list_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(doubly_linked_list_demo main.cpp)

# Generic templated demo with menu + REPL
add_executable(generic_demo generic_main.cpp)
target_include_directories(generic_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Optional: enable extra checks in Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(doubly_linked_list_demo PRIVATE DLLIST_DEBUG_CHECKS)
  target_compile_definitions(generic_demo PRIVATE DLLIST_DEBUG_CHECKS)
endif()

include(CTest)
enable_testing()

add_test(NAME repl_push_print_size
  COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_generic_repl.sh $<TARGET_FILE:generic_demo>)

add_test(NAME dll_demo_output
  COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_dll_demo.sh $<TARGET_FILE:doubly_linked_list_demo>)
