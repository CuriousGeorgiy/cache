add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
   CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"
   )
    target_compile_options(doctest INTERFACE /EHs)
endif()