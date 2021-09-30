cmake_minimum_required(VERSION 3.1)

project(cache)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
   CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU"
   )
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -stdlib=libc++)
endif()

add_subdirectory(cache)
add_subdirectory(doctest)
add_subdirectory(stdin-ideal-test)
add_subdirectory(stdin-lfu-test)