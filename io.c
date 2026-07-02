#include <stdio.h>
#include "io.h"

int read_adc_header(const char *filename, ADCHeader *header) {
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

    fclose(fp);

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
