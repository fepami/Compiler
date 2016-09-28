include_directories(${INCLUDE_DIR} ${CMOCKA_INCLUDE_DIR})

add_executable(lex_test lex_test.c ${SOURCE_DIR}/lex.c ${SOURCE_DIR}/util.c ${SOURCE_DIR}/token.c)
target_link_libraries (lex_test ${CMOCKA_LIBRARIES})
add_test(lex_test ${CMAKE_CURRENT_BINARY_DIR}/lex_test)