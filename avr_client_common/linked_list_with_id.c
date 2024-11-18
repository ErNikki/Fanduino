#include "linked_list_with_id.h"

node *linked_list_get_node(linked_list *list, uint8_t id)
{
    if (list == NULL)
    {
        return NULL;
    }

    node *node = list->head;

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

void *linked_list_get_data(linked_list *list, uint8_t id)
{

    node *node = linked_list_get_node(list, id);
    if (node == NULL)
    {
        return NULL;
    }
    return node->data;
}

linked_list *linked_list_init(void)
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->head = NULL;
    return list;
}

void linked_list_delete(linked_list *list, uint8_t flag, uint8_t clear_list_info)
{
    if (list == NULL)
    {
        return;
    }

    node *node = list->head;
    list->head=NULL;
    if(clear_list_info)
      free(list);

    struct node_s *next_node;

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



void linked_list_add_node(linked_list *list, void *data)
{

    if (list == NULL || data == NULL)
    {
        return;
    }

    uint8_t *id_n = (uint8_t *)data;

    if (linked_list_get_node(list, *id_n) != NULL)
    {
        return;
    }

    node *new_element = (node *)malloc(sizeof(node));
    new_element->next = NULL;
    new_element->prev = NULL;
    new_element->data = data;

    if (list->head == NULL)
    {
        list->head = new_element;
    }
    else
    {
        node *node = list->head;
        while (node->next != NULL)
        {
            node = node->next;
        }
        node->next = new_element;
        new_element->prev = node;
    }
}

void linked_list_remove_node(linked_list *list, uint8_t id,uint8_t flag)
{

    if (list == NULL || list->head == NULL)
    {
        return;
    }

    node *node = linked_list_get_node(list, id);
    struct node_s *prev_node = NULL;
    struct node_s *next_node = NULL;

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

void linked_list_printf(linked_list *list)
{

    if (list == NULL)
    {

        printf("no list\n");
        return;
    }

    node *node = list->head;

    while (node != NULL)
    {
        uint8_t *data = node->data;
        printf("id:%d", *data);
        printf("\t");
        node = node->next;
    }

    printf("\n");
}

void *linked_list_get_head_data(linked_list *list)
{
    if (list->head != NULL)
        return list->head->data;
    else
        return NULL;
}

int linked_list_size(linked_list *list)
{

    int k = 0;

    if (list == NULL)
        return k;

    struct node_s *n = list->head;
    while (n != NULL)
    {
        k++;
        n = n->next;
    }

    return k;
}
