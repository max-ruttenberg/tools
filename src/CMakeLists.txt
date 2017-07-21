include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(emutechnology SHARED table.c)
install(TARGETS emutechnology LIBRARY DESTINATION lib)
