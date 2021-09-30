add_executable(cache-unit-tests
               Main.cpp
               lookup-hits/LFULookupHitsTest.cpp
               )
target_link_libraries(cache-unit-tests
                      cache
                      doctest
                      )