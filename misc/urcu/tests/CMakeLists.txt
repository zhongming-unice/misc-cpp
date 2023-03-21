cmake_minimum_required(VERSION 2.0)

project(test)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_PARENT_DIR ${PARENT_DIR} DIRECTORY)

message(status "source directory is: " ${PROJECT_SOURCE_DIR})
message(status "parent directory is: " ${PARENT_DIR})
message(status "parent parent directory is: " ${PARENT_PARENT_DIR})

add_compile_options(-g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PARENT_PARENT_DIR} ${PARENT_DIR})

add_executable(urcu_test
urcu_test.c
${PARENT_DIR}/urcu.c
)

target_link_libraries(urcu_test
pthread
)
