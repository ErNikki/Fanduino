#ifndef PACKETS_H
#define PACKETS_H

#include <stdint.h>

/*
Definisci qua le strutture per lo scambio di pacchetti tra avr e il pc
*/
#define MAX_DATA 27

struct data {
  uint32_t data_size;
  uint8_t data_type;
  uint8_t data[MAX_DATA];
}data;

// AVR -> PC

#define SENSOR 1
struct sensor_packet{
    uint8_t id;
    uint8_t type;
    uint8_t pin;
    uint8_t temperature;
}sensor_packet;

#define FAN 2
struct fan_packet {
    uint8_t id;
    uint8_t pwm_pin;
    int8_t encoder_pin;
    uint8_t speed;
}fan_packet;

#define RGB_LED 3
struct rgb_led_packet {
    uint8_t id;
    uint8_t red_pin;
    uint8_t green_pin;
    uint8_t blue_pin;
    uint8_t current_color;
}rgb_led_packet;

#define PROFILE 4
//TODO: da rivedere
struct profile_packet{
    uint8_t id;
    uint8_t pin;
    uint8_t mode;
    uint8_t perc_x_temp[10];
}profile_packet;

#define DELETED_DATA 5
struct deleted_data {
  uint8_t type;
}deleted_data;

// PC -> AVR

//FAN
#define ADD_FAN 6
struct add_fan_packet {
    int8_t pwm_pin;
    int8_t encoder_pin;
}add_fan_packet;

#define SET_FAN_SPEED 7
struct set_fan_speed_packet {
    uint8_t id;
    uint8_t speed;
}set_fan_speed_packet;


#define REMOVE_FAN 8
struct remove_fan_packet {
  uint8_t id;
}remove_fan_packet;

//LED

#define ADD_RGB_LED 9
struct add_rgb_led_packet{
  uint8_t red_pin;
  uint8_t green_pin;
  uint8_t blue_pin;
}add_rgb_led_packet;

#define SET_RGB_LED_COLOR 10
struct set_rgb_led_color_packet{
  uint8_t id;
  uint8_t color;
}set_rgb_led_color_packet;

#define REMOVE_RGB_LED 11
struct remove_rgb_led_packet{
  uint8_t id;
}remove_rgb_led_packet;

//SENSOR
#define ADD_SENSOR 12
struct add_sensor_packet{
  uint8_t type;
  uint8_t pin;
}add_sensor_packet;

#define REMOVE_SENSOR 13
struct remove_sensor_packet{
  uint8_t id;
}remove_sensor_packet;

#define ADD_PROFILE 14
struct add_profile_packet{
  uint8_t fan_id;
  uint8_t sensor_id;
  uint8_t mode;
  uint8_t manual_array[10];
}add_profile_packet;

#define REMOVE_PROFILE 15
struct remove_profile_packet{
  uint8_t id;
}remove_profile_packet;

#define CLEAR_EEPROM 20
#define GET_DATA 17
#define EMPTY_DATA 18

#endif
