#ifndef COMUNICATION_H
#define COMUNICATION_H

#include "../avr_client_common/packets.h"
#include <stdio.h>
#include <stdint.h>
#include "print_data.h"

//FAN
void add_fan(int fd,sem_t * sem, int8_t * used_digital_pin);
void set_fan_speed(int fd,sem_t * sem, linked_list * fan_list);
void delete_fan(int fd,sem_t * sem, linked_list * fan_list, int8_t * used_digital_pin);

//LED
void add_led(int fd,sem_t * sem, int8_t * used_digital_pin);
void set_led_color(int fd, sem_t * sem, linked_list * sem_list);
void delete_led(int fd,sem_t * serial_sem,linked_list * led_linked_list, int8_t * used_digital_pin);

//SENSOR
void add_sensor(int fd, int8_t * used_digital_pin, int8_t * used_analog_pin, sem_t * sem);
void delete_sensor(int fd, sem_t * sem , linked_list * list, int8_t * used_digital_pin , int8_t * used_analog_pin);

//PROFILE
void add_profile(int fd,sem_t * sem, linked_list * fan_list, linked_list * sensor_list);
void remove_profile(int fd, sem_t * serial_sem,linked_list * profile_linked_list);

#endif
