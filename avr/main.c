#include "lib/uart.h"
#include "lib/eeprom_manager.h"
#include "lib/led.h"
#include "lib/profilazione.h"
#include "../avr_client_common/packets.h"
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

struct data d;
struct UART * uart;

struct sensor_packet s_packet;
struct fan_packet f_packet;
struct rgb_led_packet led_packet;
struct profile_packet p_packet;

void delete_data(uint8_t type){
  struct deleted_data packet;
  packet.type=type;
  UART_putData(uart, (uint8_t*)&packet,sizeof(struct deleted_data),DELETED_DATA);
}

void send_data(void) {
  node * n;
  node_pi * npi;

  n = get_sensor_list_head();
  if(n==NULL)
    UART_putData(uart, NULL ,0,EMPTY_DATA);
  while(n!=NULL){
    sensor * s = n->data;
    s_packet.id = s->pin;
    s_packet.type =s->type;
    s_packet.pin = s->pin;
    int t = get_temperature(s->pin);
    if (t>=0) s_packet.temperature = t;
    else s_packet.temperature = abs(t);
    UART_putData(uart, (uint8_t*)&s_packet ,sizeof(struct sensor_packet),SENSOR);
    n=n->next;
  }

  if(get_fan_number()==0)
    UART_putData(uart, NULL ,0,EMPTY_DATA);
  for(int i = 0; i<FAN_NUMBER; i++){
    struct fan ** f = get_fan_manager();
    if(f[i]!=NULL){
      f_packet.id=f[i]->pwm_pin;
      f_packet.pwm_pin=f[i]->pwm_pin;
      f_packet.encoder_pin=f[i]->encoder_pin;
      f_packet.speed=f[i]->speed;
      UART_putData(uart, (uint8_t *)&f_packet, sizeof(struct fan_packet), FAN);
    }
  }


  n = get_led_list_head();
  if(n==NULL)
    UART_putData(uart, NULL ,0,EMPTY_DATA);
  while(n!=NULL){
    struct led * l = n->data;
    led_packet.id = l->red_pin;
    led_packet.red_pin = l->red_pin;
    led_packet.green_pin = l->green_pin;
    led_packet.blue_pin = l->blue_pin;
    led_packet.current_color = l->current_color;
    UART_putData(uart, (uint8_t *)&led_packet , sizeof(struct rgb_led_packet), RGB_LED);
    n=n->next;
  }

  npi = get_profile_list_head();
  if(npi==NULL)
    UART_putData(uart, NULL ,0,EMPTY_DATA);
  while(npi!=NULL){
    struct profile * p = npi->data;
    p_packet.id = p->id_fan;
    p_packet.pin = p->pin_sensor;
    p_packet.mode = p->mode;
    memcpy(p_packet.perc_x_temp,p->perc_x_temp,10);
    UART_putData(uart, (uint8_t *)&p_packet, sizeof(struct profile_packet), PROFILE);
    npi=npi->next;
  }
}

void update_data(struct data * d) {
  if(d==NULL)return;
  else if(d->data_type == ADD_FAN){
    struct add_fan_packet afp;
    parse_data (d, (uint8_t*)&afp, sizeof(struct add_fan_packet));
    add_fan(afp.pwm_pin,afp.encoder_pin);
    //prova
    add_fan_to_eeprom(afp.pwm_pin,afp.encoder_pin);
  }
  else if(d->data_type == SET_RGB_LED_COLOR){
    struct set_rgb_led_color_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct set_rgb_led_color_packet));
    set_color(packet.id,packet.color);
    //TODO funzione che aggiorna il colore del led nella eeprom
    add_color_to_led_eeprom(packet.id, packet.color);
  }
  else if(d->data_type == SET_FAN_SPEED){
    struct set_fan_speed_packet fsp;
    parse_data (d, (uint8_t*)&fsp, sizeof(struct set_fan_speed_packet));
    set_speed(fsp.id,fsp.speed);
    add_speed_to_fan_eeprom(fsp.id,fsp.speed);
  }

  else if(d->data_type == REMOVE_FAN){
    struct remove_fan_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct remove_fan_packet));
    remove_fan(packet.id);
    remove_profile_by_id(packet.id);
    delete_data(FAN);
    delete_data(PROFILE);
    //prova
    update_eeprom_fan();
    update_eeprom_profile();
  }
  else if(d->data_type == ADD_RGB_LED){
    struct add_rgb_led_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct add_rgb_led_packet));
    add_rgb_led(packet.red_pin,packet.green_pin,packet.blue_pin);
    //prova
    add_led_to_eeprom(packet.red_pin,packet.green_pin,packet.blue_pin);
  }
  else if(d->data_type == REMOVE_RGB_LED){
    struct remove_rgb_led_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct remove_rgb_led_packet));
    remove_rgb_led(packet.id);
    delete_data(RGB_LED);
    //prova
    update_eeprom_led();
  }
  else if(d->data_type == ADD_SENSOR){
    struct add_sensor_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct add_sensor_packet));
    //add_sensor_to_eeprom(packet.pin,packet.type);
    add_sensor(packet.pin,packet.type);
    //prova
    add_sensor_to_eeprom(packet.pin,packet.type);
  }
  else if(d->data_type == REMOVE_SENSOR){
    struct remove_sensor_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct remove_sensor_packet));
    remove_sensor(packet.id);
    remove_profile_by_pin(packet.id);
    delete_data(SENSOR);
    delete_data(PROFILE);
    //prova
    update_eeprom_sensor();
    update_eeprom_profile();
  }
  else if(d->data_type == ADD_PROFILE){
    struct add_profile_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct add_profile_packet));
    add_profile(packet.fan_id,packet.sensor_id,packet.mode,packet.manual_array);
    add_profile_to_eeprom(packet.fan_id,packet.sensor_id,packet.mode,packet.manual_array);
  }
  else if(d->data_type == REMOVE_PROFILE){
    struct remove_profile_packet packet;
    parse_data (d, (uint8_t*)&packet, sizeof(struct remove_profile_packet));
    remove_profile_by_id(packet.id);
    delete_data(PROFILE);
    //prova
    set_speed(packet.id,0);
    set_flag_speed_to_zero(packet.id);
    update_eeprom_profile();
  }
  else if(d->data_type == CLEAR_EEPROM){
    eeprom_dirty_clean();
  }
  send_data();
}

int main(void)
{
  uart=UART_init();
  create_sensor_list();
  create_fan_manager();
  init_led_manager();
  create_profile_list();
  init_from_eeprom();
  send_data();
  sei();

  while (1)
  {
    if(UART_getData(uart,(uint8_t*)&d,sizeof(struct data))==1)
      update_data(&d);

    profile_update();
  }
}
