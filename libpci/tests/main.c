#include <stdio.h>
#include <libpci.h>

void handler(char error[LSPCI_ERROR_SIZE], struct pci_device* device) {
    if (error != NULL) {
        printf("Error: %s", error);
        return;
    }
    printf("Device: %s\nVendor id: %d\nProduct id: %d\n",
            device->device_name, device->vendor_id, device->product_id);
}

int main() {
    char error[LSPCI_ERROR_SIZE];
    struct pci_devices_list* list = lspci_get_devices_list(error);

    lspci_enumerate_devices(list, handler);

    lspci_free_devices_list(list);
    return 0;
}
