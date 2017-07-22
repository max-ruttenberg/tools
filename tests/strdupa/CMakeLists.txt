include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_BINARY_DIR}/src)

add_executable(strdupa EXCLUDE_FROM_ALL strdupa.c)
add_dependencies(strdupa tools)

add_test(NAME build_strdupa COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target strdupa)
add_test(NAME strdupa COMMAND strdupa leaf-routine)
set_tests_properties(strdupa PROPERTIES DEPENDS build_strdupa)

target_link_libraries(strdupa -ltools)
