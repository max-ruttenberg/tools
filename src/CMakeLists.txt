include_directories("${PROJECT_SOURCE_DIR}/include")
set(TOOLS_SOURCES "table.c")
add_library(tools STATIC ${TOOLS_SOURCES})

