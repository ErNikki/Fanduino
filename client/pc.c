#include "serial_linux.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "../avr_client_common/linked_list_with_id.h"
#include "../avr_client_common/color.h"
#include "comunication.h"
#include "print_data.h"

int8_t used_digital_pin[54]={0};
int8_t used_analog_pin[16]={0};
struct data * d;
sem_t * serial_sem;

linked_list * sensor_linked_list;
linked_list * fan_linked_list;
linked_list * led_linked_list;
linked_list * profile_linked_list;

void * update_data(void * par){

  int fd = *(int*)par;

  while (1){
    //printf("Ho chiesto i dati\n");

    serial_read(fd,(uint8_t*)d,sizeof(struct data));
    sem_wait(serial_sem);

    if(d->data_type==SENSOR) {
      struct sensor_packet * sp = malloc(sizeof(struct sensor_packet));
      parse_data(d,(uint8_t*)sp,sizeof(struct sensor_packet));
      if(sp!=NULL && sp->type!=0 && sp->pin!=0){
        if(linked_list_get_node(sensor_linked_list,sp->id)!=NULL)
          linked_list_remove_node(sensor_linked_list,sp->id,1);
        linked_list_add_node(sensor_linked_list,(void *)sp);
        if(sp->type==1)used_digital_pin[sp->pin]=1;
        else used_analog_pin[sp->pin-82]=1;
      }
    }

    else if (d->data_type==FAN) {
      struct fan_packet * fp = malloc(sizeof(struct fan_packet));
      parse_data(d,(uint8_t*)fp,sizeof(struct fan_packet));
      if(fp!=NULL && fp->pwm_pin!=0){
        if(linked_list_get_node(fan_linked_list,fp->id)!=NULL)
          linked_list_remove_node(fan_linked_list,fp->id,1);
        linked_list_add_node(fan_linked_list,(void *)fp);

        used_digital_pin[fp->pwm_pin]=1;
        if(fp->encoder_pin!=-1)
          used_digital_pin[fp->encoder_pin]=1;
      }

    }

    else if (d->data_type==RGB_LED) {
      struct rgb_led_packet * led_packet = malloc(sizeof(struct rgb_led_packet));
      parse_data(d,(uint8_t*)led_packet,sizeof(struct rgb_led_packet));
      if(led_packet !=NULL && led_packet->red_pin != 0 && led_packet->green_pin != 0 && led_packet->blue_pin != 0){
        if(linked_list_get_node(led_linked_list,led_packet->id)!=NULL)
          linked_list_remove_node(led_linked_list,led_packet->id,1);
        linked_list_add_node(led_linked_list,(void *)led_packet);

        used_digital_pin[led_packet->red_pin]=1;
        used_digital_pin[led_packet->green_pin]=1;
        used_digital_pin[led_packet->blue_pin]=1;
      }
    }

    else if (d->data_type==PROFILE) {
      struct profile_packet * packet = malloc(sizeof(struct profile_packet));
      parse_data(d,(uint8_t*)packet,sizeof(struct profile_packet));
      if(packet !=NULL && packet->id != 0 && packet->pin!=0){
        if(linked_list_get_node(profile_linked_list,packet->id)!=NULL)
          linked_list_remove_node(profile_linked_list,packet->id,1);
        linked_list_add_node(profile_linked_list,(void *)packet);
      }
    }

    else if(d->data_type==DELETED_DATA){
      struct deleted_data packet;
      parse_data(d,(uint8_t*)&packet,sizeof(struct deleted_data));
      switch (packet.type) {
        case RGB_LED:
          linked_list_delete(led_linked_list,1,0);
        break;
        case SENSOR:
          linked_list_delete(sensor_linked_list,1,0);
        break;
        case FAN:
          linked_list_delete(fan_linked_list,1,0);
        break;
        case PROFILE:
          linked_list_delete(profile_linked_list,1,0);
        break;
      }
      memset(used_digital_pin,0,54);
      memset(used_analog_pin,0,16);;
    }
    sem_post(serial_sem);
  }

}

void * print_information_thread(void * arg){
  int * data = (int *)arg;
  int * checker = data;
  int fd = data[1];
  while(*checker){
    printf("\e[1;1H\e[2J");

    print_led(serial_sem,led_linked_list);
    print_fan(serial_sem,fan_linked_list);
    print_sensor(serial_sem,sensor_linked_list);
    print_profile(serial_sem,profile_linked_list);


    printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
    serial_putData(fd,NULL,0,GET_DATA);
    usleep(1000*1000);
  }
  pthread_exit(NULL);
}


void print_information(int fd){

  int data[2] = {1,fd};
  pthread_t information_thread;
  pthread_create(&information_thread,NULL,print_information_thread,(void*)data);
  while(getchar()!='\n');
  getchar();
  data[0]=0;
  pthread_join(information_thread,NULL);


  /*
  print_led(serial_sem,led_linked_list);
  print_fan(serial_sem,fan_linked_list);
  print_sensor(serial_sem,sensor_linked_list);
  print_profile(serial_sem,profile_linked_list);
  while(getchar()!='\n');
  getchar();
  */



}


int main(int argc, char *argv[]){

    int fd = serial_open("/dev/ttyACM0");
    if(fd<0){
      printf("FanDuino non collegato\n");
      return -1;
    }

    serial_set_interface_attribs(fd, 57600, 0);
    serial_set_blocking(fd,1);
    sleep(1);
    // NON SO PERCHé ma se non aspetti del tempo non funziona la seriale

    d = malloc(sizeof(struct data));
    serial_sem = malloc(sizeof(sem_t));

    sem_init(serial_sem,0,1);

    sensor_linked_list=linked_list_init();
    fan_linked_list=linked_list_init();
    led_linked_list=linked_list_init();
    profile_linked_list=linked_list_init();

    pthread_t serial_thread;
    pthread_create(&serial_thread,NULL,update_data,(void*)&fd);

    int choose;

    printf("Syncing...");
    for(int i = 0;i<50;i++){
      printf(".");
      serial_putData(fd,NULL,0,GET_DATA);
    }
    printf("\n");
    printf("\e[1;1H\e[2J");

    while (1) {

      printf("\e[1;1H\e[2J");
      printf("Premi 0 per visualizzare le informazioni\n");
      printf("VENTOLE\n");
      printf("\tPremi 1 per aggiungere un ventola\n");
      printf("\tPremi 2 per impostare la velocita di una ventola\n");
      printf("\tPremi 3 per eliminare una ventola\n");
      printf("LED\n");
      printf("\tPremi 4 per aggiungere un led\n");
      printf("\tPremi 5 per impostare il colore di un led\n");
      printf("\tPremi 6 per eliminare un led\n");
      printf("SENSORI\n");
      printf("\tPremi 7 per aggiungere un sensore\n");
      printf("\tPremi 8 per rimuovere un sensore\n");
      printf("PROFILI\n");
      printf("\tPremi 9 per aggiungere un profilo\n");
      printf("\tPremi 10 per rimuovere un profilo\n");

      printf("\tPremi 11 per pulire la eeprom\n");
      serial_putData(fd,NULL,0,GET_DATA);
      scanf("%d",&choose);
      serial_putData(fd,NULL,0,GET_DATA);
      printf("\e[1;1H\e[2J");

      switch (choose) {
        case 0:
          print_information(fd);
        break;
        case 1:
          add_fan(fd, serial_sem, used_digital_pin);
        break;
        case 2:
          set_fan_speed(fd,serial_sem,fan_linked_list);
        break;
        case 3:
          delete_fan(fd,serial_sem,fan_linked_list, used_digital_pin);
        break;
        case 4:
          add_led(fd,serial_sem,used_digital_pin);
        break;
        case 5:
          set_led_color(fd,serial_sem,led_linked_list);
        break;
        case 6:
          delete_led(fd,serial_sem,led_linked_list, used_digital_pin);
        break;
        case 7:
          add_sensor(fd,used_digital_pin,used_analog_pin,serial_sem);
        break;
        case 8:
          delete_sensor(fd, serial_sem, sensor_linked_list, used_digital_pin, used_analog_pin);
        break;
        case 9:
          add_profile(fd,serial_sem,fan_linked_list,sensor_linked_list);
        break;
        case 10:
          remove_profile(fd,serial_sem,profile_linked_list);
        break;
        case 11:
          printf("Cancellazione EEPROM\n");
          sleep(1);
          serial_putData(fd,NULL,0,CLEAR_EEPROM);
          printf("EEPROM cancellata\n");
          printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
          getchar();
          while(getchar()!='\n')
        break;
        default:
          printf("Opzione non disponile!\n");
        break;
      }

    }
    close(fd);
    return 0;



}
