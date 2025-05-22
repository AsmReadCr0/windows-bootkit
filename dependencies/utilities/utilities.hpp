#pragma once
#include "..\std.h"
#include <core/patch/patch.hpp>

namespace utils
{
    void copy_memory(void* destination, void* source, UINTN size);
    std::uint64_t compare_memory(std::uint8_t* source1, std::uint8_t* source2, std::uint64_t size);
}

#define sec_to_ms( seconds ) seconds * 1000000

#define section_rwx (EFI_IMAGE_SCN_MEM_READ | EFI_IMAGE_SCN_MEM_WRITE | EFI_IMAGE_SCN_MEM_EXECUTE)
#define section_rw (EFI_IMAGE_SCN_MEM_READ | EFI_IMAGE_SCN_MEM_WRITE)

#define printf(text, ...) \
    do \
    { \
        gST->ConOut->OutputString(gST->ConOut, (CHAR16*)text L"\r\n"); \
    } while (0)

#define rva(result, rip_offset, rva_offset) \
	( *( int32_t* )( ( ( uint64_t )result ) + rva_offset ) ) + ( ( uint64_t )result ) + rip_offset

#define relative_address(addr, size) ((VOID *)((UINT8 *)(addr) + *(INT32 *)((UINT8 *)(addr) + ((size) - (INT32)sizeof(INT32))) + (size)))

#define sleep(ms) gBS->Stall(ms)

#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x&(~0x20)),'A','F')?((x&(~0x20))-'A'+0xA):(in_range(x,'0','9')?x-'0':0))
#define get_byte(a, b) (get_bits(a) << 4 | get_bits(b))

#define infinity_loop() \
    while (TRUE) \
    { \
    }

