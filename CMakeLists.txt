cmake_minimum_required(VERSION 3.1)

project(WinDrivesInfo)

set(CMAKE_INSTALL_PREFIX bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(
	-DNOMINMAX # do not define min() and max()
	-D_CRT_SECURE_NO_WARNINGS 
	-D_CRT_NONSTDC_NO_WARNINGS 
	-D__STDC_CONSTANT_MACROS
	-DWIN32_LEAN_AND_MEAN # remove obsolete things from windows headers
	-DUNICODE -D_UNICODE
)


include_directories(include)

add_subdirectory(src)
