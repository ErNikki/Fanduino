#ifndef PRINT_DATA_H
#define PRINT_DATA_H

#include "../avr_client_common/linked_list_with_id.h"
#include <stdio.h>
#include "../avr_client_common/packets.h"
#include "../avr_client_common/color.h"
#include <semaphore.h>
#include "serial_linux.h"

int is_in_array(const int8_t * arr, size_t size, int8_t dato);
int is_pin_used(const int8_t * arr,int8_t * arr_used, size_t size, int8_t dato);
void print_sensor (sem_t * serial_sem , linked_list * sensor_linked_list);
void print_fan(sem_t * serial_sem , linked_list * fan_linked_list);
void print_led(sem_t * serial_sem , linked_list * led_linked_list);
void print_profile(sem_t * serial_sem , linked_list * profile_linked_list);

char * get_analog_name(uint8_t pin);
uint8_t get_analog_pin(char * name);
#endif
