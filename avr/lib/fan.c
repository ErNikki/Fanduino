#include "fan.h"

struct fan ** fan_manager = NULL;

ISR(INT0_vect) {
  const uint8_t interrupt_number = 0;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number)
      fan_manager[i]->encoder_tick++;
  }
  EIMSK |= 1 << interrupt_number;
}

ISR(INT1_vect) {
  const uint8_t interrupt_number = 1;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number)
      fan_manager[i]->encoder_tick++;
  }
  EIMSK |= 1 << interrupt_number;
}

ISR(INT2_vect) {
  const uint8_t interrupt_number = 2;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number)
      fan_manager[i]->encoder_tick++;
  }
  EIMSK |= 1 << interrupt_number;
}

ISR(INT3_vect) {
  const uint8_t interrupt_number = 3;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number)
      fan_manager[i]->encoder_tick++;
  }
  EIMSK |= 1 << interrupt_number;
}

ISR(INT4_vect) {
  uint8_t interrupt_number = 4;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number &&
        fan_manager[i]->encoder_tick < 255) {
      fan_manager[i]->encoder_tick++;
    }
  }
  EIMSK |= 1 << interrupt_number;
}

ISR(INT5_vect) {
  const uint8_t interrupt_number = 5;
  EIMSK &= ~1 << interrupt_number;
  for (uint8_t i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL &&
        fan_manager[i]->interrupt_id == interrupt_number)
      fan_manager[i]->encoder_tick++;
  }
  EIMSK |= 1 << interrupt_number;
}

void create_fan_manager(void) {
  fan_manager = malloc(sizeof(struct fan *) * FAN_NUMBER);
  for (int i = 0; i < FAN_NUMBER; i++)
    fan_manager[i] = NULL;
}

void delete_fan_manager(void) {
  for (int i = 0; i < FAN_NUMBER; i++)
    remove_fan(i);
}

int8_t add_fan(int8_t pwm_pin, int8_t fan_encoder_pin) {
  const int8_t pwm_pins[15]={2,3,4,5,6,7,8,9,10,11,12,13,44,45,46};
  const int8_t interrupt_pins[7]={2,3,18,19,20,21,-1};

  if (fan_manager == NULL)
    return FAN_MANAGER_NOT_INITIALIZED;
  if (pwm_pin == (uint8_t)fan_encoder_pin)
    return PWM_PIN_AND_FAN_SPEED_EQUALS;
  if(!is_in_array(pwm_pins,15,pwm_pin))
    return PWM_PIN_WRONG;
  if(!is_in_array(interrupt_pins,7,fan_encoder_pin))
    return ENCODER_PIN_WRONG;

  uint8_t i;
  for (i = 0; i < FAN_NUMBER; i++) {
    if (fan_manager[i] != NULL && fan_manager[i]->pwm_pin == pwm_pin)
      return FAN_ALREADY_CONNECTED;
    if (fan_manager[i] == NULL)
      break;
  }

  if (i == FAN_NUMBER)
    return MAX_FAN_CONNECTED;

  if (fan_encoder_pin != -1) {
    set_mode(fan_encoder_pin, INPUT);
    set_level(fan_encoder_pin, HIGH);
  }



  struct fan *fan_struct = malloc(sizeof(struct fan));
  int8_t int_number = get_interrupt(fan_encoder_pin);
  fan_struct->id = pwm_pin;
  fan_struct->pwm_pin = pwm_pin;
  fan_struct->encoder_pin = fan_encoder_pin;
  fan_struct->interrupt_id = int_number;
  fan_struct->encoder_tick = 0;

  fan_manager[i] = fan_struct;

  if (int_number != -1) {
    EIMSK |= 1 << int_number;
    switch (int_number / 4) {
    case 0:
      EICRA = 0xFF; // 3<<(int_number*2);
      break;
    case 1:
      EICRB = 0xFF; // 3<<((int_number-4)*2);
      break;
    }
  }
  set_speed(fan_struct->id, 0);

  return fan_struct->id;
}

void remove_fan(int8_t fan_id) {
  if (fan_manager == NULL)
    return;
  set_speed(fan_id, 0);
  fan_id = get_fan_index(fan_id);
  int8_t interrupt_id = fan_manager[fan_id]->interrupt_id;
  if (interrupt_id >= 0)
    EIMSK |= 1 << interrupt_id;

  free(fan_manager[fan_id]);
  fan_manager[fan_id] = NULL;
}

int get_fan_number(void){
  if(fan_manager==NULL)return 0;
  int k=0;
  for(int i = 0;i<FAN_NUMBER;i++){
    if(fan_manager[i]!=NULL)k++;
  }
  return k;
}

void set_speed(int8_t fan_id, uint8_t rate) {
  fan_id = get_fan_index(fan_id);
  const uint8_t threshold = 50;

  if (fan_manager == NULL || fan_manager[fan_id] == NULL)
    return;

  if(rate>100)rate=100;

  fan_manager[fan_id]->encoder_tick = 0;
  rate = ((float)rate)*2.55;

  if (rate == 0 || fan_manager[fan_id]->encoder_pin == -1) {
    set_pwm(fan_manager[fan_id]->pwm_pin, rate);
  }
  else if (rate > 200){
    set_pwm(fan_manager[fan_id]->pwm_pin, rate);
  }
  else {
    set_pwm(fan_manager[fan_id]->pwm_pin, 200);
    sei();
    uint16_t watchdog = 0;
    while (watchdog < 1000) {
      fan_manager[fan_id]->encoder_tick=0;
      _delay_ms(50);
      if(fan_manager[fan_id]->encoder_tick > threshold){
        rate-=8;
        set_pwm(fan_manager[fan_id]->pwm_pin, rate);
      }
      else {
        rate+=8;
        set_pwm(fan_manager[fan_id]->pwm_pin, rate);
        break;
      }
      watchdog++;
    }
    //cli();


  }
  fan_manager[fan_id]->speed = ((float)rate)/2.55;
}

void dump_fan(struct fan_packet * data){
    for(int i = 0;i<FAN_NUMBER;i++){
      if(fan_manager[i]!=NULL){
        data->id = fan_manager[i]->id;
        data->pwm_pin = fan_manager[i]->pwm_pin;
        data->encoder_pin = fan_manager[i]->encoder_pin;
        data->speed = fan_manager[i]->speed;
        data++;
      }
  }
}

int8_t get_interrupt(int8_t pin) {
  switch (pin) {
  case 21:
    return 0;
  case 20:
    return 1;
  case 19:
    return 2;
  case 18:
    return 3;
  case 2:
    return 4;
  case 3:
    return 5;
  default:
    return -1;
  }
}

int is_in_array(const int8_t * arr, size_t size, int8_t dato){
  for(int i = 0;i<size;i++){
    if(arr[i]==dato)
      return 1;
  }
  return 0;
}

void stop_all(void){
  for(int i = 0;i<FAN_NUMBER;i++){
    if(fan_manager[i]!=NULL){
      set_pwm(fan_manager[i]->pwm_pin,0);
    }
  }
}

void busrt_all(uint8_t delta){
  for(int i = 0;i<FAN_NUMBER;i++){
    if(fan_manager[i]!=NULL){
      uint16_t speed = fan_manager[i]->speed+delta;
      if(speed>255)speed=255;
      set_pwm(fan_manager[i]->pwm_pin,speed);
    }
  }
}

void start_all(void){
  for(int i = 0;i<FAN_NUMBER;i++){
    if(fan_manager[i]!=NULL){
      set_pwm(fan_manager[i]->pwm_pin,fan_manager[i]->speed);
    }
  }
}

struct fan** get_fan_manager(void){
  if(fan_manager!=NULL){
    return fan_manager;
  }
  return NULL;
}

struct fan * get_fan(uint8_t index){
  return fan_manager[index];
}

uint8_t get_fan_index(uint8_t id){
  for(uint8_t i = 0;i< FAN_NUMBER;i++)
    if(fan_manager[i]!=NULL && fan_manager[i]->id==id)
      return i;
  return 255;
}

uint8_t fan_is_setted(uint8_t id_fan){
  for(uint8_t i = 0;i< FAN_NUMBER;i++)
    if(fan_manager[i]!=NULL && fan_manager[i]->id==id_fan)
      return 1;
  return 0;
}
