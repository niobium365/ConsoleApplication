cmake_minimum_required(VERSION 3.19.1)

add_compile_options(-v -Wall 
-fno-function-sections -fno-data-sections -Wextra -Wno-unused-parameter -pthread 
-Wno-missing-field-initializers
-Wno-unused-variable
#-stdlib=libc++
-std=gnu++14 
#-m32
-gdwarf-2 -ggdb
)

project(MyProject)

set(helloworld_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/test_task.cpp
)

set(_target "test_task")

add_executable(${_target} ${helloworld_SRC})

target_compile_options(${_target} PUBLIC $<$<COMPILE_LANGUAGE:C>:
-std=gnu11
-Wall
-fno-strict-aliasing
-O0
-fno-omit-frame-pointer
-fno-short-enums
-Wno-deprecated-declarations
-fno-addrsig
>
$<$<COMPILE_LANGUAGE:CXX>:
-std=gnu++14 
-fexceptions
-O0
>
)

target_include_directories(${_target} PUBLIC
)

target_link_options(${_target} PUBLIC 
-v 
-Wl,-stats
-Wl,--build-id=none
-Wl,--allow-multiple-definition
-Wl,-no-demangle
#-m32
)

target_link_libraries(${_target} PUBLIC 
c++
c++abi
)
