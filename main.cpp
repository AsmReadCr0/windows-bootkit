#include "core/uefi_include.hpp"
#include "dependencies/utilities/utilities.hpp"
#include "core/bootmgfw/bootmgfw.hpp"
#include "core/patch/patch.hpp"

extern "C" EFI_STATUS EFIAPI UefiMain( EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table )
{
    EFI_HANDLE bootmgfw_handle;
    EFI_DEVICE_PATH* bootmgfw_path;
    EFI_STATUS status;

    // clearing screen

    gST->ConOut->ClearScreen(gST->ConOut);
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

    // get boot firmware manager path

    printf("[*] getting boot firmware manager path...\n");

    bootmgfw_path = bootmgfw::get_path( );
    if ( !bootmgfw_path ) {
        sleep(sec_to_ms(2));
        return EFI_NOT_FOUND;
    }

    // loading original bootmgfw.efi

    printf("[*] loading boot firmware manager...\n");

    status = gBS->LoadImage(TRUE, image_handle, bootmgfw_path, 0, 0, &bootmgfw_handle);
    if ( EFI_ERROR ( status ) ) {
        sleep(sec_to_ms(2));
        return status;
    }

    status = bootmgfw::install_hooks(bootmgfw_handle);
    if ( EFI_ERROR ( status ) ) {
        sleep(sec_to_ms(2));
        printf("[*] failed to install boot firmware manager hooks...\n");
        return status;
    }

    printf("[~] starting boot firmware manager...\n");

    sleep(sec_to_ms(3));

    // starting bootmgfw.efi

    status = gBS->StartImage( bootmgfw_handle, 0, 0 );
    if (EFI_ERROR(status)) {
        sleep(sec_to_ms(2));
        return status;
    }
    
    return efi_success;
}
extern "C" EFI_STATUS EFIAPI UefiUnload(EFI_HANDLE handle) { return efi_success; }