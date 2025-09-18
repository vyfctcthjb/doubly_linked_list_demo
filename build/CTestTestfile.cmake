# CMake generated Testfile for 
# Source directory: /Users/jguida941/doubly_linked_list_demo
# Build directory: /Users/jguida941/doubly_linked_list_demo/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[repl_push_print_size]=] "bash" "/Users/jguida941/doubly_linked_list_demo/tests/test_generic_repl.sh" "/Users/jguida941/doubly_linked_list_demo/build/generic_demo")
set_tests_properties([=[repl_push_print_size]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/jguida941/doubly_linked_list_demo/CMakeLists.txt;23;add_test;/Users/jguida941/doubly_linked_list_demo/CMakeLists.txt;0;")
add_test([=[dll_demo_output]=] "bash" "/Users/jguida941/doubly_linked_list_demo/tests/test_dll_demo.sh" "/Users/jguida941/doubly_linked_list_demo/build/doubly_linked_list_demo")
set_tests_properties([=[dll_demo_output]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/jguida941/doubly_linked_list_demo/CMakeLists.txt;26;add_test;/Users/jguida941/doubly_linked_list_demo/CMakeLists.txt;0;")
