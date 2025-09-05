cmake_minimum_required(VERSION 4.0)
project(learn_asio)

set(CMAKE_CXX_STANDARD 20)

find_package(asio CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
