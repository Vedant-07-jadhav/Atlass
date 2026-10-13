add_test([=[OccupancyGridTest.Initialization]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.Initialization]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.Initialization]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:4]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[OccupancyGridTest.InvalidInitialization]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.InvalidInitialization]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.InvalidInitialization]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:12]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[OccupancyGridTest.BoundsChecking]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.BoundsChecking]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.BoundsChecking]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:17]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[OccupancyGridTest.OccupancyReadWrite]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.OccupancyReadWrite]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.OccupancyReadWrite]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:26]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[OccupancyGridTest.CoordinateConversion]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.CoordinateConversion]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.CoordinateConversion]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:38]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[OccupancyGridTest.DeterministicRandomObstacles]=]  /Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build/test_grid [==[--gtest_filter=OccupancyGridTest.DeterministicRandomObstacles]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[OccupancyGridTest.DeterministicRandomObstacles]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/tests/test_grid.cpp:55]==]
    WORKING_DIRECTORY [==[/Users/angelinjacobs/Desktop/Vedant/Projects/Atlass/build]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(test_grid_TESTS [==[OccupancyGridTest.Initialization]==] [==[OccupancyGridTest.InvalidInitialization]==] [==[OccupancyGridTest.BoundsChecking]==] [==[OccupancyGridTest.OccupancyReadWrite]==] [==[OccupancyGridTest.CoordinateConversion]==] [==[OccupancyGridTest.DeterministicRandomObstacles]==])
