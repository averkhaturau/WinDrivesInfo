#pragma once

#include <string>
#include <assert.h>
#include <Windows.h>
#include <iostream>
#include <algorithm>

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
        initVolumeInfo();

        initDriveHandle();
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

    struct VolumeInfo {
        std::string volName;
        DWORD volumeSerialNumber;
        DWORD maximumComponentLength;
        DWORD fileSystemFlags;
        std::string fileSysName;
    };
    VolumeInfo const& volumeInfo()const {return vi;}

private:
    char drivePath[4]; // e.g. "A:\\"

    HANDLE driveHandle = INVALID_HANDLE_VALUE;

    DiscSpaceInfo dsi = {};

    std::string dasDevice;

    DriveType drive_t;

    VolumeInfo vi;

    void initDosDevice()
    {
        dasDevice.resize(100);
        long long charsWritten;
        char dosDrive[3] = {drivePath[0], ':'};
        while ((charsWritten = QueryDosDeviceA(dosDrive, &*dasDevice.begin(), dasDevice.size())) == 0 && ERROR_INSUFFICIENT_BUFFER == GetLastError())
            dasDevice.resize(dasDevice.size() * 2);
        if (charsWritten > 2)
            dasDevice.resize(static_cast<size_t>(charsWritten - 2));
        else {
            std::cerr << "QueryDosDeviceA failed w/e " << GetLastError() << "\n";
            dasDevice.resize(0);
        }

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

    void initVolumeInfo()
    {
        vi.volName.resize(MAX_PATH + 1, '\0');
        vi.fileSysName.resize(MAX_PATH + 1, '\0');
        BOOL res = GetVolumeInformationA(
                       drivePath,
                       &*vi.volName.begin(),
                       vi.volName.size(),
                       &vi.volumeSerialNumber,
                       &vi.maximumComponentLength,
                       &vi.fileSystemFlags,
                       &*vi.fileSysName.begin(),
                       vi.fileSysName.size()
                   );
        if (res == FALSE)
            std::cerr << "GetVolumeInformationA failed w/e " << GetLastError() << "\n";
        vi.volName.resize(vi.volName.find('\0'));
        vi.fileSysName.resize(vi.fileSysName.find('\0'));
    }

    void initDriveHandle()
    {
        const char uncDrive[7] = {'\\', '\\', '.', '\\', drivePath[0], ':'};
        driveHandle = CreateFileA(uncDrive,
                                  GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
        if (driveHandle == INVALID_HANDLE_VALUE)
            std::cerr << "CreateFile failed w/e " << GetLastError() << "\n";
    }
};
