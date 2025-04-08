cmake_minimum_required(VERSION 3.16.0)
project(telemetry_server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost required components
find_package(Boost REQUIRED COMPONENTS system thread)

# Include directories
include_directories(
    include
    third_party           # for nlohmann/json
)

# Source files
file(GLOB SOURCES
    src/*.cpp
    main.cpp
)
# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Boost
target_link_libraries(${PROJECT_NAME}
    Boost::system
    Boost::thread
)