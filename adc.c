//

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
