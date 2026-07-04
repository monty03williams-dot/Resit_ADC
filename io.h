#ifndef IO_H
#define IO_H

#include "adc.h"
#include "stats.h"

int read_adc_file(const char *filename, ADCHeader *header, ADCSample **samples);
void print_header_info(const ADCHeader *header);

int write_results_file(
    const char *filename,
    const ADCHeader *header,
    const ChannelStats stats[MAX_CHANNELS],
    const FaultStats *faults,
    const SequenceStats *sequence_stats
);

#endif

