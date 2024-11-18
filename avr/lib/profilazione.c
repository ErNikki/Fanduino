
#include "profilazione.h"

//TODO i profili sono da sistemare 10  20  30  40  50  60  70  80  90  100
const uint8_t performance_profile[10] = {30, 40, 50, 50, 50, 70, 70, 80, 80, 100};
const uint8_t silent_profile[10] =      {0 ,  0,  0, 20, 30, 40, 50, 70, 80, 100};
linked_list_pi *profile_list = NULL;

void create_profile_list(void)
{
    if (profile_list == NULL)
    {
        profile_list = linked_list_pi_init();
    }
}

void delete_profile_list(void)
{

    if (profile_list != NULL)
    {
        linked_list_pi_delete(profile_list, 1);
    }
    profile_list = NULL;
}

//TODO: da testare l'if riga 25 il resto funziona
void add_profile(uint8_t id_fan, uint8_t pin, uint8_t mode, uint8_t * manual_profile)
{
    if (sensor_is_setted(pin) && (linked_list_pi_get_node_by_id(profile_list, id_fan) == NULL) && fan_is_setted(id_fan))
    {
        struct profile *profile = (struct profile *)malloc(sizeof(struct profile));
        profile->id_fan = id_fan;
        profile->pin_sensor = pin;
        profile->mode = mode;
        memcpy(profile->perc_x_temp,manual_profile,10);
        //for(uint8_t i = 0;i<10;i++)
        //  profile->perc_x_temp[i]=manual_profile[i];
        linked_list_pi_add_node(profile_list, profile);
    }
}

void remove_profile_by_id(uint8_t id)
{
    node_pi *node = linked_list_pi_get_node_by_id(profile_list, id);
    if (node != NULL)
    {
      linked_list_pi_remove_node_by_id(profile_list, id, 1);
    }
}

void remove_profile_by_pin(uint8_t pin)
{
    node_pi *node = linked_list_pi_get_node_by_pin(profile_list, pin);
    while (node != NULL)
    {
        linked_list_pi_remove_node_by_pin(profile_list, pin, 1);
        node = linked_list_pi_get_node_by_pin(profile_list, pin);
    }
}
uint8_t line_between_two_point(uint8_t x, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{

    return ((x - x1) * (y2 - y1) / (x2 - x1)) + y1;
}

//TODO é sistemata ma è fatta da pc non da arduino
//la struttura vai poi comunicata ad arduino

//TODO: andrebbe riprovata
uint8_t get_percentage_by_profile(uint8_t temp, struct profile *profile)
{

    uint8_t mode = profile->mode;
    uint8_t x;
    uint8_t index;
    uint8_t y1;
    uint8_t y2;
    uint8_t x1;
    uint8_t x2;
    uint8_t * perc_x_temp;
    if(temp<=10)return 0;
    switch (mode)
    {
    case MANUAL:

        perc_x_temp = profile->perc_x_temp;
        x = temp;
        index = (temp / 10);
        y1 = perc_x_temp[index - 1];
        y2 = perc_x_temp[index];
        x1 = index * 10;
        x2 = (index + 1) * 10;
        return line_between_two_point(x, x1, x2, y1, y2);
    break;
    case PERFORMANCE:
        x = temp;
        index = (temp / 10);
        y1 = performance_profile[index - 1];
        y2 = performance_profile[index];
        x1 = index * 10;
        x2 = (index + 1) * 10;
        return line_between_two_point(x, x1, x2, y1, y2);
    break;
    case SILENT:

        x = temp;
        index = (temp / 10);
        y1 = silent_profile[index - 1];
        y2 = silent_profile[index];
        x1 = index * 10;
        x2 = (index + 1) * 10;
        return line_between_two_point(x, x1, x2, y1, y2);
    break;
    default:
      return 77;
    break;
    }
    return 200;
}

uint8_t get_percentage_by_id(uint8_t temp, uint8_t id)
{

    struct profile *profile = linked_list_pi_get_data_by_id(profile_list, id);
    return get_percentage_by_profile(temp, profile);
}

int get_profile_number(void)
{
    return linked_list_pi_size(profile_list);
}
//TODO: da rivedere
void dump_profile(struct profile_packet *data)
{
    node_pi *n = profile_list->head;
    while (n != NULL)
    {
        struct profile *s = n->data;
        data->id = s->id_fan;
        data->pin = s->pin_sensor;
        data->mode = s->mode;
        //la struttura va inviata solo se è differente da quelle basic
        // e quindi che è stata scelta dall'utente
        if (s->mode == MANUAL)
        {
            uint8_t * old_perc_x_temp = s->perc_x_temp;
            uint8_t * new_perc_x_temp = data->perc_x_temp;
            for (int i = 0; i < 10; i++)
            {
                new_perc_x_temp[i] = old_perc_x_temp[i];
            }
        }
        data++;
        n = n->next;
    }
}

void profile_update(void){
  if(profile_list==NULL)
    return;
  node_pi * node = profile_list->head;
  struct profile * p;
  while (node!=NULL) {
    p = node->data;

    uint8_t temp = get_temperature(p->pin_sensor);
    uint8_t percentage = get_percentage_by_profile(temp,p);
    set_speed(p->id_fan,percentage);

    node=node->next;
  }
}

node_pi* get_profile_list_head(void){
    if(profile_list!=NULL){
        return profile_list->head;
    }
    return NULL;
}
