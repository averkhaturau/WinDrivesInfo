project(WinDrivesInfo)

set(SRC
	../include/DriveInfo.h
	../include/EnumDrives.h

	main.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
