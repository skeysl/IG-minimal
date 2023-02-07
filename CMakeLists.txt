cmake_minimum_required (VERSION 3.20)

project (IG-minimal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
add_definitions(-DNOMINMAX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory (igx)
add_subdirectory (igui)
add_subdirectory (vksync)





