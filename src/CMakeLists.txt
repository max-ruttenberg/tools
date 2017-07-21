include_directories("${PROJECT_SOURCE_DIR}/include")
set(EMUTECHNOLOGY_SOURCES "table.c")
add_library(emutechnology STATIC ${EMUTECHNOLOGY_SOURCES})

