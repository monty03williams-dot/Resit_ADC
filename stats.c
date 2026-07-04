//

#include <stdio.h>
#include "stats.h"

static void initialise_stats(ChannelStats stats[MAX_CHANNELS]) {
    for (int ch = 0; ch < MAX_CHANNELS; ch++) {
        stats[ch].count = 0;

        stats[ch].min_voltage = 0.0;
        stats[ch].max_voltage = 0.0;
        stats[ch].sum_voltage = 0.0;
        stats[ch].average_voltage = 0.0;

        stats[ch].min_temperature = 0.0;
        stats[ch].max_temperature = 0.0;
        stats[ch].sum_temperature = 0.0;
        stats[ch].average_temperature = 0.0;
    }
}

void calculate_channel_stats(
    const ADCSample *samples,
    size_t sample_count,
    ChannelStats stats[MAX_CHANNELS]
) {
    initialise_stats(stats);

    if (samples == NULL) {
        return;
    }

    for (size_t i = 0; i < sample_count; i++) {
        unsigned int ch = samples[i].channel_id;

        if (ch >= MAX_CHANNELS) {
            continue;
        }

        double voltage = samples[i].voltage;
        double temperature = samples[i].temperature / 10.0;

        if (stats[ch].count == 0) {
            stats[ch].min_voltage = voltage;
            stats[ch].max_voltage = voltage;
            stats[ch].min_temperature = temperature;
            stats[ch].max_temperature = temperature;
        } else {
            if (voltage < stats[ch].min_voltage) {
                stats[ch].min_voltage = voltage;
            }

            if (voltage > stats[ch].max_voltage) {
                stats[ch].max_voltage = voltage;
            }

            if (temperature < stats[ch].min_temperature) {
                stats[ch].min_temperature = temperature;
            }

            if (temperature > stats[ch].max_temperature) {
                stats[ch].max_temperature = temperature;
            }
        }

        stats[ch].sum_voltage += voltage;
        stats[ch].sum_temperature += temperature;
        stats[ch].count++;
    }

    for (int ch = 0; ch < MAX_CHANNELS; ch++) {
        if (stats[ch].count > 0) {
            stats[ch].average_voltage =
                stats[ch].sum_voltage / stats[ch].count;

            stats[ch].average_temperature =
                stats[ch].sum_temperature / stats[ch].count;
        }
    }
}

void print_channel_stats(const ChannelStats stats[MAX_CHANNELS]) {
    printf("\nPer-channel statistics:\n");

    for (int ch = 0; ch < MAX_CHANNELS; ch++) {
        printf("\nChannel %d:\n", ch);
        printf("  Count              : %u\n", stats[ch].count);
        printf("  Voltage min        : %.6f V\n", stats[ch].min_voltage);
        printf("  Voltage max        : %.6f V\n", stats[ch].max_voltage);
        printf("  Voltage average    : %.6f V\n", stats[ch].average_voltage);
        printf("  Temperature min    : %.1f C\n", stats[ch].min_temperature);
        printf("  Temperature max    : %.1f C\n", stats[ch].max_temperature);
        printf("  Temperature average: %.1f C\n", stats[ch].average_temperature);
    }
}
