#pragma once
#include "..\core\uefi_include.hpp"
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>

#pragma comment( linker, "/merge:.CRT=.rdata" )
#pragma comment( linker, "/merge:.pdata=.rdata" )
#pragma comment( linker, "/merge:.rdata=.text" )

#define EXTERN_C extern "C"

namespace std
{
    typedef unsigned long long uint64_t;
    typedef unsigned int       uint32_t;
    typedef unsigned short     uint16_t;
    typedef unsigned char      uint8_t;

    typedef signed long long int64_t;
    typedef signed int       int32_t;
    typedef signed short     int16_t;
    typedef signed char      int8_t;
}

#define bit32_max 0xffffffff
#define bit64_max 0xffffffffffffffff
#define scan_max 0x5f5e100
#define trampoline_size 14
#define containing_record(address, type, field) ((type *)((UINT8 *)(address) - (UINTN)(&((type *)0)->field)))
#define align_page(Va) ((VOID*)((QWORD)(Va) & ~(0x1000 - 1)))

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;	// 16
    LIST_ENTRY InMemoryOrderLinks;	// 32
    LIST_ENTRY InInitializationOrderLinks; // 48
    UINT64 ModuleBase; // 56
    UINT64 EntryPoint; // 64
    UINTN SizeOfImage; // 72
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY, ** PPLDR_DATA_TABLE_ENTRY;

typedef struct _LOADER_PARAMETER_BLOCK {
    UINT32 OsMajorVersion;
    UINT32 OsMinorVersion;
    UINT32 Size;
    UINT32 OsLoaderSecurityVersion;
    LIST_ENTRY LoadOrderListHead;
} LOADER_PARAMETER_BLOCK;

typedef struct _UNICODE_STRING {
    UINT16 Length;
    UINT16 MaximumLength;
    CHAR16* Buffer;
} UNICODE_STRING;

typedef struct _KLDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    VOID* ExceptionTable;
    UINT32 ExceptionTableSize;
    VOID* GpValue;
    VOID* NonPagedDebugInfo;
    VOID* ImageBase;
    VOID* EntryPoint;
    UINT32 SizeOfImage;
    UNICODE_STRING FullImageName;
    UNICODE_STRING BaseImageName;
} KLDR_DATA_TABLE_ENTRY;

struct IMAGE_DOS_HEADER
{
    std::uint16_t e_magic;    // Magic number
    std::uint16_t e_cblp;
    std::uint16_t e_cp;
    std::uint16_t e_crlc;
    std::uint16_t e_cparhdr;
    std::uint16_t e_minalloc;
    std::uint16_t e_maxalloc;
    std::uint16_t e_ss;
    std::uint16_t e_sp;
    std::uint16_t e_csum;
    std::uint16_t e_ip;
    std::uint16_t e_cs;
    std::uint16_t e_lfarlc;
    std::uint16_t e_ovno;
    std::uint16_t e_res[4];
    std::uint16_t e_oemid;
    std::uint16_t e_oeminfo;
    std::uint16_t e_res2[10];
    std::int32_t  e_lfanew;    // Offset to PE header
};

struct IMAGE_NT_HEADERS
{
    std::uint32_t Signature; // "PE\0\0"
};

typedef EFI_STATUS(EFIAPI* IMG_ARCH_START_BOOT_APPLICATION)(VOID*, VOID*, UINT32, UINT8, VOID*);
typedef EFI_STATUS(EFIAPI* OSL_FWP_KERNEL_SETUP_PHASE_1)(LOADER_PARAMETER_BLOCK*);


typedef std::uint64_t HANDLE;
typedef std::uint64_t* PHANDLE;
typedef std::uint32_t ACCESS_MASK;
typedef void* PVOID;
typedef std::uint32_t ULONG;

typedef std::uint64_t(EFIAPI* NT_CREATE_FILE)(
    PHANDLE,             // FileHandle
    ACCESS_MASK,         // DesiredAccess
    PVOID,              // ObjectAttributes
    PVOID,              // IoStatusBlock
    PVOID,              // AllocationSize
    ULONG,              // FileAttributes
    ULONG,              // ShareAccess
    ULONG,              // CreateDisposition
    ULONG,              // CreateOptions
    PVOID,              // EaBuffer
    ULONG               // EaLength
    );
