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
    std::stringstream result; result <<
                                     dsi.freeBytes << "(" << dsi.totalFreeBytes << ") / " << dsi.totalBytes << " bytes free;\n" <<
                                     dsi.freeClusters << " / " << dsi.totalClusters << " clusters free; " <<
                                     dsi.bytesPerSect << "bytes/sect; " <<
                                     dsi.sectPerClust << "sect/cluster\n";
    return result.str();
}

int main()
{
    enumDrives([](const char* drive) {
        auto driveInfo = DriveInfo(drive);
        std::cout << drive << "\t" << driveTypeStr[driveInfo.driveType()] << "\t" << driveInfo.dosDeviceName() << "\n" << to_string(driveInfo.discSpaceInfo());
    });
    return 0;
}