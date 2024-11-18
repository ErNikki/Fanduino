#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "packets.h"



int serial_order_data(uint8_t * src,uint8_t * dst, size_t size);
struct data fill_data(uint8_t * data, uint32_t data_size, uint8_t data_type);
void parse_data (struct data * message_data, uint8_t * data, uint32_t data_size);

#endif
