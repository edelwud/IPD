#pragma once

#include <windows.h>

#define LIBPCI_ERROR_SIZE 1024
#define LIBPCI_DEVICE_NAME 512

struct pci_device {
    char device_name[LIBPCI_DEVICE_NAME];
    unsigned int vendor_id;
    unsigned int product_id;
};

struct pci_devices_list {
    unsigned int device_count;
    PRAWINPUTDEVICELIST device_list;
};

typedef void(*pci_device_handler)(char[LIBPCI_ERROR_SIZE], struct pci_device*);

extern struct pci_devices_list* lspci_get_devices_list(char error[LIBPCI_ERROR_SIZE]);

extern void lspci_enumerate_devices(struct pci_devices_list* devices_list, pci_device_handler);

extern void lspci_free_devices_list(struct pci_devices_list* devices_list);