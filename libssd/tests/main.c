#include <stdio.h>
#include <libssd.h>

void handler(CHAR error[LIBSSD_ERROR_SIZE], struct storage_device* device) {
    if (error != NULL) {
        printf("Error: %s\n", error);
        return;
    }
    printf("Model: %s\nCreator: %s\nSerial: %s\nFirmware: %s\nBus type: %s\n\n",
           device->model, device->creator, device->serial, device->firmware, device->bus_type);
    printf("Total device volume: %.2fGB\n", (double)(device->disk_size / 1024 / 1024 / 1024));
    printf("Free device volume: %.2fGB\n", (double)(device->free_disk_space / 1024 / 1024 / 1024));
    printf("\n%s", device->features);
    puts("---------------------------");
}

int main() {
    libssd_enumerate_devices(handler);
    return 0;
}
