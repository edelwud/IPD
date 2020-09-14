#include <stdio.h>
#include <stdbool.h>

#include "../include/libpci.h"

bool libpci_parse_usb_hardware(char[LIBPCI_DEVICE_ID], struct pci_device*, char[LIBPCI_ERROR_SIZE]);

HDEVINFO libpci_get_devices_list(char error[LIBPCI_ERROR_SIZE]) {
    HDEVINFO device_list = SetupDiGetClassDevs(NULL, TEXT("PCI"), NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (device_list == INVALID_HANDLE_VALUE) {
        sprintf(error, TEXT("Cannot get pci devices descriptor"));
        return NULL;
    }
    return device_list;
}

void libpci_free_devices_list(HDEVINFO list) {
    SetupDiDestroyDeviceInfoList(list);
}

void libpci_enumerate_devices(HDEVINFO list, pci_device_handler handler) {
    char error[LIBPCI_ERROR_SIZE];
    SP_DEVINFO_DATA device_info;
    device_info.cbSize = sizeof(SP_DEVINFO_DATA);

    BYTE description[LIBPCI_DEVICE_NAME];
    BYTE hardware_id[LIBPCI_DEVICE_ID];

    for (int i = 0; SetupDiEnumDeviceInfo(list, i, &device_info); i++) {
        memset(description, 0, LIBPCI_DEVICE_NAME);
        memset(hardware_id, 0, LIBPCI_DEVICE_ID);

        struct pci_device device;
        memset(&device, 0, sizeof(device));

        DWORD data_type, length;
        if (!SetupDiGetDeviceRegistryProperty(
                list, &device_info, SPDRP_DEVICEDESC,
                &data_type, description, sizeof(description), &length)) {
            sprintf(error, TEXT("Cannot read device description"));
            handler(error, NULL);
            continue;
        }

        if (!SetupDiGetDeviceRegistryProperty(
                list, &device_info, SPDRP_HARDWAREID,
                &data_type, hardware_id, sizeof(hardware_id), &length)) {
            sprintf(error, TEXT("Cannot read device hardware"));
            handler(error, NULL);
            continue;
        }

        if (!libpci_parse_usb_hardware(hardware_id, &device, error)) {
            handler(error, NULL);
        }

        memcpy(device.device_name, description, LIBPCI_DEVICE_NAME);
        memcpy(device.hardware_id, hardware_id, LIBPCI_DEVICE_ID);
        handler(NULL, &device);
    }
}

bool libpci_parse_usb_hardware(char hardware_id[LIBPCI_DEVICE_ID],
        struct pci_device* device, char error[LIBPCI_ERROR_SIZE]) {
    char* vid = strstr(hardware_id, "VEN_");
    if (vid == NULL) {
        sprintf(error, "Cannot detect vendor id");
        return false;
    }
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);
    for (int i = 4; vid[i] != '&'; i++) {
        buffer[i-4] = vid[i];
    }

    device->vendor_id = strtol(buffer, NULL, 16);
    memset(buffer, 0, BUFSIZ);

    char* pid = strstr(hardware_id, "DEV_");
    if (pid == NULL) {
        sprintf(error, "Cannot detect product id");
        return false;
    }
    for (int i = 4; pid[i] != '&'; i++) {
        buffer[i-4] = pid[i];
    }
    device->product_id = strtol(buffer, NULL, 16);
    return true;
}