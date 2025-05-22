#pragma once
#include "..\uefi_include.hpp"
#include "dependencies/std.h"

typedef struct _INLINE_HOOK
{
	unsigned char code[14];
	unsigned char jmp[14];

	void* address;
	void* hook_address;
} INLINE_HOOK, * PINLINE_HOOK_T;

namespace hook {
	VOID inline_hook(PINLINE_HOOK_T hook, VOID* from, VOID* to, BOOLEAN install);
	VOID enable_inline_hook(PINLINE_HOOK_T hook);
	VOID disable_inline_hook(PINLINE_HOOK_T hook);
} 
