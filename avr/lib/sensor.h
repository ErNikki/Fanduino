#ifndef SENSOR_H
#define SENSOR_H

#include <stdlib.h>
#include <util/delay.h>
#include "port.h"
#include "../../avr_client_common/linked_list_with_id.h"
#include "fan.h"
#include <util/atomic.h>

#define TINUDHT_ACK_TIMEOUT 160 // @ 1 MHz: 10.. (no less than that)
// NOTE: This should change with the F_CPU change.
#define TINUDHT_BITLEN 7 // @ 1 MHz: 3..7
// NOTE: This should change with the F_CPU change.

#define DHT11 1
#define LM35DZ 2
#define MPC9700A 3

//il valore pin verrà considerato come id
typedef struct sensor_s
{
  uint8_t pin;
  uint8_t type;
} sensor;


int dht11_temperature(uint8_t pin);

int lm35dz_temperature(uint8_t pin);

int mcp9700a_temperature(uint8_t pin);

void create_sensor_list(void);
/*
crea una lista collegata contentente il tipo sensore,
controllando che la lista non sia stata già allocata.
*/

void delete_sensor_list(void);
/*
elimina la lista collegata contentente il tipo sensore,
controllando che la lista sia allocata.
la dealloca anche nel caso sia piena di sensori!!!
*/

int8_t add_sensor(uint8_t pin, uint8_t type);
/*
aggiuge alla lista un oggetto di tipo sensore,
allocato tramite malloc, di cui vengono specificate
il pin in cui è collegato e il tipo di sensore.
nel caso in cui abbia successo ritorna il numero di in
nel caso in cui il pin venga già utilizzato ritorna-1
nel caso in cui il tipo non sia tra quelli supportati ritorna -2
*/

void remove_sensor(uint8_t sensor_id);
/*
rimuove dalla lista un oggetto di tipo sensore,
controllando che il pin sia presente nella lista.
nel caso in cui non lo sia ritorna -1
*/

int get_temperature(uint8_t sensor_id);
/*
legge dalla lista collegata il tipo del pin passato
e restituisce la temperatura a seconda del tipo di sensore
ritorna un numero negativo in caso di errore
ritorna -1 nel caso il pin non venga utilizzato
ritorna -2 in caso di errore nella scelta del sensore
*/
void dump_sensor(struct sensor_packet * data);
/*
  Riempe la struttura passata come parametro con tutte le informazioni
  sui sensori collegati
*/
int get_sensor_number(void);

uint8_t sensor_is_setted(uint8_t pin);
/*
ritorna 1 se il sensore è settato
0 se non è settato
*/
node* get_sensor_list_head(void);

#endif
