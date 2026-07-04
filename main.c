#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "adc.h"
#include "io.h"
#include "stats.h"

int main(int argc, char *argv[]) {
    ADCHeader header;
    ADCSample *samples = NULL;
    ChannelStats stats[MAX_CHANNELS];
    FaultStats faults;
    SequenceStats sequence_stats;


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <adcsensorlog.bin>\n", argv[0]);
        return 1;
    }

    if (!read_adc_file(argv[1], &header, &samples)) {
        return 1;
    }

    adc_convert_samples(samples, header.record_count);
    calculate_channel_stats(samples, header.record_count, stats);
    calculate_faults(samples, header.record_count, &faults);
    check_sequence_gaps(samples, header.record_count, &sequence_stats);

    print_header_info(&header);

    printf("\nFirst 5 samples:\n");

    for (uint32_t i = 0; i < 5 && i < header.record_count; i++) {
        printf(
            "Record %u: time=%.4f ch=%u raw=%u voltage=%.6f temp=%.1f flags=0x%02X seq=%u\n",
            i,
            samples[i].timestamp,
            samples[i].channel_id,
            samples[i].raw_value,
            samples[i].voltage,
            samples[i].temperature / 10.0,
            samples[i].status_flags,
            samples[i].sequence_number
        );
    }

    print_channel_stats(stats);
    print_faults(&faults);
    print_sequence_stats(&sequence_stats);

    if (!write_results_file("results.txt", &header, stats, &faults, &sequence_stats)) {
        free(samples);
        return 1;
    }

    printf("\nResults written to results.txt\n");

    free(samples);

    return 0;
}

