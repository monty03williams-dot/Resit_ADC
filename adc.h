#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stddef.h>

#define ADC_MAGIC 0xADC1BEEF
#define ADC_EXPECTED_VERSION 1
#define ADC_EXPECTED_CHANNELS 4
#define ADC_EXPECTED_SAMPLE_RATE 1000

#define ADC_VREF 3.3
#define ADC_MAX_RAW 4095.0

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

#define OVERVOLTAGE_LIMIT 3.0
#define UNDERVOLTAGE_LIMIT 0.3

typedef struct {
    unsigned int sensor_faults;
    unsigned int overvoltage_faults;
    unsigned int undervoltage_faults;
} FaultStats;

void calculate_faults(
    const ADCSample *samples,
    size_t sample_count,
    FaultStats *faults
);

void print_faults(const FaultStats *faults);
typedef struct {
    unsigned int gap_count;
} SequenceStats;

void check_sequence_gaps(
    const ADCSample *samples,
    size_t sample_count,
    SequenceStats *sequence_stats
);

void print_sequence_stats(const SequenceStats *sequence_stats);

#endif
