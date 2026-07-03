//

#include "stats.h"
#include <math.h>

void calculate_channel_stats(const double *values, size_t count, ChannelStats *stats) {
    if (values == NULL || stats == NULL || count == 0) {
        return;
    }

    double sum = 0.0;

    stats->min = values[0];
    stats->max = values[0];
    stats->count = (unsigned int) count;

    for (size_t i = 0; i < count; i++) {
        if (values[i] < stats->min) {
            stats->min = values[i];
        }

        if (values[i] > stats->max) {
            stats->max = values[i];
        }

        sum += values[i];
    }

    stats->mean = sum / count;

    double squared_difference_sum = 0.0;

    for (size_t i = 0; i < count; i++) {
        double difference = values[i] - stats->mean;
        squared_difference_sum += difference * difference;
    }

    stats->standard_deviation = sqrt(squared_difference_sum / count);
}

