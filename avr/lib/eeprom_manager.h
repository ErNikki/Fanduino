#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include "linked_list_with_id_and_pin.h"
#include "../../avr_client_common/linked_list_with_id.h"
#include <avr/eeprom.h>
#include "sensor.h"
#include "fan.h"
#include "profilazione.h"
#include "led.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

struct sensor_eeprom_s
{
  uint8_t pin;
  uint8_t type;
};

struct fan_eeprom_s
{
  uint8_t pwm_pin;
  int8_t encoder_pin;
  uint8_t speed_flag;
  uint8_t speed;

};

struct led_eeprom_s
{
  uint8_t pin_red;
  uint8_t pin_green;
  uint8_t pin_blue;
  uint8_t pin_color;
};

struct profile_eeprom_s
{
  uint8_t sensor_pin;
  uint8_t fan_id;
  uint8_t mode;
  unsigned char perc_x_temp[10];
};

void add_sensor_to_eeprom(uint8_t pin, uint8_t type);
void add_fan_to_eeprom(uint8_t pwm_pin, int8_t encoder_pin);
void add_led_to_eeprom(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue);//, uint8_t pin_color);
void add_profile_to_eeprom(uint8_t sensor_pin, uint8_t fan_id, uint8_t mode, uint8_t *perc_x_temp);

void add_sensors_from_eeprom(void);
void add_fans_from_eeprom(void);
void add_leds_from_eeprom(void);
void add_profiles_from_eeprom(void);
void add_structs_from_eeprom(void);

void init_from_eeprom(void);
void update_eeprom(void);
void update_eeprom_led(void);
void update_eeprom_fan(void);
void update_eeprom_sensor(void);
void update_eeprom_profile(void);


void eeprom_dirty_clean(void);
void clean_eeprom(void);

void add_color_to_led_eeprom(uint8_t red_pin,uint8_t color);
void add_speed_to_fan_eeprom(uint8_t id_fan, uint8_t speed);
void set_flag_speed_to_zero(uint8_t id_fan);

/*
quando chiamo add_speed setta il flag a 1
quando chiamo add_profile sia from eeprom che to eeprom viene chiamata set_flag_speed_to_zero
e la velocità viene inizializzata nella add_fans_from_eeprom
*/


#endif
