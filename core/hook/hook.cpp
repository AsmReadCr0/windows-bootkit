#include "hook.hpp"
#include "dependencies/utilities/utilities.hpp"

VOID hook::inline_hook(PINLINE_HOOK_T hook, VOID* from, VOID* to, BOOLEAN install)
{
	unsigned char jmp_instruction[14] =
	{
		0xff, 0x25, 0x0, 0x0, 0x0, 0x0,		// jmp    QWORD PTR[rip + 0x0]

		0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0
	};

	hook->address = from;
	hook->hook_address = to;
	utils::copy_memory(hook->code, from, sizeof hook->code);

	utils::copy_memory(jmp_instruction + 6, &to, sizeof to);
	utils::copy_memory(hook->jmp, jmp_instruction, sizeof jmp_instruction);

	if (install)
	{
		enable_inline_hook(hook);
	}
}

VOID hook::enable_inline_hook(PINLINE_HOOK_T hook)
{
	utils::copy_memory(hook->address, hook->jmp, sizeof hook->jmp);
}

VOID hook::disable_inline_hook(PINLINE_HOOK_T hook)
{
	utils::copy_memory(hook->address, hook->code, sizeof hook->code);
}

