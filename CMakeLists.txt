cmake_minimum_required(VERSION 4.0)
project(learn_asio)

set(CMAKE_CXX_STANDARD 20)

find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(boost_bind CONFIG REQUIRED)
find_package(boost_thread CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::thread)
