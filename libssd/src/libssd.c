#include <stdio.h>
#include <Windows.h>
#include <winioctl.h>
#include <nvme.h>

#include "../include/libssd.h"

HANDLE libssd_get_device(CHAR error[LIBSSD_ERROR_SIZE], CHAR disk_path[LIBSSD_DEVICE_PATH_SIZE]) {
    HANDLE result = CreateFile(disk_path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (result == INVALID_HANDLE_VALUE) {
        sprintf(error, TEXT("Cannot open device: %lu"), GetLastError());
        return NULL;
    }
    return result;
}

void libssd_free_device(HANDLE device) {
    CloseHandle(device);
}

void libssd_enumerate_devices(ssd_device_handler handler) {
    CHAR error[LIBSSD_ERROR_SIZE];

    for (int i = 0;; i++) {
        struct storage_device info;
        ZeroMemory(error, LIBSSD_ERROR_SIZE);
        ZeroMemory(&info, sizeof(struct storage_device));

        info.disk_num = i;
        sprintf(info.disk, TEXT("\\\\.\\PhysicalDrive%lu"), i);

        HANDLE device = libssd_get_device(error, info.disk);
        if (strlen(error) != 0) {
            libssd_free_device(device);
            return;
        }

        libssd_get_storage_device_info(error, device, &info);
        if (strlen(error) != 0) {
            handler(error, NULL);
            libssd_free_device(device);
            continue;
        }

        libssd_get_storage_device_memory_info(error, device, &info);
        if (strlen(error) != 0) {
            handler(error, NULL);
            libssd_free_device(device);
            continue;
        }

        libssd_get_storage_interfaces(error, device, &info);
        if (strlen(error) != 0) {
            handler(error, NULL);
            libssd_free_device(device);
            continue;
        }

        handler(NULL, &info);
        libssd_free_device(device);
    }
}

void libssd_get_storage_device_info(CHAR error[LIBSSD_ERROR_SIZE], HANDLE device, struct storage_device* info) {
    LPVOID device_info_buffer[LIBSSD_STORAGE_DEVICE_INFO_SIZE];
    STORAGE_PROPERTY_QUERY storage_property_query;
    ZeroMemory(&storage_property_query, sizeof(STORAGE_PROPERTY_QUERY));

    if (!DeviceIoControl(device,
                         IOCTL_STORAGE_QUERY_PROPERTY,
                         &storage_property_query,
                         sizeof(STORAGE_PROPERTY_QUERY),
                         device_info_buffer,
                         LIBSSD_STORAGE_DEVICE_INFO_SIZE,
                         NULL,
                         NULL)) {
        sprintf(error, TEXT("Cannot get device product: %lu"), GetLastError());
        return;
    }

    STORAGE_DEVICE_DESCRIPTOR* device_descriptor = (STORAGE_DEVICE_DESCRIPTOR*)device_info_buffer;

    if (device_descriptor->ProductIdOffset)
        strcpy(info->model, device_descriptor->ProductIdOffset + (char*)device_info_buffer);
    if (device_descriptor->VendorIdOffset)
        strcpy(info->creator, device_descriptor->VendorIdOffset + (char*)device_info_buffer);
    if (device_descriptor->SerialNumberOffset)
        strcpy(info->serial, device_descriptor->SerialNumberOffset + (char*)device_info_buffer);
    if (device_descriptor->ProductRevisionOffset)
        strcpy(info->firmware, device_descriptor->ProductRevisionOffset + (char*)device_info_buffer);
    strcpy(info->bus_type, storage_device_type[device_descriptor->BusType]);
}

void libssd_get_storage_device_memory_info(CHAR error[LIBSSD_DEVICE_PATH_SIZE],
        HANDLE device, struct storage_device* info) {

    DWORD logical_drives = GetLogicalDrives();
    DWORDLONG total_free_space = 0;
    DWORDLONG total_storage_space = 0;

    ULARGE_INTEGER free_space;
    ULARGE_INTEGER storage_space;

    for (int i = 'C'; logical_drives != 0; i++) {
        if (!(logical_drives % 2)) {
            CHAR logical_drive[LIBSSD_DESCRIPTION];
            sprintf(logical_drive, TEXT("%c:\\"), i);
            if (!GetDiskFreeSpaceEx(logical_drive, 0, &storage_space, &free_space)) {
                sprintf(error, TEXT("Cannot get logical disk space: %lu"), GetLastError());
                return;
            }

            CHAR logical_volume[LIBSSD_DESCRIPTION];
            sprintf(logical_volume, TEXT("\\\\.\\%c:"), i);
            HANDLE logical_handle = CreateFile(logical_volume, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
            if (logical_handle == INVALID_HANDLE_VALUE) {
                sprintf(error, TEXT("Cannot get logical disk volume: %lu"), GetLastError());
                return;
            }

            BYTE storage_device_buffer[12];
            ZeroMemory(storage_device_buffer, 12);

            if (!DeviceIoControl(logical_handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0,
                                 storage_device_buffer, 12, NULL, NULL))
            {
                sprintf(error, TEXT("Cannot get device number: %lu"), GetLastError());
                libssd_free_device(logical_handle);
                return;
            }

            STORAGE_DEVICE_NUMBER* device_number = (STORAGE_DEVICE_NUMBER*)storage_device_buffer;

            if (device_number->DeviceNumber == info->disk_num) {
                total_free_space += free_space.QuadPart;
                total_storage_space += storage_space.QuadPart;
            }
        }
        logical_drives >>= 1;
    }

    info->disk_size = total_storage_space;
    info->free_disk_space = total_free_space;
}

void libssd_get_storage_interfaces(CHAR error[LIBSSD_DEVICE_PATH_SIZE],
                                   HANDLE device, struct storage_device* info) {
    ULONG buffer_len = FIELD_OFFSET(STORAGE_PROPERTY_QUERY, AdditionalParameters)
            + sizeof(STORAGE_PROTOCOL_SPECIFIC_DATA) + NVME_MAX_LOG_SIZE;
    PVOID buffer = malloc(buffer_len * sizeof(BYTE));
    ULONG returned_len = 0;

    for (USHORT feature_num = 1; feature_num <= 131; feature_num++)
    {
        if (feature_num == 18)
            feature_num = 128;

        ZeroMemory(buffer, buffer_len);

        PSTORAGE_PROPERTY_QUERY query = (PSTORAGE_PROPERTY_QUERY)buffer;
        PSTORAGE_PROTOCOL_DATA_DESCRIPTOR protocol_data_descr = (PSTORAGE_PROTOCOL_DATA_DESCRIPTOR)buffer;
        PSTORAGE_PROTOCOL_SPECIFIC_DATA protocol_data = (PSTORAGE_PROTOCOL_SPECIFIC_DATA)query->AdditionalParameters;

        query->PropertyId = StorageDeviceProtocolSpecificProperty;
        query->QueryType = PropertyStandardQuery;

        protocol_data->ProtocolType = ProtocolTypeNvme;
        protocol_data->DataType = NVMeDataTypeFeature;
        protocol_data->ProtocolDataRequestValue = feature_num;
        protocol_data->ProtocolDataRequestSubValue = 0;
        protocol_data->ProtocolDataOffset = 0;
        protocol_data->ProtocolDataLength = 0;

        BOOL result = DeviceIoControl(device,
                                 IOCTL_STORAGE_QUERY_PROPERTY,
                                 buffer,
                                 buffer_len,
                                 buffer,
                                 buffer_len,
                                 &returned_len,
                                 NULL
        );

        if (!result || (returned_len == 0)) {
            continue;
        }

        if ((protocol_data_descr->Version != sizeof(STORAGE_PROTOCOL_DATA_DESCRIPTOR)) ||
            (protocol_data_descr->Size != sizeof(STORAGE_PROTOCOL_DATA_DESCRIPTOR))) {
            continue;
        }
        sprintf(info->features,
                TEXT("%sSupport: %s\n"),
                info->features,
                (feature_num <= 17 ? nvme_features[feature_num] : nvme_features_128[feature_num - 128]));
    }
}