#pragma once

#include <windows.h>
#include <setupapi.h>

#define LIBPCI_ERROR_SIZE 1024
#define LIBPCI_DEVICE_NAME 512
#define LIBPCI_DEVICE_ID 1024

struct pci_device {
    char device_name[LIBPCI_DEVICE_NAME];
    char hardware_id[LIBPCI_DEVICE_ID];
    unsigned int vendor_id;
    unsigned int product_id;
};

typedef void(*pci_device_handler)(char[LIBPCI_ERROR_SIZE], struct pci_device*);

extern HDEVINFO lspci_get_devices_list(char error[LIBPCI_ERROR_SIZE]);

extern void lspci_enumerate_devices(HDEVINFO devices_list, pci_device_handler);

extern void lspci_free_devices_list(HDEVINFO devices_list);