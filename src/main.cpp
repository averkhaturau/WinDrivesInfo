#include "DriveInfo.h"
#include "EnumDrives.h"


#include <sstream>

const char* const driveTypeStr[] = {
    "DRIVE_UNKNOWN",
    "DRIVE_NO_ROOT_DIR",
    "DRIVE_REMOVABLE",
    "DRIVE_FIXED",
    "DRIVE_REMOTE",
    "DRIVE_CDROM",
    "DRIVE_RAMDISK"
};

std::string to_string(DriveInfo::DiscSpaceInfo const& dsi)
{
    std::stringstream result;
    result <<
           dsi.freeBytes << "(" << dsi.totalFreeBytes << ") / " << dsi.totalBytes << " bytes free;\n" <<
           dsi.freeClusters << " / " << dsi.totalClusters << " clusters free; " <<
           dsi.bytesPerSect << " bytes/sect; " <<
           dsi.sectPerClust << " sect/cluster\n";
    return result.str();
}

std::string to_string(DriveInfo::VolumeInfo const& vi)
{
    std::stringstream result;
    result <<
           vi.volName << "\t" <<
           "S/N:" << vi.volumeSerialNumber << "\t" <<
           vi.maximumComponentLength << "\t" <<
           "fs: " << vi.fileSysName << "\t" <<
           vi.fileSystemFlags;

    return result.str();
}

int main()
{
    enumDrives([](const char* drive) {
        std::cout << "\n" << drive << "\t";
        auto driveInfo = DriveInfo(drive);
        std::cout <<
                  driveTypeStr[driveInfo.driveType()] << "\t" <<
                  driveInfo.dosDeviceName() << "\n" <<
                  to_string(driveInfo.discSpaceInfo()) <<
                  to_string(driveInfo.volumeInfo()) << "\n";
    });
    return 0;
}