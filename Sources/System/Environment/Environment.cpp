#include <System/Environment/Environment.hpp>
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
#include <windows.h> 
#include <cstdint>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <limits.h>
#endif 
namespace System {

    int Environment::CurrentProcessId() {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        uint32_t processID = GetCurrentProcessId();
        return processID;
#else
        pid_t processID = getpid();
        return processID;
#endif	
    }

    int Environment::ProcessorCount() {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
#else
        return sysconf(_SC_NPROCESSORS_ONLN);
#endif			
    }
    std::string Environment::ProcessorBrand() {
#ifndef SYSTEM4CPPARM_LINUX_GCC		
        uint32_t regs[4] = {};
        std::string brand;
        // Retrieve the brand information in three parts
        for (int i = 0; i < 3; ++i) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
            __cpuid((int*)regs, 0x80000002 + i);  // Start from 0x80000002
#elif (defined(__GNUC__) || defined(__clang__)) && !defined(__aarch64__)
            __get_cpuid(0x80000002 + i, &regs[0], &regs[1], &regs[2], &regs[3]);
#else
#endif
            // Append the brand parts
            brand += std::string((const char*)&regs[0], 4);
            brand += std::string((const char*)&regs[1], 4);
            brand += std::string((const char*)&regs[2], 4);
            brand += std::string((const char*)&regs[3], 4);
        }

        return brand;
#endif	
    }
    std::string Environment::ProcessorVendor() {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        const int _ebx = 1;
        const int _ecx = 2;
        const int _edx = 3;
        uint32_t regs[4] = {};
        __cpuid((int*)regs, (int)0);
        std::string vendor;
        vendor += std::string((const char*)&regs[_ebx], 4);
        vendor += std::string((const char*)&regs[_edx], 4);
        vendor += std::string((const char*)&regs[_ecx], 4);
        return vendor;
#endif
    }
    bool Environment::ProcessorHasFeature(CPUFEATURE ProcessorFeature) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        const int _ecx = 2;
        const int _edx = 3;
        uint32_t regs[4] = {};
        __cpuid((int*)regs, (int)1);
        if (ProcessorFeature == CPUFEATURE::x64_3DNOW) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_3DNOWEXT) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_ABM) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_ADX) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AES) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX2) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX512CD) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX512ER) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX512F) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_AVX512PF) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_BMI1) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_BMI2) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_CLFSH) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_CMPXCHG16B) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_CX8) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_ERMS) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_F16C) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_FMA) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_FSGSBASE) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_FXSR) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_HLE) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_INVPCID) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_LAHF) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_LZCNT) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_MMX) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_MMXEXT) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_MONITOR) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_MOVBE) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_MSR) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_OSXSAVE) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_PCLMULQDQ) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_POPCNT) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_PREFETCHWT1) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_RDRAND) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_RDSEED) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_RDTSCP) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_RTM) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SEP) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SHA) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE2) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE3) {
            return (regs[_ecx] & (1 << 0)) != 0;// check
        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE4_1) {
            return (regs[_ecx] & (1 << 19)) != 0;// check
        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE4_2) {
            return (regs[_ecx] & (1 << 20)) != 0; // SSE4.2 is bit 20 in ECX
        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSE4a) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_SSSE3) {
            return (regs[_ecx] & (1 << 9)) != 0;//check
        }
        else if (ProcessorFeature == CPUFEATURE::x64_SYSCALL) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_TBM) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_XOP) {

        }
        else if (ProcessorFeature == CPUFEATURE::x64_XSAVE) {

        }
        else {
            printf("  SSE3 instructions: %d\n", regs[_ecx] & 1);

        }
        return 0;
#endif	
    }




    uint64_t Environment::TotalPhysicalMemory(MEMFORMAT memory_format) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC

        MEMORYSTATUSEX memInfo{};
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return static_cast<size_t>(memInfo.ullTotalPhys);
#else
        struct sysinfo info;
        if (sysinfo(&info) != 0) {
            perror("sysinfo");
            return 0; // Return 0 on error
        }
        return info.totalram * info.mem_unit;
#endif
    }
    uint64_t Environment::TotalVirtualMemory(MEMFORMAT MemoryFormat) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC

        MEMORYSTATUSEX memInfo{};
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return static_cast<size_t>(memInfo.ullTotalVirtual);
#else
        long pages = sysconf(_SC_PHYS_PAGES);
        long pageSize = sysconf(_SC_PAGE_SIZE);
        return pages * pageSize;
#endif
    }
    uint64_t Environment::AvailablePhysicalMemory(MEMFORMAT MemoryFormat) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        MEMORYSTATUSEX memInfo{};
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return static_cast<size_t>(memInfo.ullAvailPhys);
#else
        struct sysinfo info;
        if (sysinfo(&info) != 0) {
            return 0;
        }
        return info.freeram;
#endif	
    }
    uint64_t Environment::AvailableVirtualMemory(MEMFORMAT MemoryFormat) {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        MEMORYSTATUSEX memInfo{};
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return static_cast<size_t>(memInfo.ullAvailVirtual);
#else
        struct sysinfo si;
        if (sysinfo(&si) != 0) {
            perror("sysinfo");
            return 0; // Error occurred
        }
        size_t totalAvailable = si.freeram + si.bufferram + si.freeswap;
        return totalAvailable; // in bytes (usually includes just RAM)
#endif	
    }
    std::string Environment::CurrentDirectory() {
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
        const unsigned long maxDir = 260;
        char currentDir[maxDir];
        GetCurrentDirectoryA(maxDir, (currentDir));
        return std::string(currentDir);
#else
        const unsigned long maxDir = PATH_MAX;
        char currentDir[maxDir];
        if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
            return std::string("");
        }
        return std::string(currentDir);
#endif					
    }









}