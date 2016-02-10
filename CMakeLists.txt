cmake_minimum_required(VERSION 3.1)

project(WinDrivesInfo)

set(CMAKE_INSTALL_PREFIX bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(include)

add_subdirectory(src)
