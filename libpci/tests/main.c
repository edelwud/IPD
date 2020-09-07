#include <stdio.h>
#include <libpci.h>

void handler(char error[LIBPCI_ERROR_SIZE], struct pci_device* device) {
    if (error != NULL) {
        printf("Error: %s\n", error);
        return;
    }
    printf("Device: %s\nHardware id: %s\nProduct id: %d\nVendor id: %d\n",
            device->device_name, device->hardware_id, device->product_id, device->vendor_id);
}

int main() {
    char error[LIBPCI_ERROR_SIZE];
    HDEVINFO list = lspci_get_devices_list(error);

    lspci_enumerate_devices(list, handler);

    lspci_free_devices_list(list);
    return 0;
}
