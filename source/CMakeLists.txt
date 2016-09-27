cmake_minimum_required(VERSION 2.6)

include_directories(inc)

aux_source_directory(src/ SOURCE_FILES)
add_executable(lex ${SOURCE_FILES})

add_subdirectory(tests/)

install(TARGETS lex RUNTIME DESTINATION bin)
