//
// Created by Monty Williams on 02/07/2026.
//

#ifndef IO_H
#define IO_H

#include "adc.h"

int read_adc_header(const char *filename, ADCHeader *header);
void print_header_info(const ADCHeader *header);

#endif

