#ifndef PROFILAZIONE_H
#define PROFILAZIONE_H

#include <stdio.h>
#include <stdlib.h>
#include "linked_list_with_id_and_pin.h"
#include "sensor.h"
#include "fan.h"
#include <inttypes.h>
#include <stdint.h>

#define MANUAL 1
#define PERFORMANCE 2
#define SILENT 3

struct profile
{
    uint8_t id_fan;
    uint8_t pin_sensor;
    uint8_t mode;
    uint8_t perc_x_temp[10];
};

//TODO: da commentare
void create_profile_list(void);
/*
inizializza la lista di profili
*/

void delete_profile_list(void);
/*
rimuove completamente la lista di profili
dealloando le varie strutture data che vengono
negli add profile
*/

void add_profile(uint8_t id_fan, uint8_t pin, uint8_t mode, uint8_t * manual_profile);
/*
aggiunge un profilo tenendo conto del fatto che sia la ventola
che il sensore sia stati già settati
viene creata una struttura data allocata dinamicamente
*/

void remove_profile_by_id(uint8_t id);
void remove_profile_by_pin(uint8_t pin);
/*
rimuovono il profilo deallocando la struttura data
creata nell'add profile
*/

uint8_t line_between_two_point(uint8_t x, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);
/*
costruisce la funzione tra due punti
e ne ritorna l'ascissa in funzione della x passata
*/

/*
cheide all'utente le percentuali di potenza delle ventole che preferisce
in base alle temperature che gli vengono chieste
*/

uint8_t get_percentage_by_profile(uint8_t temp, struct profile *profile);
uint8_t get_percentage_by_id(uint8_t temp, uint8_t id);
/*
restituiscono la percentuale di potenza delle ventole
in base alla variabile passata
*/

int get_profile_number(void);
/*
restituisce il numero di profili creati
*/

void dump_profile(struct profile_packet * data);
/*
crea la struttura dati che verrà poi inviata al pc
*/

void profile_update(void);
/*
  Aggiorna la velocita di tutte le ventole in base alla temperatura corrente
  E il profilo impostato
*/

node_pi* get_profile_list_head(void);

#endif
