//

#include "adc.h"
#include "stats.h"
#include <stdlib.h>

double adc_raw_to_voltage(uint16_t raw_value) {
    return (raw_value / ADC_MAX_RAW) * ADC_VREF;
}

void adc_calculate_all_channel_stats(const ADCSample *samples, size_t count, ChannelStats channel_stats[4]) {
    if (samples == NULL || channel_stats == NULL) {
        return;
    }

    size_t channel_counts[4] = {0, 0, 0, 0};

    for (size_t i = 0; i < count; i++) {
        if (samples[i].channel_id < 4) {
            channel_counts[samples[i].channel_id]++;
        }
    }

    double *channel_voltages[4];

    for (int ch = 0; ch < 4; ch++) {
        channel_voltages[ch] = malloc(channel_counts[ch] * sizeof(double));

        if (channel_voltages[ch] == NULL) {
            for (int j = 0; j < ch; j++) {
                free(channel_voltages[j]);
            }
            return;
        }
    }

    size_t positions[4] = {0, 0, 0, 0};

    for (size_t i = 0; i < count; i++) {
        uint8_t ch = samples[i].channel_id;

        if (ch < 4) {
            channel_voltages[ch][positions[ch]] = samples[i].voltage;
            positions[ch]++;
        }
    }

    for (int ch = 0; ch < 4; ch++) {
        calculate_channel_stats(channel_voltages[ch], channel_counts[ch], &channel_stats[ch]);
    }

    for (int ch = 0; ch < 4; ch++) {
        free(channel_voltages[ch]);
    }
}

