#include "led.h"

linked_list *list = NULL;

void init_led_manager(void) {
  if(list == NULL)
    list = linked_list_init();
 }

//TODO: vedi se ti piace di più cosi
void delete_led_manager(void) {
  /*
  while (linked_list_get_head_data(list) != NULL) {
    struct led * l = linked_list_get_head_data(list);
    if(l!=NULL)remove_rgb_led(l->red_pin,1);
  }*/

  if(list != NULL)
  {
    linked_list_delete(list,1,1);
  }
  list = NULL;

}

uint8_t add_rgb_led(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) {

  struct led * l = malloc(sizeof(struct led));
  l->red_pin = red_pin;
  l->green_pin = green_pin;
  l->blue_pin = blue_pin;
  l->current_color = TURNOFF;

  set_mode(red_pin, OUTPUT);
  set_mode(green_pin, OUTPUT);
  set_mode(blue_pin, OUTPUT);

  linked_list_add_node(list, (void *)l);
  return red_pin;
}

void remove_rgb_led(uint8_t id) {
  set_color(id, TURNOFF);
  linked_list_remove_node(list, id,1);
}

int get_led_number(void){
  return linked_list_size(list);
}

void dump_led(struct rgb_led_packet * data){
  node * n = list->head;
  while (n!=NULL) {
    struct led * l = n->data;
    data->id = l->red_pin;
    data->red_pin = l->red_pin;
    data->green_pin = l->green_pin;
    data->blue_pin = l->blue_pin;
    data->current_color = l->current_color;
    data++;
    n=n->next;
  }
}


void set_color(uint8_t id, uint8_t color) {

  struct led * l = linked_list_get_data(list, id);
  if( l == NULL )return;
  l->current_color=color;
  switch (color) {
    case TURNOFF:
      set_level(l->red_pin, LOW);
      set_level(l->green_pin, LOW);
      set_level(l->blue_pin, LOW);
      break;
    case RED:
      set_level(l->red_pin, HIGH);
      set_level(l->green_pin, LOW);
      set_level(l->blue_pin, LOW);
      break;
    case GREEN:
      set_level(l->red_pin, LOW);
      set_level(l->green_pin, HIGH);
      set_level(l->blue_pin, LOW);
      break;
    case BLUE:
      set_level(l->red_pin, LOW);
      set_level(l->green_pin, LOW);
      set_level(l->blue_pin, HIGH);
      break;
    case YELLOW:
      set_level(l->red_pin, HIGH);
      set_level(l->green_pin, HIGH);
      set_level(l->blue_pin, LOW);
      break;
    case AZURE:
      set_level(l->red_pin, LOW);
      set_level(l->green_pin, HIGH);
      set_level(l->blue_pin, HIGH);
      break;
    case MAGENTA:
      set_level(l->red_pin, HIGH);
      set_level(l->green_pin, LOW);
      set_level(l->blue_pin, HIGH);
      break;
    case WHITE:
      set_level(l->red_pin, HIGH);
      set_level(l->green_pin, HIGH);
      set_level(l->blue_pin, HIGH);
      break;
    default:
      break;
  }
}

node* get_led_list_head(void){
    if(list!=NULL){
      return list->head;
    }
    return NULL;
}