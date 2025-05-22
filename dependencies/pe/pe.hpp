#pragma once
#include "core/uefi_include.hpp"
#include "dependencies/std.h"
#include "dependencies/utilities/utilities.hpp"

namespace pe
{
    std::uint64_t find_pattern(VOID* base_addr, UINT64 size, const CHAR8* pattern);

    VOID* find_pattern_ex(CHAR8* base, UINTN size, CHAR8* pattern, CHAR8* mask);

    BOOLEAN check_mask(CHAR8* base, CHAR8* pattern, CHAR8* mask);

    BOOLEAN validate_pe(std::uint64_t module_base);

    std::uint64_t get_section_header_address(std::uint64_t base_addr, const char* section);
    std::uint64_t get_section_address(std::uint64_t base_addr, const char* section);
    std::uint32_t get_section_size(std::uint64_t base_addr, const char* section);

    std::uint64_t get_module(std::uint64_t list_entry, const wchar_t* module);
    std::uint64_t get_module_base(std::uint64_t list_entry, const wchar_t* module);

    std::uint64_t scan_section(std::uint64_t base_addr, const char* section, std::uint8_t* pattern, std::uint64_t pattern_size);
    std::uint64_t scan_section_ex(std::uint64_t base_addr, const char* section, std::uint8_t* pattern, std::uint64_t pattern_size, std::uint8_t* mask);

    KLDR_DATA_TABLE_ENTRY* get_module_entry(LIST_ENTRY* list, CHAR16* name);

    std::uint64_t get_export_address(std::uint64_t base_address, const char* name);

    void disable_wp();
    void enable_wp();

}