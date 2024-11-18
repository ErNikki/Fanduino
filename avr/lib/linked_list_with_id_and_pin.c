#include "linked_list_with_id_and_pin.h"

node_pi *linked_list_pi_get_node_by_pin(linked_list_pi *list, uint8_t pin)
{
    if (list == NULL)
    {
        return NULL;
    }

    node_pi *node = list->head;

    if (node == NULL)
    {
        return NULL;
    }

    uint8_t *pin_n = (node->data) + 1;

    while (*pin_n != pin)
    {
        node = node->next;
        if (node == NULL)
        {
            return NULL;
        }
        pin_n = (node->data) + 1;
    }

    return node;
}

node_pi *linked_list_pi_get_node_by_id(linked_list_pi *list, uint8_t id)
{
    if (list == NULL)
    {
        return NULL;
    }

    node_pi *node = list->head;

    if (node == NULL)
    {
        return NULL;
    }

    uint8_t *id_n = node->data;

    while (*id_n != id)
    {
        node = node->next;
        if (node == NULL)
        {
            return NULL;
        }
        id_n = node->data;
    }

    return node;
}

void *linked_list_pi_get_data_by_pin(linked_list_pi *list, uint8_t pin)
{

    node_pi *node = linked_list_pi_get_node_by_pin(list, pin);
    if (node == NULL)
    {
        return NULL;
    }
    return node->data;
}

void *linked_list_pi_get_data_by_id(linked_list_pi *list, uint8_t id)
{

    node_pi *node = linked_list_pi_get_node_by_id(list, id);
    if (node == NULL)
    {
        return NULL;
    }
    return node->data;
}

linked_list_pi *linked_list_pi_init(void)
{
    linked_list_pi *list = (linked_list_pi *)malloc(sizeof(linked_list_pi));
    list->head = NULL;
    return list;
}

void linked_list_pi_delete(linked_list_pi *list, uint8_t flag)
{
    if (list == NULL)
    {
        return;
    }

    node_pi *node = list->head;
    free(list);

    struct node_pi_s *next_node;

    while (node != NULL)
    {

        next_node = node->next;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        node = next_node;
    }
}

void linked_list_pi_add_node(linked_list_pi *list, void *data)
{

    if (list == NULL || data == NULL)
    {
        return;
    }

    uint8_t *id_n = (uint8_t *)data;

    if (linked_list_pi_get_node_by_id(list, *id_n) != NULL)
    {
        return;
    }

    node_pi *new_element = (node_pi *)malloc(sizeof(node_pi));
    new_element->next = NULL;
    new_element->prev = NULL;
    new_element->data = data;

    if (list->head == NULL)
    {

        list->head = new_element;
    }
    else
    {
        node_pi *node = list->head;
        while (node->next != NULL)
        {
            node = node->next;
        }
        node->next = new_element;
        new_element->prev = node;
    }
}

void linked_list_pi_remove_node_by_id(linked_list_pi *list, uint8_t id, uint8_t flag)
{

    if (list == NULL || list->head == NULL)
    {
        return;
    }

    node_pi *node = linked_list_pi_get_node_by_id(list, id);
    struct node_pi_s *prev_node = NULL;
    struct node_pi_s *next_node = NULL;

    if (node == NULL)
    {
        return;
    }

    //caso rimozione unico elemento
    if (node->prev == NULL && node->next == NULL)
    {

        list->head = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso rimozione in testa
    if (node->prev == NULL)
    {

        list->head = node->next;
        list->head->prev = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso in cosa
    if (node->next == NULL)
    {
        prev_node = node->prev;
        prev_node->next = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso al centro

    prev_node = node->prev;
    next_node = node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    if (flag == 1)
    {
        free(node->data);
    }
    free(node);
}

void linked_list_pi_remove_node_by_pin(linked_list_pi *list, uint8_t pin, uint8_t flag)
{

    if (list == NULL || list->head == NULL)
    {
        return;
    }

    node_pi *node = linked_list_pi_get_node_by_pin(list, pin);
    struct node_pi_s *prev_node = NULL;
    struct node_pi_s *next_node = NULL;

    if (node == NULL)
    {
        return;
    }

    //caso rimozione unico elemento
    if (node->prev == NULL && node->next == NULL)
    {
        list->head = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso rimozione in testa
    if (node->prev == NULL)
    {
        list->head = node->next;
        list->head->prev = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso in cosa
    if (node->next == NULL)
    {
        prev_node = node->prev;
        prev_node->next = NULL;
        if (flag == 1)
        {
            free(node->data);
        }
        free(node);
        return;
    }

    //caso al centro

    prev_node = node->prev;
    next_node = node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    if (flag == 1)
    {
        free(node->data);
    }
    free(node);
}

void linked_list_pi_printf(linked_list_pi *list)
{

    if (list == NULL)
    {

        printf("no list\n");
        return;
    }

    node_pi *node = list->head;

    while (node != NULL)
    {
        uint8_t *data = node->data;
        printf("id:%d\t", *data);
        printf("pin:%d", *(data + 1));
        printf("\t");
        node = node->next;
    }

    printf("\n");
}

void *linked_list_pi_get_head_data(linked_list_pi *list)
{
    if (list->head != NULL)
        return list->head->data;
    else
        return NULL;
}

int linked_list_pi_size(linked_list_pi *list)
{

    int k = 0;

    if (list == NULL)
        return k;

    struct node_pi_s *n = list->head;
    while (n != NULL)
    {
        k++;
        n = n->next;
    }

    return k;
}
