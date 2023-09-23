include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0)
# For Windows: Prevent overriding the parent project's compiler/linker
# settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(
  sqids_tests
  alphabetTests.cpp blocklistTests.cpp encodingTests.cpp minLengthTests.cpp)

target_link_libraries(sqids_tests PRIVATE GTest::GTest sqids)

target_include_directories(sqids_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(sqids_tests sqids_tests)
