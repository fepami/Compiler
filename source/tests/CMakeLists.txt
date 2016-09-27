cmake_minimum_required(VERSION 2.6)

add_executable(lex_test lex_test.c ../src/lex.c ../src/util.c ../src/token.c)
target_link_libraries(lex_test cmocka)
add_test(lex_test ${CMAKE_CURRENT_BINARY_DIR}/lex_test)

