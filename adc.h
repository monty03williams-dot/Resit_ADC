//
// Created by Monty Williams on 02/07/2026.
//

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stddef.h>

#define ADC_VREF 3.3
#define ADC_MAX_RAW 4095.0
#define OVERVOLTAGE_THRESHOLD 3.0
#define UNDERVOLTAGE_THRESHOLD 0.3

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t version;
    uint16_t channel_count;
    uint32_t record_count;
    uint32_t sample_rate_hz;
    uint8_t reserved[8];
} ADCHeader;

typedef struct __attribute__((packed)) {
    float timestamp;
    uint8_t channel_id;
    uint16_t raw_value;
    int16_t temperature;
    uint8_t status_flags;
    uint32_t sequence_number;
    uint8_t reserved[2];
} ADCRecordRaw;

typedef struct {
    float timestamp;
    uint8_t channel_id;
    uint16_t raw_value;
    double voltage;
    int16_t temperature;
    uint8_t status_flags;
    uint32_t sequence_number;
} ADCSample;

double adc_raw_to_voltage(uint16_t raw_value);
void adc_convert_samples(ADCSample *samples, size_t count);

#endif

