#pragma once

#include <windows.h>
#include <setupapi.h>

#define LIBSSD_ERROR_SIZE 1024
#define LIBSSD_DEVICE_NAME 512
#define LIBSSD_DEVICE_ID 1024
#define LIBSSD_DESCRIPTION 128

struct ssd_device {
    CHAR device_name[LIBSSD_DEVICE_NAME];
    CHAR hardware_id[LIBSSD_DEVICE_ID];
    DWORD vendor_id;
    DWORD product_id;
};

struct storage_device {
    CHAR vendor_id[LIBSSD_DESCRIPTION];
    CHAR product_id[LIBSSD_DESCRIPTION];
    CHAR firmware_id[LIBSSD_DESCRIPTION];
    CHAR serial_id[LIBSSD_DESCRIPTION];
    DWORD bus_type;
};

typedef void(*ssd_device_handler)(CHAR[LIBSSD_ERROR_SIZE], struct storage_device*);

extern HANDLE libssd_get_device(CHAR error[LIBSSD_ERROR_SIZE], DWORD number);

extern void libssd_get_storage_device_info(CHAR error[LIBSSD_ERROR_SIZE], HANDLE device, struct storage_device* info);
extern void libssd_get_storage_device_memory_info(HANDLE device, struct storage_device* info);

extern void libssd_enumerate_devices(ssd_device_handler handler);

extern void libssd_free_devices_list(HDEVINFO devices_list);