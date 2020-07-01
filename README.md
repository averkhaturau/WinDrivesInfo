# WinDrivesInfo

Header-only very small C++ library, providing info of Windows disk drives.

It's a C++ abstraction on Windows functions GetLogicalDrives, GetDiskFreeSpace[Ex], QueryDosDevice, and GetDriveType.

Usage example 1:
  #include "EnumDrives.h"
  ...
  enumDrives([](const char* drive) {std::cout << drive << "\n;});
  
Usage example 2:
  #include "DriveInfo.h"
  ...
  const auto driveInfo = DriveInfo("C:");
  std::cout << driveInfo.dosDeviceName()
            << " has "
            << driveInfo.discSpaceInfo().totalFreeBytes
            << " free of "
            << driveInfo.discSpaceInfo().totalBytes
            << "\n";
