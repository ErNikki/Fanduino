
#ifndef LED_H
#define LED_H
#include "port.h"
#include "../../avr_client_common/linked_list_with_id.h"
#include "../../avr_client_common/color.h"
#include "../../avr_client_common/packets.h"

struct led {
  uint8_t red_pin;
  uint8_t green_pin;
  uint8_t blue_pin;
  uint8_t current_color;
} led;

void init_led_manager(void);
void delete_led_manager(void);

uint8_t add_rgb_led(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);
/*
  Aggiungi un led rgb, passandogli i pin del rosso, verde e blu
  Restituisci l'id del led
*/
void remove_rgb_led(uint8_t id);
/*
  Rimuovi un led, passandogli l'id
*/

void set_color(uint8_t id, uint8_t color);
/*
  Imposta un colore al led, passandogli l'id e il colore,
  che deve essere tra quelli predefiniti.
  Se il colore non esiste, non succede nulla
*/

int get_led_number(void);
void dump_led(struct rgb_led_packet * data);
/*
  Riempe la struttura passata come parametro con tutte le informazioni
  sui led collegati
*/

node* get_led_list_head(void);

#endif
