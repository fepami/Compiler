include_directories(${INCLUDE_DIR})
aux_source_directory(${SOURCE_DIR} SOURCE_FILES)

add_executable(lex ${SOURCE_FILES})
install(TARGETS lex RUNTIME DESTINATION bin)
