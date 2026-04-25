cmake_minimum_required(VERSION 3.10)
project(FlightReservationSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 收集所有源文件
set(SOURCES
    ${CMAKE_SOURCE_DIR}/sources/main.cpp
    ${CMAKE_SOURCE_DIR}/sources/solver.cpp
    ${CMAKE_SOURCE_DIR}/sources/airline.cpp
)

# 生成可执行文件
add_executable(main ${SOURCES})

# 生成测试数据生成器
add_executable(generator tests/generator.cpp)

# 设置输出目录
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# 设置生成器输出目录
set_target_properties(generator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)