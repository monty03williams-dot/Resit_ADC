//

#include <stdio.h>
#include <stdlib.h>
#include "adc.h"
#include "io.h"
#include "stats.h"

int main(int argc, char *argv[]) {
    ADCHeader header;
    ADCSample *samples = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <adc_sensor_log.bin>\n", argv[0]);
        return 1;
    }

    if (!read_adc_file(argv[1], &header, &samples)) {
        return 1;
    }

    print_header_info(&header);

    printf("\nFirst 5 samples:\n");
    for (uint32_t i = 0; i < 5 && i < header.record_count; i++) {
        printf("Record %u: time=%.4f ch=%u raw=%u voltage=%.6f temp=%.1f flags=0x%02X seq=%u\n",
               i,
               samples[i].timestamp,
               samples[i].channel_id,
               samples[i].raw_value,
               samples[i].voltage,
               samples[i].temperature / 10.0,
               samples[i].status_flags,
               samples[i].sequence_number);
    }
    ChannelStats channel_stats[4];

    adc_calculate_all_channel_stats(samples, header.record_count, channel_stats);

    printf("\nPer-channel voltage statistics:\n");

    for (int ch = 0; ch < 4; ch++) {
        printf("Channel %d: count=%u mean=%.6f V min=%.6f V max=%.6f V stddev=%.6f V\n",
               ch,
               channel_stats[ch].count,
               channel_stats[ch].mean,
               channel_stats[ch].min,
               channel_stats[ch].max,
               channel_stats[ch].standard_deviation);
    }


    free(samples);
    return 0;
}

