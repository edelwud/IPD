#include <stdio.h>
#include <stdbool.h>

#include "../include/libssd.h"

#define LIBSSD_DEVICE_PATH_SIZE 256

HANDLE libssd_get_device(CHAR error[LIBSSD_ERROR_SIZE], DWORD number) {
    CHAR device_path[LIBSSD_DEVICE_PATH_SIZE];
    sprintf(device_path, TEXT("\\\\.\\PhysicalDrive%u"), number);

    HANDLE result = CreateFile(device_path, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (result == INVALID_HANDLE_VALUE) {
        sprintf(error, "Cannot open device");
        return NULL;
    }

    return result;
}

void libssd_free_device(HANDLE device) {
    CloseHandle(device);
}

void libssd_enumerate_devices(ssd_device_handler handler) {
    CHAR error[LIBSSD_ERROR_SIZE];
    BYTE description[LIBSSD_DEVICE_NAME];
    BYTE hardware_id[LIBSSD_DEVICE_ID];

    for (int i = 0;; i++) {
        memset(error, 0, LIBSSD_ERROR_SIZE);
        memset(description, 0, LIBSSD_DEVICE_NAME);
        memset(hardware_id, 0, LIBSSD_DEVICE_ID);

        HANDLE device = libssd_get_device(error, i);
        if (strlen(error) != 0) {
            handler(error, NULL);
            return;
        }
        struct storage_device info;
        memset(&info, 0, sizeof(struct storage_device));

        libssd_get_storage_device_info(error, device, &info);
        if (strlen(error) != 0) {
            handler(error, NULL);
        }
        libssd_free_device(device);

        handler(NULL, &info);
    }
}

void libssd_get_storage_device_info(CHAR error[LIBSSD_ERROR_SIZE], HANDLE device, struct storage_device* info) {
    STORAGE_PROPERTY_QUERY storage_property_query;
    STORAGE_DESCRIPTOR_HEADER storage_descriptor_header;
    DWORD written;
    if (DeviceIoControl(
            device,
            IOCTL_STORAGE_QUERY_PROPERTY,
            &storage_property_query,
            sizeof(STORAGE_PROPERTY_QUERY),
            &storage_descriptor_header,
            sizeof(STORAGE_PROPERTY_QUERY),
            &written,
            NULL)) {
        sprintf(error, TEXT("Cannot open IO control device"));
        return;
    }

    DWORD out_buffer_size = storage_descriptor_header.Size;
    BYTE* out_buffer = (BYTE*)malloc(out_buffer_size * sizeof(BYTE));
    ZeroMemory(out_buffer, out_buffer_size);

    if (DeviceIoControl(
            device,
            IOCTL_STORAGE_QUERY_PROPERTY,
            &storage_property_query,
            sizeof(STORAGE_PROPERTY_QUERY),
            out_buffer,
            out_buffer_size,
            &written,
            NULL)) {
        sprintf(error, TEXT("Cannot open IO control device"));
        return;
    }

    STORAGE_DEVICE_DESCRIPTOR* device_descriptor = (STORAGE_DEVICE_DESCRIPTOR*)out_buffer;
    device_descriptor->Size = out_buffer_size;

    if (device_descriptor->VendorIdOffset != 0) {
        strcpy(info->vendor_id, (char*)(out_buffer)+device_descriptor->VendorIdOffset);
    }

    if (device_descriptor->ProductIdOffset != 0) {
        strcpy(info->product_id, (char*)(out_buffer)+device_descriptor->ProductIdOffset);
    }

    if (device_descriptor->ProductRevisionOffset != 0) {
        strcpy(info->firmware_id, (char*)(out_buffer)+device_descriptor->ProductRevisionOffset);
    }

    if (device_descriptor->SerialNumberOffset != 0) {
        strcpy(info->serial_id, (char*)(out_buffer)+device_descriptor->SerialNumberOffset);
    }

    if (device_descriptor->BusType != 0) {
        info->bus_type = device_descriptor->BusType;
    }

    free(out_buffer);
}
