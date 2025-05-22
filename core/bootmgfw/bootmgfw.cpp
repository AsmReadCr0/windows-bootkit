#include "bootmgfw.hpp"
#include "..\hook\hook.hpp"
#include "..\patch\patch.hpp"

#include "dependencies/pe/pe.hpp"
#include "dependencies\utilities\utilities.hpp"

INLINE_HOOK fw_hook;

EFI_GUID gEfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

#define START_BOOT_APPLICATION_SIG "\x48\x8B\xC4\x48\x89\x58\x20\x44\x89\x40\x18\x48\x89\x50\x10\x48\x89\x48\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\xA9" // \x48\x8B\xC4\x48\x89\x58\x20\x44\x89\x40\x18\x48\x89\x50\x10\x48\x89\x48\x08\x55\x56\x57\x41\x54
#define START_BOOT_APPLICATION_MASK "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

EFI_DEVICE_PATH* EFIAPI bootmgfw::get_path() {
	UINTN handleCount;
	EFI_HANDLE* handles;
	EFI_DEVICE_PATH* devicePath = 0;

	static CHAR16* WINDOWS_BOOTMGFW_PATH = (CHAR16*)u"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";

	EFI_STATUS status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &handleCount, &handles);

	if (EFI_ERROR(status)) {
		printf(" [-] failed to get filesystem handles.");
		return devicePath;
	}

	for (UINTN i = 0; i < handleCount && !devicePath; ++i) {
		EFI_FILE_IO_INTERFACE* fileSystem;
		status = gBS->OpenProtocol(
			handles[i], &gEfiSimpleFileSystemProtocolGuid, (VOID**)&fileSystem,
			gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);

		if (EFI_ERROR(status)) {
			continue;
		}

		EFI_FILE_HANDLE volume;
		status = fileSystem->OpenVolume(fileSystem, &volume);
		if (!EFI_ERROR(status)) {
			EFI_FILE_HANDLE file;
			status = volume->Open(volume, &file, WINDOWS_BOOTMGFW_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

			if (!EFI_ERROR(status)) {
				volume->Close(file);

				devicePath = FileDevicePath(handles[i], WINDOWS_BOOTMGFW_PATH);
			}
		}

		gBS->CloseProtocol(handles[i], &gEfiSimpleFileSystemProtocolGuid,
			gImageHandle, NULL);
	}

	gBS->FreePool(handles);
	return devicePath;
}

EFI_STATUS EFIAPI bootmgfw::install_hooks(EFI_HANDLE ImageHandle)
{
	printf("[~] loading...\n");

	EFI_STATUS result = EFI_SUCCESS;
	EFI_LOADED_IMAGE* bootmgfw = 0;

	EFI_STATUS status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&bootmgfw);

	if (EFI_ERROR(status)) {
		printf("[-] failed to load boot firmware manager...\n");
		printf("[-] rebooting...\n");
		gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, nullptr);
	}

	VOID* start_boot_application = pe::find_pattern_ex((CHAR8*)bootmgfw->ImageBase, bootmgfw->ImageSize,
		(CHAR8*)START_BOOT_APPLICATION_SIG,
		(CHAR8*)START_BOOT_APPLICATION_MASK
	);

	hook::inline_hook(&fw_hook, start_boot_application, &patch::arch_start_boot_application_hook, TRUE);

	return EFI_SUCCESS;
}