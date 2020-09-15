#include <stdio.h>
#include <libssd.h>

void handler(CHAR error[LIBSSD_ERROR_SIZE], struct storage_device* device) {
    if (error != NULL) {
        printf("Error: %s\n", error);
        return;
    }
    printf("Device: %s\nHardware id: %s\nProduct id: %s\nVendor id: %s\n",
           device->serial_id, device->firmware_id, device->vendor_id, device->product_id);
    puts("---------------------------");
}

int main() {
    libssd_enumerate_devices(handler);
    return 0;
}
