#include "eeprom_manager.h"

//EEPROM dedicata ai pin va da 2 a 133
//peso 2 byte
//posso inserire 66 sensori
//l'indirizzo del counter si trova in 0
uint16_t start_addr_for_sensors = 2;
uint16_t addr_for_counter_sensors = 0;
uint16_t counter_eeprom_sensors = 0;
const uint16_t offset_for_sensors = 2;

//EEPROM dedicata ai fan va da 136 a 199
//peso 4 byte
//posso inserire 16 fan
////l'indirizzo del counter si trova in 134
uint16_t start_addr_for_fans = 136;
uint16_t addr_for_counter_fans = 134;
uint16_t counter_eeprom_fans = 0;
const uint16_t offset_for_fans = 4;

//EEPROM dedicata ai led  va da 202 a 401
//peso 4 byte
//posso inserire 50 led
////l'indirizzo del counter si trova in 200
uint16_t start_addr_for_leds = 202;
uint16_t addr_for_counter_leds = 200;
uint16_t counter_eeprom_leds = 0;
const uint16_t offset_for_leds = 4;

//EEPROM dedicata ai profili inizia in 404
//ed ogni struct di tipo profile occupa 13 bytes
//l'indirizzo del counter si trova in 402
uint16_t start_addr_for_profiles = 404;
uint16_t addr_for_counter_profiles = 402;
uint16_t counter_eeprom_profiles = 0;
const uint16_t offset_for_profiles = 13;

//TODO: controllo non eccedere memoria
void add_sensor_to_eeprom(uint8_t pin, uint8_t type)
{
  if (counter_eeprom_sensors > 65)
  {
    return;
  }

  uint16_t current_addr = counter_eeprom_sensors * offset_for_sensors + start_addr_for_sensors;
  struct sensor_eeprom_s new;
  new.pin = pin;
  new.type = type;

  eeprom_update_block(&new, (void *)current_addr, sizeof(struct sensor_eeprom_s));
  counter_eeprom_sensors++;
  eeprom_update_word((uint16_t *)addr_for_counter_sensors, counter_eeprom_sensors);
}

void add_fan_to_eeprom(uint8_t pwm_pin, int8_t encoder_pin)
{
  if (counter_eeprom_fans > 15)
  {
    return;
  }
  uint16_t current_addr = counter_eeprom_fans * offset_for_fans + start_addr_for_fans;
  struct fan_eeprom_s new;
  new.pwm_pin = pwm_pin;
  new.encoder_pin = encoder_pin;

  eeprom_update_block(&new, (void *)current_addr, sizeof(struct fan_eeprom_s));
  counter_eeprom_fans++;
  eeprom_update_word((uint16_t *)addr_for_counter_fans, counter_eeprom_fans);
}

void add_led_to_eeprom(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue)
{
  if (counter_eeprom_leds > 49)
  {
    return;
  }
  uint16_t current_addr = counter_eeprom_leds * offset_for_leds + start_addr_for_leds;
  struct led_eeprom_s new;
  new.pin_red = pin_red;
  new.pin_green = pin_green;
  new.pin_blue = pin_blue;
  new.pin_color = 0;

  eeprom_update_block(&new, (void *)current_addr, sizeof(struct led_eeprom_s));
  counter_eeprom_leds++;
  eeprom_update_word((uint16_t *)addr_for_counter_leds, counter_eeprom_leds);
}

void add_profile_to_eeprom(uint8_t fan_id, uint8_t sensor_pin, uint8_t mode, uint8_t * perc_x_temp)
{

  if (counter_eeprom_profiles > 15)
  {
    return;
  }
  uint16_t current_addr = counter_eeprom_profiles * offset_for_profiles + start_addr_for_profiles;
  struct profile_eeprom_s new;
  new.sensor_pin = sensor_pin;
  new.fan_id = fan_id;
  new.mode = mode;

  if (mode == MANUAL)
  {
    memcpy( new.perc_x_temp, perc_x_temp, 10);
  }
  eeprom_update_block(&new, (void *)current_addr, sizeof(struct profile_eeprom_s));
  counter_eeprom_profiles++;
  eeprom_update_word((uint16_t *)addr_for_counter_profiles, counter_eeprom_profiles);
  set_flag_speed_to_zero(fan_id);
}

void add_sensors_from_eeprom(void)
{
  struct sensor_eeprom_s sensors;
  counter_eeprom_sensors = eeprom_read_word((uint16_t *)addr_for_counter_sensors);
  uint16_t current_addr;
  for (int i = 0; i < counter_eeprom_sensors; i++)
  {

    current_addr = i * offset_for_sensors + start_addr_for_sensors;
    eeprom_read_block(&sensors, (void *)current_addr, sizeof(struct sensor_eeprom_s));
    add_sensor(sensors.pin, sensors.type);
  }
}

void add_fans_from_eeprom(void)
{
  struct fan_eeprom_s fans;
  counter_eeprom_fans = eeprom_read_word((uint16_t *)addr_for_counter_fans);
  uint16_t current_addr;
  for (int i = 0; i < counter_eeprom_fans; i++)
  {
    current_addr = i * offset_for_fans + start_addr_for_fans;
    eeprom_read_block(&fans, (void *)current_addr, sizeof(struct fan_eeprom_s));
    //da controllare che sia effetivamente cosi
    add_fan(fans.pwm_pin, fans.encoder_pin);
    if(fans.speed_flag==1){
      set_speed(fans.pwm_pin,fans.speed);
    }
  }

}

void add_leds_from_eeprom(void)
{

  struct led_eeprom_s leds;
  counter_eeprom_leds = eeprom_read_word((uint16_t *)addr_for_counter_leds);
  uint16_t current_addr;
  for (int i = 0; i < counter_eeprom_leds; i++)
  {
    current_addr = i * offset_for_leds + start_addr_for_leds;
    eeprom_read_block(&leds, (void *)current_addr, sizeof(struct led_eeprom_s));
    //da controllare che sia effetivamente cosi
    uint8_t pin_red = leds.pin_red;
    uint8_t pin_green = leds.pin_green;
    uint8_t pin_blue = leds.pin_blue;
    uint8_t pin_color = leds.pin_color;
    add_rgb_led(pin_red, pin_green, pin_blue);
    set_color(pin_red, pin_color);
  }
}

void add_profiles_from_eeprom(void)
{

  struct profile_eeprom_s profiles;
  uint8_t perc_x_temp[10];
  counter_eeprom_profiles = eeprom_read_word((uint16_t *)addr_for_counter_profiles);
  uint16_t current_addr;

  for (int i = 0; i < counter_eeprom_profiles; i++)
  {

    current_addr = i * offset_for_profiles + start_addr_for_profiles;
    eeprom_read_block(&profiles, (void *)current_addr, sizeof(struct profile_eeprom_s));
    uint8_t mode = profiles.mode;
    if (mode == MANUAL)
    {
      memcpy(&perc_x_temp, &(profiles.perc_x_temp), 10);
      add_profile(profiles.fan_id, profiles.sensor_pin, mode, (uint8_t *)perc_x_temp);
      set_flag_speed_to_zero(profiles.fan_id);
    }

    else
    {
      memset(&perc_x_temp, 0, 10);
      add_profile(profiles.fan_id, profiles.sensor_pin , mode, (uint8_t*)perc_x_temp);
      set_flag_speed_to_zero(profiles.fan_id);
    }
  }
}

void add_structs_from_eeprom(void)
{

  add_sensors_from_eeprom();
  add_fans_from_eeprom();
  add_leds_from_eeprom();
  add_profiles_from_eeprom();
}

void update_eeprom(void)
{
  //clean_eeprom();
  eeprom_dirty_clean();
  counter_eeprom_fans=0;
  counter_eeprom_leds=0;
  counter_eeprom_sensors=0;
  counter_eeprom_profiles=0;


  node *sensor_node;
  node *led_node;
  node_pi *profile_node;
  struct fan **fan_list;

  // primo passo aggiornamento sensori
  sensor_node = get_sensor_list_head();
  while (sensor_node != NULL)
  {
    sensor *s = sensor_node->data;
    add_sensor_to_eeprom(s->pin, s->type);
    sensor_node = sensor_node->next;
  }

  //secondo passo aggiornamento fan

  fan_list = get_fan_manager();
  if (fan_list != NULL)
  {
    for (int i = 0; i < FAN_NUMBER; i++)
    {
      if (fan_list[i] != NULL)
      {
        add_fan_to_eeprom(fan_list[i]->pwm_pin, fan_list[i]->encoder_pin);
      }
    }
  }

  //terzo passo aggiornamento profili


  profile_node = (node_pi *)get_profile_list_head();
  while (profile_node != NULL)
  {
    struct profile *s =(struct profile *) profile_node->data;
    uint8_t fan_id = s->id_fan;
    uint8_t sensor_pin = s->pin_sensor;
    uint8_t mode = s->mode;
    //la struttura va inviata solo se è differente da quelle basic
    // e quindi che è stata scelta dall'utente
    if (mode == MANUAL)
    {

      uint8_t *old_perc_x_temp = s->perc_x_temp;
      uint8_t new_perc_x_temp[10];
      for (int i = 0; i < 10; i++)
      {
        new_perc_x_temp[i] = old_perc_x_temp[i];
      }

      add_profile_to_eeprom(fan_id, sensor_pin, mode, new_perc_x_temp);
    }
    else
    {
      add_profile_to_eeprom(fan_id ,sensor_pin, mode, NULL);
    }
    profile_node = profile_node->next;
  }

  //quarto passo aggiunta dei led

  led_node = get_led_list_head();
  while (led_node != NULL)
  {
    struct led *l = led_node->data;
    uint8_t red_pin = l->red_pin;
    uint8_t green_pin = l->green_pin;
    uint8_t blue_pin = l->blue_pin;
    uint8_t color_pin = l->current_color;

    add_led_to_eeprom(red_pin, green_pin, blue_pin);
    add_color_to_led_eeprom(red_pin,color_pin);//, color_pin);

    led_node = led_node->next;
  }


}

void update_eeprom_led(void){
  eeprom_update_word((uint16_t *)addr_for_counter_leds, 0);
  counter_eeprom_leds=0;

  node *led_node;
  led_node = get_led_list_head();
  while (led_node != NULL)
  {
    struct led *l = led_node->data;
    uint8_t red_pin = l->red_pin;
    uint8_t green_pin = l->green_pin;
    uint8_t blue_pin = l->blue_pin;
    uint8_t color_pin = l->current_color;

    add_led_to_eeprom(red_pin, green_pin, blue_pin);
    add_color_to_led_eeprom(red_pin,color_pin);//, color_pin);

    led_node = led_node->next;
  }

}
void update_eeprom_fan(void){

  eeprom_update_word((uint16_t *)addr_for_counter_fans, 0);
  counter_eeprom_fans=0;

  struct fan **fan_list;
  fan_list = get_fan_manager();
  if (fan_list != NULL)
  {
    for (int i = 0; i < FAN_NUMBER; i++)
    {
      if (fan_list[i] != NULL)
      {
        add_fan_to_eeprom(fan_list[i]->pwm_pin, fan_list[i]->encoder_pin);
      }
    }
  }
}
void update_eeprom_sensor(void){

  eeprom_update_word((uint16_t *)addr_for_counter_sensors, 0);
  counter_eeprom_sensors=0;

  node *sensor_node;
  sensor_node = get_sensor_list_head();
  while (sensor_node != NULL)
  {
    sensor *s = sensor_node->data;
    add_sensor_to_eeprom(s->pin, s->type);
    sensor_node = sensor_node->next;
  }
}

void update_eeprom_profile(void){
  eeprom_update_word((uint16_t *)addr_for_counter_profiles, 0);
  counter_eeprom_profiles=0;
  node_pi * profile_node;
  profile_node = (node_pi *)get_profile_list_head();
  while (profile_node != NULL)
  {
    struct profile *s =(struct profile *) profile_node->data;
    uint8_t fan_id = s->id_fan;
    uint8_t sensor_pin = s->pin_sensor;
    uint8_t mode = s->mode;
    //la struttura va inviata solo se è differente da quelle basic
    // e quindi che è stata scelta dall'utente
    if (mode == MANUAL)
    {

      uint8_t *old_perc_x_temp = s->perc_x_temp;
      uint8_t new_perc_x_temp[10];
      for (int i = 0; i < 10; i++)
      {
        new_perc_x_temp[i] = old_perc_x_temp[i];
      }

      add_profile_to_eeprom(fan_id, sensor_pin, mode, new_perc_x_temp);
    }
    else
    {
      add_profile_to_eeprom(fan_id, sensor_pin, mode, NULL);
    }
    profile_node = profile_node->next;
  }
}

void eeprom_dirty_clean(void){

  eeprom_update_word((uint16_t *)addr_for_counter_sensors, 0);
  eeprom_update_word((uint16_t *)addr_for_counter_fans, 0);
  eeprom_update_word((uint16_t *)addr_for_counter_leds, 0);
  eeprom_update_word((uint16_t *)addr_for_counter_profiles, 0);



}

void add_color_to_led_eeprom(uint8_t red_pin,uint8_t color){
  //ricorda che il red_pin è l'id ed è unico!!!
  uint16_t current_addr;
  struct led_eeprom_s  leds;
  for(int i=0;i<counter_eeprom_leds;i++){
    current_addr=i*offset_for_leds + start_addr_for_leds;
    eeprom_read_block(&leds, (void *)current_addr, sizeof(struct led_eeprom_s));
    if(leds.pin_red==red_pin){
      leds.pin_color=color;
      eeprom_update_block(&leds, (void *)current_addr, sizeof(struct led_eeprom_s));
      break;
    }


  }

}

void add_speed_to_fan_eeprom(uint8_t id_fan, uint8_t speed){

  uint16_t current_addr;
  struct fan_eeprom_s fans;
  for(int i=0;i<counter_eeprom_fans;i++){
    current_addr= i * offset_for_fans + start_addr_for_fans;
    eeprom_read_block(&fans, (void *)current_addr, sizeof(struct led_eeprom_s));
    if(fans.pwm_pin==id_fan){
      fans.speed=speed;
      fans.speed_flag=1;
      eeprom_update_block(&fans, (void *)current_addr, sizeof(struct fan_eeprom_s));
      break;
    }


  }

}

void set_flag_speed_to_zero(uint8_t id_fan){
  /*
  uint16_t current_addr;
  struct fan_eeprom_s fans;
  for(int i=0;i<counter_eeprom_fans;i++){
    current_addr= i * offset_for_fans + start_addr_for_fans;
    eeprom_read_block(&fans, (void *)current_addr, sizeof(struct led_eeprom_s));
    if(fans.pwm_pin==id_fan){
      fans.speed_flag=0;
      eeprom_update_block(&fans, (void *)current_addr, sizeof(struct fan_eeprom_s));
      break;
    }


  }
  */

}

//barbatrucco
const uint16_t control_addr1 = 3094;
const uint16_t control_addr2 = 3095;
const uint8_t value_addr1=220;
const uint8_t value_addr2=185;
//se nei due indirizzi addr1 and addr2
//sono salvate le due variabili value_addr1 e value_addr2
//allora non è la prima volta che il programma viene lanciato
//vedi init_from_epprom
void init_from_eeprom(void){
  uint8_t value1=eeprom_read_byte((uint8_t*)control_addr1);
  uint8_t value2=eeprom_read_byte((uint8_t*)control_addr2);
  if(value1==value_addr1 && value2==value_addr2){
    add_structs_from_eeprom();
  }
  else{
    clean_eeprom();
    eeprom_write_byte((uint8_t*)control_addr1,value_addr1);
    eeprom_write_byte((uint8_t*)control_addr2,value_addr2);
  }
}

void clean_eeprom(void){
  for(int i=0;i<4095;i++){
    eeprom_write_byte((uint8_t*)i,0);
  }

}
