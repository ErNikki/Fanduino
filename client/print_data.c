#include "print_data.h"

char * print_mode_name(uint8_t mode){
  switch (mode) {
    case 1:
      return "manuale";
    case 2:
      return "performance";
    case 3:
      return "silenziosa";
  }
  return "";
}

int is_in_array(const int8_t * arr, size_t size, int8_t dato){
  for(int i = 0;i<size;i++){
    if(arr[i]==dato)
      return 1;
  }
  return 0;
}

void print_sensor (sem_t * serial_sem , linked_list * sensor_linked_list){
  int tmp;
  sem_getvalue(serial_sem,&tmp);
  if(tmp>0) sem_wait(serial_sem);
  node * n = sensor_linked_list->head;
  while(n!=NULL){
    struct sensor_packet * sp;
    sp=n->data;

    printf("SENSOR %d: ",sp->id);
    if(sp->type==1) printf("PIN:%hhu - TIPO SENSORE:%hhu - TEMPERATURA %hhu\n",sp->pin,sp->type,sp->temperature);
    else printf("PIN:%s - TIPO SENSORE:%hhu - TEMPERATURA %hhu\n",get_analog_name(sp->pin),sp->type,sp->temperature);
    n=n->next;
  }
  if(tmp>0) sem_post(serial_sem);
  printf("\n");
}

void print_fan(sem_t * serial_sem , linked_list * fan_linked_list){
  int tmp;
  sem_getvalue(serial_sem,&tmp);
  if(tmp>0)sem_wait(serial_sem);
  node * n = fan_linked_list->head;
  while(n!=NULL){
    struct fan_packet * fp;
    fp=n->data;
    printf("VENTOLA %d: PWM PIN:%hhu - ENCODER PIN:%hhd - VELOCITÀ %hhu\n",fp->id,fp->pwm_pin,fp->encoder_pin,fp->speed);
    n=n->next;
  }
  if(tmp>0)sem_post(serial_sem);
  printf("\n");
}

void print_led(sem_t * serial_sem , linked_list * led_linked_list){
  int tmp;
  sem_getvalue(serial_sem,&tmp);
  if(tmp>0) sem_wait(serial_sem);
  node * n = led_linked_list->head;
  while(n!=NULL){
    struct rgb_led_packet * led_packet;
    led_packet=n->data;
    printf("LED %hhu: PIN ROSSO:%hhu - PIN VERDE:%hhu - PIN BLU:%hhu - COLORE %s\n",led_packet->id,led_packet->red_pin,
                        led_packet->green_pin,led_packet->blue_pin,get_color_name(led_packet->current_color));
    n=n->next;
  }
  if(tmp>0) sem_post(serial_sem);
  printf("\n");
}

void print_profile(sem_t * serial_sem , linked_list * profile_linked_list){
  sem_wait(serial_sem);
  node * n = profile_linked_list->head;
  while(n!=NULL){
    struct profile_packet * packet;
    packet=n->data;
    printf("PROFILO %hhu: VENTOLA %hhu - CONTROLLATA DA %hhu - MODALITÀ %s\n",packet->id,packet->id,packet->pin,print_mode_name(packet->mode));
    if(packet->mode==1){
      for(int i = 0;i<10;i++){
        printf("Percentuale a %d °C - %hhu\n",(i + 1) * 10,packet->perc_x_temp[i]);
      }
    }
    n=n->next;
  }
  sem_post(serial_sem);
  printf("\n");
}

int is_pin_used(const int8_t * arr, int8_t * arr_used, size_t size, int8_t dato){
  int i;
  for(i=0;i<size;i++){
    if(arr[i]==dato)
      break;
  }

  return arr_used[i]==1;
}

char * get_analog_name(uint8_t pin){
  if(pin >97 || pin < 82)return 0;
  pin = 97-pin;
  char * out = malloc(sizeof(char)*4);
  out[0]='A';
  sprintf(out+1, "%d", pin);
  return out;
}

uint8_t get_analog_pin(char * name){
  if(name==0)return 0;
  int ris=0;
  if('0'<=name[1] && name[1]<='9' && (name[2]==0 || ('0'<=name[2] && name[2]<='9'))){
    ris=atoi(name+1);
    ris = 97-ris;
  }
  return ris;
}
