cmake_minimum_required(VERSION 2.8)

project(coroutine)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

# elf
set(EXECUTABLE_OUTPUT_PATH ${PARENT_DIR}/bin)
# .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PARENT_DIR}/lib)
# .so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PARENT_DIR}/lib)

# set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
add_compile_options(
-g
-fno-strict-aliasing
-O2
-std=c++11
-Wall
-export-dynamic
-pipe
-D_GNU_SOURCE
-D_REENTRANT
-fPIC
-Wno-deprecated
-m64
)

enable_language(C ASM)

include_directories(
${PARENT_DIR}
)

set(SOURCE_FILES
${PARENT_DIR}/co_epoll.cpp
${PARENT_DIR}/co_routine.cpp
${PARENT_DIR}/co_hook.cpp
${PARENT_DIR}/co_ctx.cpp
${PARENT_DIR}/co_ctx_swap.S
)

add_library(coroutine_static STATIC ${SOURCE_FILES})
add_library(coroutine_shared SHARED ${SOURCE_FILES})

set_target_properties(coroutine_static PROPERTIES OUTPUT_NAME coroutine)
set_target_properties(coroutine_shared PROPERTIES OUTPUT_NAME coroutine)

set_target_properties(coroutine_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(coroutine_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

macro(add_example EXAMPLE)
	add_executable("${EXAMPLE}_test" "${PROJECT_SOURCE_DIR}/${EXAMPLE}_test.cpp")
	target_link_libraries("${EXAMPLE}_test" coroutine_static pthread dl)
endmacro(add_example)

add_example(setenv)
add_example(cond)
add_example(copystack)
