//

#include <stdio.h>
#include "adc.h"
#include "io.h"

int main(int argc, char *argv[]) {
    ADCHeader header;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <adc_sensor_log.bin>\n", argv[0]);
        return 1;
    }

    if (!read_adc_header(argv[1], &header)) {
        return 1;
    }

    print_header_info(&header);

    return 0;
}
