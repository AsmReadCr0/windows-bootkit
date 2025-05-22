#pragma once
#include "core/uefi_include.hpp"
#include "dependencies/std.h"

namespace patch
{
	EFI_STATUS EFIAPI osl_fwp_kernel_setup_phase1_hook(LOADER_PARAMETER_BLOCK* loaderParameterBlock);
	EFI_STATUS EFIAPI arch_start_boot_application_hook(VOID* entry, VOID* image_base, UINT32 image_size, UINT8 boot_option, VOID* return_args);

	typedef std::uint64_t (EFIAPI* NT_ADD_ATOM)(
		VOID* atomname,
		VOID* atom
	);
}