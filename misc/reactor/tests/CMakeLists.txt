cmake_minimum_required(VERSION 2.0)

project(test)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_PARENT_DIR ${PARENT_DIR} DIRECTORY)

message(status "source directory is: " ${PROJECT_SOURCE_DIR})
message(status "parent directory is: " ${PARENT_DIR})
message(status "parent parent directory is: " ${PARENT_PARENT_DIR})

add_compile_options(-g -D_EPOLL_ET_MODE_)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PARENT_PARENT_DIR} ${PARENT_DIR} ${PARENT_PARENT_DIR}/include)

add_executable(blocking_queue_test
blocking_queue_test.cpp
)

add_executable(reactor_test
reactor_test.cpp
${PARENT_DIR}/reactor.cpp
)

target_link_libraries(blocking_queue_test
pthread
)

target_link_libraries(reactor_test
pthread
)
