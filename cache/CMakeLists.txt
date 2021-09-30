add_library(cache INTERFACE)
target_include_directories(cache INTERFACE include)

add_subdirectory(end-to-end-tests)
add_subdirectory(unit-tests)