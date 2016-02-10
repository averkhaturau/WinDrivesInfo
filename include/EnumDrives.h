#pragma once

#include <functional>


unsigned char log2(unsigned long v)
{
    unsigned long logResult = 0;
#if defined(__GNUC__)
    asm("\tbsr %1, %0\n"
        : "=r"(logResult)
        : "r" (v)
       );
#elif defined(_MSC_VER)
    _BitScanReverse(&logResult, v);
#endif
    return static_cast<unsigned char>(logResult);
}

void enumDrives(std::function<void(const char*)> fn)
{
    char szDrive[] = "A:\\";
    DWORD uDriveMask = GetLogicalDrives();
    while (uDriveMask) {
        DWORD uDriveMask_1 = uDriveMask - 1;
        DWORD two_pow_letter = uDriveMask_1 ^ uDriveMask;
        unsigned char letterInc = log2(two_pow_letter);
        szDrive[0] = 'A' + letterInc;
        fn(szDrive);
        uDriveMask &= uDriveMask_1;
    }
}
