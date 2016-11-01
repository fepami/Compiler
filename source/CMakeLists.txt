include_directories(${INCLUDE_DIR})
aux_source_directory(${SOURCE_DIR} SOURCE_FILES)

add_executable(syntatic ${SOURCE_FILES})
install(TARGETS syntatic RUNTIME DESTINATION bin)
