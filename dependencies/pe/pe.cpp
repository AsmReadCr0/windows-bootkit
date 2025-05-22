#include "pe.hpp"
#include <IndustryStandard/PeImage.h>
#include "dependencies/crt/crt.hpp"

std::uint64_t pe::find_pattern(VOID* base_addr, UINT64 size, const CHAR8* pattern)
{
	UINT8* firstMatch = nullptr;
	const CHAR8* currentPattern = pattern;

	UINT8* start = static_cast<UINT8*>(base_addr);
	UINT8* end = start + size;

	for (UINT8* current = start; current < end; current++)
	{
		UINT8 byte = currentPattern[0]; if (!byte) return reinterpret_cast<UINT64>(firstMatch);
		if (byte == '\?' || *static_cast<UINT8*>(current) == get_byte(byte, currentPattern[1]))
		{
			if (!firstMatch) firstMatch = current;
			if (!currentPattern[2]) return reinterpret_cast<UINT64>(firstMatch);
			((byte == '\?') ? (currentPattern += 2) : (currentPattern += 3));
		}
		else
		{
			currentPattern = pattern;
			firstMatch = nullptr;
		}
	}

	return 0;
}

BOOLEAN pe::check_mask(CHAR8* base, CHAR8* pattern, CHAR8* mask)
{
	for (; *mask; ++base, ++pattern, ++mask)
		if (*mask == 'x' && *base != *pattern)
			return FALSE;

	return TRUE;
}

VOID* pe::find_pattern_ex(CHAR8* base, UINTN size, CHAR8* pattern, CHAR8* mask)
{
	size -= AsciiStrLen(mask);
	for (UINTN i = 0; i <= size; ++i)
	{
		auto* addr = &base[i];
		if (check_mask(addr, pattern, mask))
			return addr;
	}
	return NULL;
}

BOOLEAN pe::validate_pe(std::uint64_t module_base)
{
	if (module_base == 0)
		return FALSE;

	auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(module_base);
	if (dos_header->e_magic != 0x5A4D) // 'MZ'
		return FALSE;

	auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(module_base + dos_header->e_lfanew);
	if (nt_headers->Signature != 0x00004550) // 'PE\0\0'
		return FALSE;

	return true;

	return TRUE;
}

std::uint64_t pe::get_section_header_address(std::uint64_t base_addr, const char* section)
{
	std::uint16_t dos_magic = *(std::uint16_t*)(std::uint8_t*)(base_addr + 0x0 /* ->e_magic */);
	if (dos_magic != 0x5a4d)
	{
		return 0;
	}

	std::uint64_t pe_header = base_addr + *(std::uint32_t*)(std::uint8_t*)(base_addr + 0x3c /* ->e_elfanew */);

	std::uint16_t pe_signature = *(std::uint16_t*)(std::uint8_t*)(pe_header + 0x0 /* ->Signature */);
	if (pe_signature != 0x4550)
	{
		return 0;
	}

	std::uint16_t number_of_sections = *(std::uint16_t*)(std::uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x2 /* ->NumberOfSections */);
	std::uint16_t optional_header_size = *(std::uint16_t*)(std::uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x10 /* ->SizeOfOptionalHeader */);

	std::uint64_t first_section_header = pe_header + 0x18 /* ->OptionalHeader */ + optional_header_size;

	for (std::uint16_t i = 0; i < number_of_sections; ++i)
	{
		std::uint64_t current_section = first_section_header + (i * 0x28 /* sizeof(IMAGE_SECTION_HEADER) */);

		char section_name[9] = { 0 };
		utils::copy_memory((std::uint64_t*)(std::uint8_t*)(current_section + 0x0 /* ->Name */), (std::uint64_t*)&section_name, 8);

		if (utils::compare_memory((std::uint8_t*)&section_name, (std::uint8_t*)section, StrLen((CHAR16*)section)) == 0)
		{
			return current_section;
		}
	}

	return 0;
}

std::uint64_t pe::get_section_address(std::uint64_t base_addr, const char* section)
{
	return std::uint64_t();
}

std::uint32_t pe::get_section_size(std::uint64_t base_addr, const char* section)
{
	return std::uint32_t();
}

std::uint64_t pe::get_module(std::uint64_t list_entry, const wchar_t* module)
{
	std::uint64_t current_entry = *(std::uint64_t*)list_entry;

	while (current_entry != list_entry)
	{
		std::uint64_t module_name_addr = *(std::uint64_t*)((std::uint8_t*)current_entry + 0x58 /* ->BaseDllName */ + 0x8 /* ->Buffer */);

		if (module_name_addr)
		{
			if (crt::wcscmp((wchar_t*)module_name_addr, module) == 0)
			{
				return current_entry;
			}
		}

		current_entry = *(std::uint64_t*)current_entry;
	}

	return 0;
}

std::uint64_t pe::get_module_base(std::uint64_t list_entry, const wchar_t* module)
{
	std::uint64_t current_entry = get_module(list_entry, module);

	if (!current_entry)
	{
		return 0;
	}

	std::uint64_t module_base = *(std::uint64_t*)(std::uint8_t*)(current_entry + 0x30 /* ->DllBase */);

	return module_base;
}

std::uint64_t pe::scan_section(std::uint64_t base_addr, const char* section, std::uint8_t* pattern, std::uint64_t pattern_size)
{
	std::uint64_t section_address = pe::get_section_address(base_addr, section);
	std::uint32_t section_size = pe::get_section_size(base_addr, section);

	for (std::uint64_t i = 0; i < section_size; ++i)
	{
		std::uint64_t current_address = section_address + i;

		if (utils::compare_memory(pattern, (std::uint8_t*)current_address, pattern_size) == 0)
		{
			return current_address;
		}
	}

	return 0;
}

std::uint64_t pe::scan_section_ex(std::uint64_t base_addr, const char* section, std::uint8_t* pattern, std::uint64_t pattern_size, std::uint8_t* mask)
{
	std::uint64_t section_address = get_section_address(base_addr, section);
	std::uint32_t section_size = get_section_size(base_addr, section);

	for (std::uint64_t i = 0; i < section_size; ++i)
	{
		std::uint64_t current_address = section_address + i;

		bool found = true;

		for (std::uint64_t x = 0; x < pattern_size; x++)
		{
			std::uint8_t current_byte = *((std::uint8_t*)(current_address + x));

			if (mask[x] == 0xFF && current_byte != pattern[x])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return current_address;
		}
	}

	return 0;
}

KLDR_DATA_TABLE_ENTRY* pe::get_module_entry(LIST_ENTRY* list, CHAR16* name)
{
	for (LIST_ENTRY* entry = list->ForwardLink; entry != list;
		entry = entry->ForwardLink) {

		KLDR_DATA_TABLE_ENTRY* module =
			containing_record(entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (module && StrnCmp(name, module->BaseImageName.Buffer,
			module->BaseImageName.Length) == 0) {

			return module;
		}
	}

	return 0;
}

std::uint64_t pe::get_export_address(std::uint64_t base_address, const char* name)
{
	std::uint64_t pe_header = base_address + *(std::uint32_t*)(std::uint8_t*)(base_address + 0x3c /* ->e_elfanew */);

	std::uint64_t export_dir = base_address + *(std::uint32_t*)(std::uint8_t*)(pe_header + 0x18 /* ->OptionalHeader */ + 0x70 /* DataDirectory[0] */ + 0x0 /* ->VirtualAddress */);

	std::uint32_t name_list_count = *(std::uint32_t*)(std::uint8_t*)(export_dir + 0x18 /* ->NumberOfNames */);

	std::uint64_t address_list = base_address + *(std::uint32_t*)(std::uint8_t*)(export_dir + 0x1c /* ->AddressOfFunctions */);
	std::uint64_t name_list = base_address + *(std::uint32_t*)(std::uint8_t*)(export_dir + 0x20 /* ->AddressOfNames */);
	std::uint64_t name_ordinals_list = base_address + *(std::uint32_t*)(std::uint8_t*)(export_dir + 0x24 /* ->AddressOfNameOrdinals */);

	for (std::uint32_t i = 0; i < name_list_count; ++i)
	{
		std::uint64_t name_address_rva = name_list + (i * sizeof(std::uint32_t));
		std::uint64_t name_addr = base_address + *(std::uint32_t*)name_address_rva;
		char* name = (char*)(name_addr);

		if (crt::strcmp(name, name) == 0)
		{
			std::uint64_t ordinal_address = name_ordinals_list + (i * sizeof(std::uint16_t));
			std::uint16_t ordinal = *(std::uint16_t*)ordinal_address;

			std::uint64_t function_address_rva = address_list + (ordinal * sizeof(std::uint32_t));
			std::uint64_t function_address = base_address + *(std::uint32_t*)function_address_rva;

			return function_address;
		}
	}

	return 0;
}

void pe::disable_wp()
{
	UINTN cr0 = AsmReadCr0();
	cr0 &= ~(1UL << 16);
	AsmWriteCr0(cr0);
}

void pe::enable_wp()
{
	UINTN cr0 = AsmReadCr0();
	cr0 |= (1UL << 16);
	AsmWriteCr0(cr0);
}
