//
// Created by Monty Williams on 02/07/2026.
//

#ifndef STATS_H
#define STATS_H

#include <stddef.h>

typedef struct {
    double mean;
    double min;
    double max;
    double standard_deviation;
    unsigned int count;
} ChannelStats;

void calculate_channel_stats(const double *values, size_t count, ChannelStats *stats);

#endif
