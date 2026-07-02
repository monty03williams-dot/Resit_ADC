//

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

static int validate_header(const ADCHeader *header) {
    if (header->magic != 0xADC1BEEF) {
        fprintf(stderr, "Error: invalid magic number\n");
        return 0;
    }

    if (header->version != 1) {
        fprintf(stderr, "Error: unsupported file version\n");
        return 0;
    }

    if (header->channel_count != 4) {
        fprintf(stderr, "Error: unexpected channel count\n");
        return 0;
    }

    if (header->sample_rate_hz != 1000) {
        fprintf(stderr, "Error: unexpected sample rate\n");
        return 0;
    }

    return 1;
}

void print_header_info(const ADCHeader *header) {
    printf("Magic number   : 0x%X\n", header->magic);
    printf("Version        : %u\n", header->version);
    printf("Channel count  : %u\n", header->channel_count);
    printf("Record count   : %u\n", header->record_count);
    printf("Sample rate    : %u Hz\n", header->sample_rate_hz);
}

int read_adc_file(const char *filename, ADCHeader *header, ADCSample **samples) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        return 0;
    }

    if (fread(header, sizeof(ADCHeader), 1, fp) != 1) {
        fprintf(stderr, "Error: could not read file header\n");
        fclose(fp);
        return 0;
    }

    if (!validate_header(header)) {
        fclose(fp);
        return 0;
    }

    *samples = malloc(header->record_count * sizeof(ADCSample));
    if (*samples == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        fclose(fp);
        return 0;
    }

    for (uint32_t i = 0; i < header->record_count; i++) {
        ADCRecordRaw raw_record;

        if (fread(&raw_record, sizeof(ADCRecordRaw), 1, fp) != 1) {
            fprintf(stderr, "Error: could not read record %u\n", i);
            free(*samples);
            *samples = NULL;
            fclose(fp);
            return 0;
        }

        (*samples)[i].timestamp = raw_record.timestamp;
        (*samples)[i].channel_id = raw_record.channel_id;
        (*samples)[i].raw_value = raw_record.raw_value;
        (*samples)[i].temperature = raw_record.temperature;
        (*samples)[i].status_flags = raw_record.status_flags;
        (*samples)[i].sequence_number = raw_record.sequence_number;
        (*samples)[i].voltage = adc_raw_to_voltage(raw_record.raw_value);
    }

    fclose(fp);
    return 1;
}
