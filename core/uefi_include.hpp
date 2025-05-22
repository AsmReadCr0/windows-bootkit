#pragma once

extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
}

enum efi_status {
    efi_success = EFI_SUCCESS,
    efi_load_error = EFI_LOAD_ERROR,
    efi_invalid_parameter = EFI_INVALID_PARAMETER,
    efi_unsupported = EFI_UNSUPPORTED,
    efi_bad_buffer_size = EFI_BAD_BUFFER_SIZE,
    efi_buffer_too_small = EFI_BUFFER_TOO_SMALL,
    efi_not_ready = EFI_NOT_READY,
    efi_device_error = EFI_DEVICE_ERROR,
    efi_write_protected = EFI_WRITE_PROTECTED,
    efi_out_of_resources = EFI_OUT_OF_RESOURCES,
    efi_volume_full = EFI_VOLUME_FULL,
    efi_no_media = EFI_NO_MEDIA,
    efi_media_changed = EFI_MEDIA_CHANGED,
    efi_not_found = EFI_NOT_FOUND,
    efi_access_denied = EFI_ACCESS_DENIED,
    efi_no_response = EFI_NO_RESPONSE,
    efi_no_mapping = EFI_NO_MAPPING,
    efi_timeout = EFI_TIMEOUT,
    efi_not_started = EFI_NOT_STARTED,
    efi_already_started = EFI_ALREADY_STARTED,
    efi_aborted = EFI_ABORTED,
    efi_icmp_error = EFI_ICMP_ERROR,
    efi_tftp_error = EFI_TFTP_ERROR,
    efi_protocol_error = EFI_PROTOCOL_ERROR,
    efi_incompatible_version = EFI_INCOMPATIBLE_VERSION,
    efi_security_violation = EFI_SECURITY_VIOLATION,
};

enum efi_color {
    black = 0x00,
    blue = 0x01,
    green = 0x02,
    cyan = (EFI_BLUE | EFI_GREEN),
    light_cyan = (EFI_BLUE | EFI_GREEN | EFI_BRIGHT),
    red = 0x04,
    light_red = (0x04 | EFI_BRIGHT),
    magenta = (EFI_BLUE | EFI_RED),
    gray = (EFI_BLUE | EFI_GREEN | EFI_RED),
    light_blue = (EFI_BLUE | EFI_BRIGHT),
    yellow = (EFI_BROWN | EFI_BRIGHT),
    white = (EFI_BLUE | EFI_GREEN | EFI_RED | EFI_BRIGHT)
};

enum winload_context
{
    ApplicationContext,
    FirmwareContext
};