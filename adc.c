#include <stdio.h>
#include "adc.h"

double adc_raw_to_voltage(uint16_t raw_value) {
    return (raw_value / ADC_MAX_RAW) * ADC_VREF;
}

void adc_convert_samples(ADCSample *samples, size_t count) {
    if (samples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        samples[i].voltage = adc_raw_to_voltage(samples[i].raw_value);
    }
}
#include <stdio.h>

void calculate_faults(
    const ADCSample *samples,
    size_t sample_count,
    FaultStats *faults
) {
    faults->sensor_faults = 0;
    faults->overvoltage_faults = 0;
    faults->undervoltage_faults = 0;

    if (samples == NULL || faults == NULL) {
        return;
    }

    for (size_t i = 0; i < sample_count; i++) {
        if (samples[i].status_flags & 0x01) {
            faults->sensor_faults++;
        }

        if (samples[i].voltage > OVERVOLTAGE_LIMIT) {
            faults->overvoltage_faults++;
        }

        if (samples[i].voltage < UNDERVOLTAGE_LIMIT) {
            faults->undervoltage_faults++;
        }
    }
}

void print_faults(const FaultStats *faults) {
    printf("\nFault summary:\n");
    printf("  Sensor fault flags : %u\n", faults->sensor_faults);
    printf("  Overvoltage faults : %u\n", faults->overvoltage_faults);
    printf("  Undervoltage faults: %u\n", faults->undervoltage_faults);
}
void check_sequence_gaps(
    const ADCSample *samples,
    size_t sample_count,
    SequenceStats *sequence_stats
) {
    sequence_stats->gap_count = 0;

    if (samples == NULL || sequence_stats == NULL || sample_count < 2) {
        return;
    }

    for (size_t i = 1; i < sample_count; i++) {
        uint32_t expected = samples[i - 1].sequence_number + 1;

        if (samples[i].sequence_number != expected) {
            sequence_stats->gap_count++;
        }
    }
}

void print_sequence_stats(const SequenceStats *sequence_stats) {
    printf("\nSequence summary:\n");
    printf("  Sequence gaps: %u\n", sequence_stats->gap_count);
}
