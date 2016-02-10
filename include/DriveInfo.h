#pragma once

#include <string>
#include <assert.h>
#include <Windows.h>
#include <iostream>

class DriveInfo
{
public:
    explicit DriveInfo(const char* drivePath)
    {
        assert(strlen(drivePath) + 1 == _countof(this->drivePath));

        strcpy_s(this->drivePath, drivePath);
        initDosDevice();
        initFreeSpaceInfo();
        initDriveType();
    };

    std::string dosDeviceName()const {return dasDevice;}

    struct DiscSpaceInfo {
        unsigned long long freeBytes, totalBytes, totalFreeBytes;
        unsigned long sectPerClust, bytesPerSect, freeClusters, totalClusters;
    };
    DiscSpaceInfo const& discSpaceInfo()const {return dsi;}

    enum DriveType {
        dt_UNKNOWN,
        dt_NO_ROOT_DIR,
        dt_REMOVABLE,
        dt_FIXED,
        dt_REMOTE,
        dt_CDROM,
        dt_RAMDISK
    };
    DriveType driveType()const {return drive_t; }

private:
    char drivePath[3]; // e.g. "A:"

    DiscSpaceInfo dsi = {};

    std::string dasDevice;

    DriveType drive_t;

    void initDosDevice()
    {
        dasDevice.resize(100);
        DWORD charsWritten;
        while ((charsWritten = QueryDosDeviceA(drivePath, &*dasDevice.begin(), dasDevice.size())) == 0 && ERROR_INSUFFICIENT_BUFFER == GetLastError())
            dasDevice.resize(dasDevice.size() * 2);
        dasDevice.resize(charsWritten - 2);
    }

    void initFreeSpaceInfo()
    {
        BOOL res = GetDiskFreeSpaceExA(drivePath, (PULARGE_INTEGER)&dsi.freeBytes, (PULARGE_INTEGER)&dsi.totalBytes, (PULARGE_INTEGER)&dsi.totalFreeBytes);
        if (res == FALSE)
            std::cerr << "GetDiskFreeSpaceExA failed w/e " << GetLastError() << "\n";

        res = GetDiskFreeSpaceA(drivePath, &dsi.sectPerClust, &dsi.bytesPerSect, &dsi.freeClusters, &dsi.totalClusters);
        if (res == FALSE)
            std::cerr << "GetDiskFreeSpaceA failed w/e " << GetLastError() << "\n";
    }

    void initDriveType()
    {
        drive_t = static_cast<DriveType>(GetDriveTypeA(drivePath));
    }
};
