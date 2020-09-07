#include <stdio.h>

#include "../include/libpci.h"

struct pci_devices_list* lspci_get_devices_list(char error[LSPCI_ERROR_SIZE]) {
    UINT device_number;
    GetRawInputDeviceList(NULL, &device_number, sizeof(RAWINPUTDEVICELIST));
    if (device_number == 0) {
        sprintf(error, TEXT("Cannot find any device"));
        return NULL;
    }

    PRAWINPUTDEVICELIST devices_list = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * device_number);
    if (devices_list == NULL) {
        sprintf(error, TEXT("Cannot to alloc memory"));
        return NULL;
    }

    if (GetRawInputDeviceList(devices_list, &device_number, sizeof(RAWINPUTDEVICELIST)) < 0) {
        free(devices_list);
        sprintf(error, TEXT("Cannot fill devices list"));
        return NULL;
    }

    struct pci_devices_list* list = (struct pci_devices_list*)malloc(sizeof(struct pci_devices_list));
    list->device_list = devices_list;
    list->device_count = device_number;
    return list;
}

void lspci_free_devices_list(struct pci_devices_list* list) {
    free(list->device_list);
    free(list);
}

void lspci_enumerate_devices(struct pci_devices_list* list, pci_device_handler handler) {
    for (int i = 0; i < list->device_count; i++) {
        struct pci_device device;
        UINT number;

        if (GetRawInputDeviceInfo(list->device_list[i].hDevice, RIDI_DEVICENAME, device.device_name, &number) < 0) {
            char error[LSPCI_ERROR_SIZE];
            sprintf(error, TEXT("Cannot get device name"));
            handler(error, NULL);
            continue;
        }

        RID_DEVICE_INFO device_info;
        device_info.cbSize = sizeof(RID_DEVICE_INFO);
        UINT size = device_info.cbSize;

        if (GetRawInputDeviceInfo(list->device_list[i].hDevice, RIDI_DEVICEINFO, &device_info, &size) < 0) {
            char error[LSPCI_ERROR_SIZE];
            sprintf(error, TEXT("Cannot get device info"));
            handler(error, NULL);
            continue;
        }

        device.product_id = device_info.hid.dwProductId;
        device.vendor_id = device_info.hid.dwVendorId;
        handler(NULL, &device);
    }
}