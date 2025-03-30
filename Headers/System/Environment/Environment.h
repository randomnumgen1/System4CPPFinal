#pragma once
#ifndef SYSTEM_ENVIROMENT_H
#define SYSTEM_ENVIROMENT_H
#if (defined(__x86_64__) || defined(_M_X64)) && defined(_WIN32)
#define SYSTEM4CPPX64_WINDOWS_MSVC
#endif
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
#include <windows.h> 
#include <cstdint>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <limits.h>
#endif 
#include <string>

namespace System {

    class Environment {
    private:
    public:
        enum CPUFEATURE : uint64_t {
#if defined(__arm__) || defined(_M_ARM)
            ARM_NEON,
#endif
#ifdef SYSTEM4CPPX64_WINDOWS_MSVC
            x64_3DNOW,
            x64_3DNOWEXT,
            x64_ABM,
            x64_ADX,
            x64_AES,
            x64_AVX,
            x64_AVX2,
            x64_AVX512CD,
            x64_AVX512ER,
            x64_AVX512F,
            x64_AVX512PF,
            x64_BMI1,
            x64_BMI2,
            x64_CLFSH,
            x64_CMPXCHG16B,
            x64_CX8,
            x64_ERMS,
            x64_F16C,
            x64_FMA,
            x64_FSGSBASE,
            x64_FXSR,
            x64_HLE,
            x64_INVPCID,
            x64_LAHF,
            x64_LZCNT,
            x64_MMX,
            x64_MMXEXT,
            x64_MONITOR,
            x64_MOVBE,
            x64_MSR,
            x64_OSXSAVE,
            x64_PCLMULQDQ,
            x64_POPCNT,
            x64_PREFETCHWT1,
            x64_RDRAND,
            x64_RDSEED,
            x64_RDTSCP,
            x64_RTM,
            x64_SEP,
            x64_SHA,
            x64_SSE,
            x64_SSE2,
            x64_SSE3,
            x64_SSE4_1,
            x64_SSE4_2,
            x64_SSE4a,
            x64_SSSE3,
            x64_SYSCALL,
            x64_TBM,
            x64_XOP,
            x64_XSAVE
#endif
        };
        enum MEMFORMAT {
            DEFAULT,
            GB,
            MB,
            KB
        };
        static int CurrentProcessId();
        static int ProcessorCount();
        static std::string ProcessorBrand();
        static std::string ProcessorVendor();
        static bool ProcessorHasFeature(CPUFEATURE ProcessorFeature);
        static uint64_t TotalPhysicalMemory(MEMFORMAT memory_format = MEMFORMAT::GB);
        static uint64_t TotalVirtualMemory(MEMFORMAT MemoryFormat);
        static uint64_t AvailablePhysicalMemory(MEMFORMAT MemoryFormat);
        static uint64_t AvailableVirtualMemory(MEMFORMAT MemoryFormat);
        static std::string CurrentDirectory();
    };
}
#endif