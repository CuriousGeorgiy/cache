add_executable(cache-end-to-end-tests
               Main.cpp
               lookup-hits/LFULookupHitsTest.cpp
               )
target_link_libraries(cache-end-to-end-tests
                      cache
                      doctest
                      )