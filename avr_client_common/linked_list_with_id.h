#ifndef LINKED_LIST_WITH_ID
#define LINKED_LIST_WITH_ID

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct node_s
{
    void *data;
    struct node_s *next;
    struct node_s *prev;
} node;

typedef struct linked_list_s
{
    struct node_s *head;
} linked_list;


node *linked_list_get_node(linked_list *list, uint8_t id);
/*
ritorna il puntatore al nodo che ha come id quello passato
nel caso in cui non sia presente ritorna NULL
nel caso in cui la lista sia NULL torna NULL
*/

void *linked_list_get_data(linked_list *list, uint8_t id);
/*
ritorna il puntatore al data che ha come id quello passato
nel caso in cui non sia presente ritorna NULL
*/

linked_list *linked_list_init(void);
/*
inizializza la lista con il puntore alla testa NULL
ritorna il puntatore alla lista inizializzata.
la memoria viene allocata dinamicamente
*/

void linked_list_delete(linked_list *list, uint8_t flag, uint8_t clear_list_info);
/*
Se il flag è messo ad 1 libera anche le strutture data presenti nei nodi
elimina completamente la lista passata deallocando la memoria
nel caso in cui la lista sia NULL non fa nulla
clear_list_info se 1 rimuove anche la struttura per gestire la lista

*/

void linked_list_add_node(linked_list *list, void *data);
/*
aggiunge una struttura alla lista data
la memoria viene allocata dinamicamente
controlla che la lista sia allocata
non aggiunge nodi con un id già presente
*/

void linked_list_remove_node(linked_list *list, uint8_t id, uint8_t flag);
/*
Se il flag è messo ad 1 libera anche le strutture data presenti nei nodi
aggiunge una struttura alla lista data
rimuove un nodo dalla lista che ha come id quello passato,
deallocando la memoria
controlla che il nodo sia presente
*/

void linked_list_printf(linked_list *list);
/*
stampa la lista collegata controllando che sia allocata
*/

void * linked_list_get_head_data(linked_list * list);

int linked_list_size(linked_list * list);

#endif
