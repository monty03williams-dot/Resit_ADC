//

#include "adc.h"

double adc_raw_to_voltage(uint16_t raw_value) {
    return (raw_value / ADC_MAX_RAW) * ADC_VREF;
}

