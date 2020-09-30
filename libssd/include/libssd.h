#pragma once

#include <windows.h>
#include <setupapi.h>

#define LIBSSD_ERROR_SIZE 1024
#define LIBSSD_MAX_FEATURE_SIZE 1024
#define LIBSSD_DEVICE_PATH_SIZE 256
#define LIBSSD_DESCRIPTION 128
#define LIBSSD_STORAGE_DEVICE_INFO_SIZE 4096

static char* storage_device_type[] = {
        "BusTypeUnknown",
        "BusTypeScsi",
        "BusTypeAtapi",
        "BusTypeAta3",
        "BusType1394",
        "BusTypeSsa",
        "BusTypeFibre",
        "BusTypeUsb",
        "BusTypeRAID",
        "BusTypeiScsi",
        "BusTypeSas",
        "BusTypeSata",
        "BusTypeSd",
        "BusTypeMmc",
        "BusTypeVirtual",
        "BusTypeFileBackedVirtual",
        "BusTypeSpaces",
        "BusTypeMax",
        "BusTypeMaxReserved"
};

static char* nvme_features[] = {
        "NOT USED",
        "ARBITRATION",
        "POWER MANAGEMENT",
        "LBA RANGE TYPE",
        "TEMPERATURE THRESHOLD",
        "ERROR RECOVERY",
        "VOLATILE WRITE CACHE",
        "NUMBER OF QUEUES",
        "INTERRUPT COALESCING",
        "INTERRUPT VECTOR CONFIG",
        "WRITE ATOMICITY",
        "ASYNC EVENT CONFIG",
        "AUTONOMOUS POWER STATE TRANSITION",
        "HOST MEMORY BUFFER",
        "TIMESTAMP",
        "KEEP ALIVE",
        "HOST CONTROLLED THERMAL MANAGEMENT",
        "NONOPERATIONAL POWER STATE"
};

static char* nvme_features_128[] = {
        "SOFTWARE PROGRESS MARKER",
        "HOST IDENTIFIER",
        "RESERVATION NOTIFICATION MASK",
        "RESERVATION PERSISTANCE"
};

struct storage_device {
    CHAR disk[LIBSSD_DEVICE_PATH_SIZE];
    CHAR model[LIBSSD_DESCRIPTION];
    CHAR creator[LIBSSD_DESCRIPTION];
    CHAR serial[LIBSSD_DESCRIPTION];
    CHAR firmware[LIBSSD_DESCRIPTION];
    CHAR bus_type[LIBSSD_DESCRIPTION];
    INT disk_num;
    DWORDLONG disk_size;
    DWORDLONG free_disk_space;
    CHAR features[LIBSSD_MAX_FEATURE_SIZE];
};

typedef void(*ssd_device_handler)(CHAR[LIBSSD_ERROR_SIZE], struct storage_device*);

extern HANDLE libssd_get_device(CHAR error[LIBSSD_ERROR_SIZE], CHAR disk_path[LIBSSD_DEVICE_PATH_SIZE]);

extern void libssd_get_storage_device_info(CHAR error[LIBSSD_ERROR_SIZE], HANDLE device, struct storage_device* info);
extern void libssd_get_storage_device_memory_info(CHAR error[LIBSSD_DEVICE_PATH_SIZE],
        HANDLE device, struct storage_device* info);

extern void libssd_get_storage_interfaces(CHAR error[LIBSSD_DEVICE_PATH_SIZE],
        HANDLE device, struct storage_device* info);

extern void libssd_enumerate_devices(ssd_device_handler handler);