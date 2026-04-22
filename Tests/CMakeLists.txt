include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.11.0"
)

FetchContent_MakeAvailable(Catch2)

add_executable(
    CloneableTests
    "Src/CloneableTests.cpp"
)

target_link_libraries(CloneableTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(CloneableTests PRIVATE Cloneable)