#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef LINKED_LIST_WITH_ID_PIN
#define LINKED_LIST_WITH_ID_PIN

typedef struct node_pi_s
{
    void *data;
    struct node_pi_s *next;
    struct node_pi_s *prev;
} node_pi;

typedef struct linked_list_pi_s
{
    struct node_pi_s *head;
} linked_list_pi;


node_pi *linked_list_pi_get_node_by_id(linked_list_pi *list, uint8_t id);
node_pi *linked_list_pi_get_node_by_pin(linked_list_pi *list, uint8_t pin);
/*
ritorna il puntatore al nodo che ha come id quello passato
nel caso in cui non sia presente ritorna NULL
nel caso in cui la lista sia NULL torna NULL
*/

void *linked_list_pi_get_data_by_id(linked_list_pi *list, uint8_t id);
void *linked_list_pi_get_data_by_pin(linked_list_pi *list, uint8_t id);
/*
ritorna il puntatore al data che ha come id quello passato
nel caso in cui non sia presente ritorna NULL
*/

linked_list_pi *linked_list_pi_init(void);
/*
inizializza la lista con il puntore alla testa NULL
ritorna il puntatore alla lista inizializzata.
la memoria viene allocata dinamicamente
*/


void linked_list_pi_delete(linked_list_pi *list, uint8_t flag);
/*
Se il flag è messo ad 1 libera anche le strutture data presenti nei nodi
elimina completamente la lista passata deallocando la memoria
nel caso in cui la lista sia NULL non fa nulla
*/

void linked_list_pi_add_node(linked_list_pi *list, void *data);
/*
aggiunge una struttura alla lista data
la memoria viene allocata dinamicamente
controlla che la lista sia allocata
non aggiunge nodi con un id già presente
*/

void linked_list_pi_remove_node_by_id(linked_list_pi *list, uint8_t id, uint8_t flag);
void linked_list_pi_remove_node_by_pin(linked_list_pi *list, uint8_t pin, uint8_t flag);
/*
rimuove un nodo dalla lista che ha come id quello passato,
deallocando la memoria
controlla che il nodo sia presente
Se il flag è messo ad 1 libera anche le strutture data presenti nei nodi
*/
void linked_list_pi_printf(linked_list_pi *list);
/*
stampa la lista collegata controllando che sia allocata
*/

void *linked_list_pi_get_head_data(linked_list_pi *list);
int linked_list_pi_size(linked_list_pi *list);

#endif
