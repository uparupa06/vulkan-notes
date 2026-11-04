cmake_minimum_required(VERSION 3.28)

project(vulkan_tutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(vulkan_tutorial
    src/main.cpp
)

target_compile_definitions(vulkan_tutorial PRIVATE
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

target_link_libraries(vulkan_tutorial PRIVATE
    Vulkan::Vulkan
    glfw
)
