#ifndef FAN_H
#define FAN_H

#include "port.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include "../avr_client_common/packets.h"

#define FAN_NUMBER 16

#define MAX_FAN_CONNECTED -2
#define FAN_MANAGER_NOT_INITIALIZED -3
#define PWM_PIN_AND_FAN_SPEED_EQUALS -4
#define PWM_PIN_WRONG -5
#define ENCODER_PIN_WRONG -6
#define FAN_ALREADY_CONNECTED -7

struct fan {
  int8_t id;
  int8_t pwm_pin;
  int8_t encoder_pin;
  uint8_t speed;

  // Elementi privati per il funzionamente della libreria
  int8_t interrupt_id;
  uint8_t encoder_tick;
} fan;

/*
  fan_pwm_pin deve essere connesso a un pin pwm, che secondo le specifiche
  di port.h sono 2,3,4,5,6,7,8,9,10,11,12,13,44,45,46

  fan_encoder_pin è opzionale serve per leggere informazioni sullo stato della
  ventola, se la vostra ventola lo ha, vi consigliamo di collegarlo. Deve essere
  collegato nei pin 2,3,18,19,20,21
*/

void create_fan_manager(void); // Crea un fan manager
void delete_fan_manager(void); // Elimina il fan manager

int8_t add_fan(int8_t pwm_pin, int8_t fan_encoder_pin);
/*
  Aggiungi una ventola con il pin pwm collegato a pwm_pin e il pin dell'encoder
  collegato a fan_encoder_pin.

  In caso di successo restituisci un indetificatore della ventola.
  Mentre in caso di errore restituisce:
  -2 Numero massimo di ventole già collegate
  -3 fan_manager not created
  -4 pwm_pin e fan_pin diversi
*/
void remove_fan(int8_t fan_id);
// Rimuove un ventola

void set_speed(int8_t fan_id, uint8_t rate);
/* Imposta la velocita della ventole, rate deve essere espresso in percentuale
 */

int8_t get_interrupt(int8_t pin);
/*
Ritorna il numero del interrupt corrispondete al pin
Se il pin non ha un interrupt ritorna -1
*/

int is_in_array(const int8_t * arr, size_t size, int8_t dato);

int get_fan_number(void);
/*
  Get connected fan
*/

void dump_fan(struct fan_packet * data);
/*
  Riempe la struttura passata come parametro con tutte le informazioni
  sulle ventole collegate
*/
void stop_all(void);
void start_all(void);
void busrt_all(uint8_t delta);
struct fan** get_fan_manager(void);
struct fan * get_fan(uint8_t index);
uint8_t get_fan_index(uint8_t id);
uint8_t fan_is_setted(uint8_t id_fan);
#endif
