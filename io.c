#include <stdio.h>
#include <stdlib.h>
#include "io.h"

static int validate_header(const ADCHeader *header) {
    if (header->magic != ADC_MAGIC) {
        fprintf(stderr, "Error: invalid magic number: 0x%X\n", header->magic);
        return 0;
    }

    if (header->version != ADC_EXPECTED_VERSION) {
        fprintf(stderr, "Error: unsupported version: %u\n", header->version);
        return 0;
    }

    if (header->channel_count != ADC_EXPECTED_CHANNELS) {
        fprintf(stderr, "Error: unexpected channel count: %u\n", header->channel_count);
        return 0;
    }

    if (header->sample_rate_hz != ADC_EXPECTED_SAMPLE_RATE) {
        fprintf(stderr, "Error: unexpected sample rate: %u\n", header->sample_rate_hz);
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
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        return 0;
    }

    if (fread(header, sizeof(ADCHeader), 1, fp) != 1) {
        fprintf(stderr, "Error: failed to read file header\n");
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
        ADCRecordRaw raw;

        if (fread(&raw, sizeof(ADCRecordRaw), 1, fp) != 1) {
            fprintf(stderr, "Error: failed to read record %u\n", i);
            free(*samples);
            *samples = NULL;
            fclose(fp);
            return 0;
        }

        (*samples)[i].timestamp = raw.timestamp;
        (*samples)[i].channel_id = raw.channel_id;
        (*samples)[i].raw_value = raw.raw_value;
        (*samples)[i].temperature = raw.temperature;
        (*samples)[i].status_flags = raw.status_flags;
        (*samples)[i].sequence_number = raw.sequence_number;
        (*samples)[i].voltage = 0.0;
    }

    fclose(fp);
    return 1;
}
int write_results_file(
    const char *filename,
    const ADCHeader *header,
    const ChannelStats stats[MAX_CHANNELS],
    const FaultStats *faults,
    const SequenceStats *sequence_stats
) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        fprintf(stderr, "Error: could not create results file '%s'\n", filename);
        return 0;
    }

    fprintf(fp, "ADC Sensor Log Results\n");
    fprintf(fp, "======================\n\n");

    fprintf(fp, "Header\n");
    fprintf(fp, "------\n");
    fprintf(fp, "Magic number   : 0x%X\n", header->magic);
    fprintf(fp, "Version        : %u\n", header->version);
    fprintf(fp, "Channel count  : %u\n", header->channel_count);
    fprintf(fp, "Record count   : %u\n", header->record_count);
    fprintf(fp, "Sample rate    : %u Hz\n", header->sample_rate_hz);

    fprintf(fp, "\nPer-channel statistics\n");
    fprintf(fp, "----------------------\n");

    for (int ch = 0; ch < MAX_CHANNELS; ch++) {
        fprintf(fp, "\nChannel %d\n", ch);
        fprintf(fp, "  Count              : %u\n", stats[ch].count);
        fprintf(fp, "  Voltage min        : %.6f V\n", stats[ch].min_voltage);
        fprintf(fp, "  Voltage max        : %.6f V\n", stats[ch].max_voltage);
        fprintf(fp, "  Voltage average    : %.6f V\n", stats[ch].average_voltage);
        fprintf(fp, "  Temperature min    : %.1f C\n", stats[ch].min_temperature);
        fprintf(fp, "  Temperature max    : %.1f C\n", stats[ch].max_temperature);
        fprintf(fp, "  Temperature average: %.1f C\n", stats[ch].average_temperature);
    }

    fprintf(fp, "\nFault summary\n");
    fprintf(fp, "-------------\n");
    fprintf(fp, "Sensor fault flags : %u\n", faults->sensor_faults);
    fprintf(fp, "Overvoltage faults : %u\n", faults->overvoltage_faults);
    fprintf(fp, "Undervoltage faults: %u\n", faults->undervoltage_faults);

    fprintf(fp, "\nSequence summary\n");
    fprintf(fp, "----------------\n");
    fprintf(fp, "Sequence gaps: %u\n", sequence_stats->gap_count);

    fclose(fp);
    return 1;
}
