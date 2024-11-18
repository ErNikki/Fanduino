#include "comunication.h"

const int8_t pwm_pins[15]={2,3,4,5,6,7,8,9,10,11,12,13,44,45,46};
const int8_t interrupt_pins[7]={2,3,18,19,20,21,-1};

void add_fan(int fd,sem_t * sem, int8_t * used_digital_pin){
  struct add_fan_packet afp;
  printf("Inserisci 0 per tornare al menu precedente\n\n");
  printf("Inserisci il pin del pwm\n");
  sem_wait(sem);

  do{
    printf("Deve essere un pin tra: ");
    for(int i = 0;i<15;i++){
        if(used_digital_pin[pwm_pins[i]]==0)
          printf("%d ",pwm_pins[i]);
    }
    //SBLOCCA
    printf("\n");
    scanf("%hhd",&(afp.pwm_pin));
    if(afp.pwm_pin==0){
      sem_post(sem);
      return;
    }
  }while(!(is_in_array(pwm_pins,15,afp.pwm_pin) && (afp.pwm_pin < 54 && used_digital_pin[afp.pwm_pin]==0)) );

  sem_post(sem);
  printf("Inserisci 0 per tornare al menu precedente\n\n");
  printf("\nInserisci il pin dell'encoder [-1 se non presente]\n");
  sem_wait(sem);

  do{
    printf("Deve essere un pin tra: ");
    for(int i = 0;i<7;i++)
      if(used_digital_pin[interrupt_pins[i]]==0)
        printf("%d ",interrupt_pins[i]);
    printf("\n");
    scanf("%hhd",&(afp.encoder_pin) );
    if(afp.encoder_pin==0){
      sem_post(sem);
      return;
    }
  }while(!(is_in_array(interrupt_pins,7,afp.encoder_pin) && (afp.encoder_pin  < 54
    && (afp.encoder_pin == -1 || used_digital_pin[afp.encoder_pin]==0) && afp.pwm_pin!=afp.encoder_pin)));

  sem_post(sem);

  serial_putData(fd,(uint8_t*)&afp,sizeof(struct add_fan_packet),ADD_FAN);
  printf("DATA INVIATI\n");

  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();
}
void set_fan_speed(int fd, sem_t * sem, linked_list * fan_list){

  struct set_fan_speed_packet packet;
  sem_wait(sem);
  do {
    printf("Inserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci l'id della ventola\nVentole disponibili:\n");
    print_fan(sem,fan_list);

    scanf("%hhd",&(packet.id));
    if(packet.id==0){
      sem_post(sem);
      return;
    }
  }
  while(linked_list_get_node(fan_list,packet.id)==NULL);
  sem_post(sem);

  printf("Inserisci 101 per tornare al menu precedente\n\n");
  printf("Inserisci la velocita desiderata [da 0 a 100]\n");

  scanf("%hhd",&(packet.speed));
  if(packet.speed==101){
      return;
  }
  serial_putData(fd,(uint8_t*)&packet,sizeof(struct set_fan_speed_packet),SET_FAN_SPEED);
  printf("DATA INVIATI\n");

  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();
}

void delete_fan(int fd,sem_t * sem, linked_list * fan_list, int8_t * used_digital_pin){
  if(linked_list_size(fan_list)>0){
    struct remove_fan_packet packet;
    printf("Inserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci l'id della ventola da eliminare\n");

    sem_wait(sem);
    struct fan_packet * p;
    do {
      printf("Ventole disponibili:\n");
      print_fan(sem,fan_list);
      scanf("%hhd",&(packet.id));
      if(packet.id==0){
        sem_post(sem);
        return;
      }
      p = linked_list_get_data(fan_list,packet.id);
    }while(p==NULL);

    sem_post(sem);
    serial_putData(fd,(uint8_t*)&packet,sizeof(struct remove_fan_packet),REMOVE_FAN);
    printf("DATA INVIATI\n");
  }
  else printf("Nessuna ventola trovata\n");
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();
}

//LED
void add_led(int fd,sem_t * sem, int8_t * used_digital_pin){

  struct add_rgb_led_packet packet;
  sem_wait(sem);
  printf("puoi selezionare i seguenti pin:\n");
  for(int i =22 ;i<54;i++){
      if(used_digital_pin[i]==0)
        printf("%d ",i);
      if(i==37){
        printf("\n");
      }
  }

  printf("\nInserisci 0 per tornare al menu precedente\n");
  printf("\n");
  int k = 0;
  do{

    if(k==1) printf("il pin inserito è errato \n");
    k=1;
    printf("Inserisci il pin del rosso\n");
    scanf("%hhd",&(packet.red_pin));
    if(packet.red_pin==0){
      sem_post(sem);
      return;
    }
  }while(!( (packet.red_pin>=22 || packet.red_pin<=53 ) && used_digital_pin[packet.red_pin]==0));

  k=0;

  do{
    //printf("Inserisci 0 per tornare al menu precedente\n\n");
    if(k==1) printf("il pin inserito è errato \n");
    k=1;
    printf("Inserisci il pin del verde\n");
    scanf("%hhd",&(packet.green_pin));
    if(packet.green_pin==0){
      sem_post(sem);
      return;
    }
  }while(!( (packet.green_pin>=22 && packet.green_pin<=53 && packet.red_pin!=packet.green_pin)
    && used_digital_pin[packet.green_pin]==0));

  k=0;

  do{
    //printf("Inserisci 0 per tornare al menu precedente\n\n");
    if(k==1) printf("il pin inserito è errato \n");
    k=1;
    printf("Inserisci il pin del blu\n");
    scanf("%hhd",&(packet.blue_pin));
    if(packet.blue_pin==0){
      sem_post(sem);
      return;
    }
  }while(!( (packet.blue_pin>=22 && packet.blue_pin<=53 && packet.blue_pin!=packet.red_pin
    && packet.blue_pin!=packet.green_pin) && used_digital_pin[packet.blue_pin]==0));

  sem_post(sem);

  serial_putData(fd,(uint8_t*)&packet,sizeof(struct add_rgb_led_packet),ADD_RGB_LED);
  printf("DATA INVIATI\n");
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();
}
void set_led_color(int fd, sem_t * sem, linked_list * list){
  struct set_rgb_led_color_packet packet;
  printf("Inserisci 0 per tornare al menu precedente\n\n");
  printf("Inserisci l'id del led\n");

  //sem_wait(sem);

  if(linked_list_size(list) == 0){
    printf("Non è stato inserito nessun led\n");
    return;
  }
  int k = 0;
  do {

    if(k>0)printf("ID errato, LED non disponile!");
    k++;
    printf("LED disponibili\n");
    print_led(sem,list);
    scanf("%hhd",&(packet.id));
    if(packet.id==0){
      sem_post(sem);
      return;
    }
  }while(linked_list_get_node(list,packet.id)==NULL);
  sem_post(sem);

  printf("Inserisci 8 per tornare al menu precedente\n\n");
  printf("Inserisci numero del colore\n");
  for(uint32_t i = 0 ;i<8;i++){
    printf("%d - %s\n",i,get_color_name((uint8_t)i));
  }
  scanf("%hhd",&(packet.color));
  if(packet.color==8){
    return;
  }

  while(packet.color<0 || packet.color>7){
    printf("\nInserisci 8 per tornare al menu precedente\n\n");
    printf("Colore sbagliato\n");
    printf("Inserisci numero del colore\n");
    for(uint32_t i = 0 ;i<8;i++){
      printf("%d - %s\n",i,get_color_name((uint8_t)i));
    }
    scanf("%hhd",&(packet.color));
    if(packet.color==8){
      return;
    }
  }
  

  serial_putData(fd,(uint8_t*)&packet,sizeof(struct set_rgb_led_color_packet),SET_RGB_LED_COLOR);
  printf("DATA INVIATI\n");
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
  getchar();
  while(getchar()!='\n');
}
void delete_led(int fd, sem_t * sem, linked_list * list, int8_t * used_digital_pin){

  if(linked_list_size(list)>0){
    struct remove_rgb_led_packet packet;

    sem_wait(sem);
    struct rgb_led_packet * p;
    int k = 0;
    do {
      printf("\nInserisci 0 per tornare al menu precedente\n\n");
      if(k==1)printf("Il led selezionato non è disponibile\n");
      k=1;
      printf("Inserisci l'id del led\nLED disponibili\n");
      print_led(sem,list);
      scanf("%hhd",&(packet.id));
      if(packet.id==0){
        sem_post(sem);
        return;
      }
      p = linked_list_get_data(list,packet.id);
    } while(p==NULL);

    serial_putData(fd,(uint8_t*)&packet,sizeof(struct remove_rgb_led_packet),REMOVE_RGB_LED);
    sem_post(sem);
    printf("DATA INVIATI\n");
  }
  else printf("Nessun led trovato\n");

  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
  getchar();
  while(getchar()!='\n');
}

//SENSOR
void add_sensor(int fd, int8_t * used_digital_pin, int8_t * used_analog_pin, sem_t * sem){
  struct add_sensor_packet packet;
  printf("Inserisci 0 per tornare al menu precedente\n\n");
  printf("Inserisci il tipo di sensore\n");
  printf("1 - DHT11\n");
  printf("2 - LM35DZ\n");
  printf("3 - MPC9700A\n");
  scanf("%hhd",&(packet.type));

  if(packet.type==0){
    return;
  }

  while(packet.type<1 || packet.type>3){
    printf("il tipo inserito è errato \n");
    printf("Inserisci il tipo del sensore\n");
    printf("1 - DHT11\n");
    printf("2 - LM35DZ\n");
    printf("3 - MPC9700A\n");
    scanf("%hhd",&(packet.type));
    if(packet.type==0){
    return;
    }
  }

  int k = 0;
  sem_wait(sem);
  if(packet.type == 1){
    const uint8_t lower_limit = 22;
    const uint8_t upper_limit = 53;
    do{
      printf("\nInserisci 0 per tornare al menu precedente\n\n");
      printf("puoi inserire i seguenti pin:\n");
      for(int i = lower_limit ;i<=upper_limit;i++){
          if(used_digital_pin[i]==0) printf("%d ",i);
          if(i==37) printf("\n");
      }
      printf("\n");
      if(k==1) printf("il pin inserito è errato \n");
      k=1;
      printf("Inserisci il pin in cui vuoi collegare il sensore\n");
      scanf("%hhd",&(packet.pin));
      if(packet.pin==0){
        sem_post(sem);
        return;
      }
    } while(!( (packet.pin>=lower_limit || packet.pin<=upper_limit) && used_digital_pin[packet.pin]==0));
  }
  else {
    const uint8_t lower_limit = 82;
    const uint8_t upper_limit = 97;

    do{
      if(k==1) printf("il pin inserito è errato \n");
      k=1;
      printf("\nScrivi esc per tornare al menu precedente\n\n");

      printf("puoi inserire i seguenti pin:\n");
      for(int i = upper_limit ;i>=lower_limit;i--){
          if(used_analog_pin[i-lower_limit]==0) printf("%s ",get_analog_name(i));
          if(i==37) printf("\n");
      }
      printf("\n");
      printf("Inserisci il pin in cui vuoi collegare il sensore\n");
      char buf[32];
      char* esc="esc";
      if(strcmp(buf,esc)==0){
        sem_post(sem);
        return;
      }
      scanf("%s",buf);

      packet.pin = get_analog_pin(buf);
      printf("selezionato %s [%d]\n\n\n",buf,packet.pin );
    } while(  packet.pin<lower_limit || packet.pin>upper_limit ||
              packet.pin == 0 || used_analog_pin[packet.pin-lower_limit]==1 );


  }
  sem_post(sem);

  serial_putData(fd,(uint8_t*)&packet,sizeof(struct add_sensor_packet),ADD_SENSOR);
  printf("DATA INVIATI - Pin: %hhd - tipo: %hhd\n",packet.pin,packet.type);
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();

}
void delete_sensor(int fd, sem_t * sem , linked_list * list, int8_t * used_digital_pin , int8_t * used_analog_pin){
  sem_wait(sem);
  if(linked_list_size(list)>0){
    int k = 0;
    struct remove_sensor_packet packet;
    printf("Inserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci l'id del sensore\n");
    struct sensor_packet * p;
    do {
      if(k==1)printf("ID errato\n");
      k=1;
      printf("Sensori disponibili\n");
      print_sensor(sem,list);
      scanf("%hhd",&(packet.id));
      if(packet.id==0){
        sem_post(sem);
        return;
      }
      p = linked_list_get_data(list,packet.id);
    }while(p==NULL);

    serial_putData(fd,(uint8_t*)&packet,sizeof(struct remove_sensor_packet),REMOVE_SENSOR);
    printf("DATA INVIATI\n");
  }
  else printf("Nessun sensore trovato\n");
  sem_post(sem);
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();
}

//PROFILE
void add_profile(int fd,sem_t * sem, linked_list * fan_list, linked_list * sensor_list){
  struct add_profile_packet packet;
  int k = 0;
  sem_wait(sem);
  do {
    if(k==1)printf("Ventola non esistente\n");
    k=1;
    printf("\nInserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci l'id della ventola\n");
    printf("Ventole disponibili\n");
    print_fan(sem,fan_list);
    scanf("%hhd",&(packet.fan_id));
    if(packet.fan_id==0){
      sem_post(sem);
      return;
    }
  }while(linked_list_get_node(fan_list,packet.fan_id)==NULL);

  k=0;
  do {
    if(k==1)printf("Sensore non esistente\n");
    k=1;
    printf("\nInserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci l'id del sensore\n");
    printf("Sensori disponibili\n");
    print_sensor(sem,sensor_list);
    scanf("%hhd",&(packet.sensor_id));
    if(packet.sensor_id==0){
      sem_post(sem);
      return;
    }
  }while(linked_list_get_node(sensor_list,packet.sensor_id)==NULL);

  sem_post(sem);
  printf("\nInserisci 0 per tornare al menu precedente\n\n");
  printf("Inserisci la modalità\n");
  printf("1 - MANUALE\n");
  printf("2 - PERFORMANCE\n");
  printf("3 - SILENZIOSA\n");
  scanf("%hhd",&(packet.mode));
  if(packet.mode==0){
    return;
  }

  while(packet.mode<1 || packet.mode>3){
    printf("Modalità non corretta\n");
    printf("\nInserisci 0 per tornare al menu precedente\n\n");
    printf("Inserisci la modalità\n");
    printf("1 - MANUALE\n");
    printf("2 - PERFORMANCE\n");
    printf("3 - SILENZIOSA\n");
    scanf("%hhd",&(packet.mode));
    if(packet.mode==0){
      return;
    }
  }

  if(packet.mode==1){
    uint8_t * p = packet.manual_array;
    for (int i = 0; i < 10; i++)
    {
        printf("inserisci la percentuale della velocita della ventola\nche desideri per la temperatura %d\n", (i + 1) * 10);
        scanf("%hhd", p);
        p++;
    }
  }
  else memset(packet.manual_array,0,10);

  serial_putData(fd,(uint8_t*)&packet,sizeof(struct add_profile_packet),ADD_PROFILE);
  printf("DATA INVIATI\n");
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
      while(getchar()!='\n');
      getchar();

}
void remove_profile(int fd, sem_t * serial_sem,linked_list * profile_linked_list){
  if(linked_list_size(profile_linked_list)>0){
    struct remove_profile_packet packet;
    int k = 0;
    do {
      if(k==1)printf("ID non disponile\n");
      k=1;
      printf("\nInserisci 0 per tornare al menu precedente\n\n");
      printf("Inserisci l'id del profilo\n");
      printf("Profili disponibili\n");
      print_profile(serial_sem,profile_linked_list);
      scanf("%hhd",&(packet.id));
      if(packet.id==0){
        return;
      }
    }
    while(linked_list_get_node(profile_linked_list,packet.id)==NULL);

    serial_putData(fd,(uint8_t*)&packet,sizeof(struct remove_profile_packet),REMOVE_PROFILE);
    printf("DATA INVIATI\n");
  }
  else printf("Nessun profilo trovato\n");
  printf("\nPREMI INVIO PER TORNARE AL MENU PRINCIPALE\n");
  getchar();
  while(getchar()!='\n');
}
