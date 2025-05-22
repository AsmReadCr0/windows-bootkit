#pragma once
#include "..\uefi_include.hpp"
#include "..\..\dependencies\std.h"
#include "..\hook\hook.hpp"

extern INLINE_HOOK fw_hook;

namespace bootmgfw
{
	EFI_DEVICE_PATH* EFIAPI get_path();
	EFI_STATUS EFIAPI install_hooks(EFI_HANDLE ImageHandle);
}