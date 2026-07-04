//

#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include "adc.h"

#define MAX_CHANNELS 4

typedef struct {
    unsigned int count;

    double min_voltage;
    double max_voltage;
    double sum_voltage;
    double average_voltage;

    double min_temperature;
    double max_temperature;
    double sum_temperature;
    double average_temperature;
} ChannelStats;

void calculate_channel_stats(
    const ADCSample *samples,
    size_t sample_count,
    ChannelStats stats[MAX_CHANNELS]
);

void print_channel_stats(const ChannelStats stats[MAX_CHANNELS]);

#endif
