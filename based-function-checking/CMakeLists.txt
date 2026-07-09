cmake_minimum_required(VERSION 3.10)
project(FunctionTest)

set(Catch2_DIR "D:/catch2/Catch2-devel/lib/cmake/Catch2")

find_package(Catch2 REQUIRED)

include_directories(D:/catch2/Catch2-devel/src)

add_executable(${PROJECT_NAME} based_function_checking.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)