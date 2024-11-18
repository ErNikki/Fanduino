#include "sensor.h"

struct linked_list_s *sensor_list = NULL;

int dht_temp;

int dht11_temperature(uint8_t pin)
{

  uint8_t timeout = 0;
  uint8_t i = 0;
  int dht11_dat[5];
  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  //per sicurezza
  set_mode(pin, INPUTPULLUP);
  _delay_ms(1);

  //ATOMIC_BLOCK(ATOMIC_FORCEON){

    //inizio la comunicazione
    //setto a low per iniziare la comunicazione
    set_mode(pin, OUTPUT);
    set_level(pin, LOW);
    _delay_ms(18);

    //setto ad high e aspetto una risposta
    set_level(pin, HIGH);
    _delay_us(20);

    //mi metto in attesa
    set_mode(pin, INPUTPULLUP);

    //aspetto di ricevere il primo segnale low
    timeout = TINUDHT_ACK_TIMEOUT;
    while (get_level(pin) == 0)
    {
      _delay_us(1);
      if (--timeout == 0)
        return -100;
    }

    //aspetto il successivo segnale high
    timeout = TINUDHT_ACK_TIMEOUT;
    while (get_level(pin) != 0)
    {
      _delay_us(1); // Wait for HI-to-LO
      if (--timeout == 0)
        return -200;
    }

    //inizio a ricevere dati (5 byte)
    for (; i < 40; i++)
    {

      //il primo segnale low è di avvertimento
      while (get_level(pin) == 0)
      {
        _delay_us(1);
        if (--timeout == 0)
          return -300;
      }

      //il successivo segnale high è il bit
      timeout = TINUDHT_ACK_TIMEOUT;
      while (get_level(pin) != 0)
      {
        _delay_us(1);
        if (--timeout == 0)
          return -400;
      }

      dht11_dat[i / 8] <<= 1;
      if (timeout <= TINUDHT_ACK_TIMEOUT - TINUDHT_BITLEN)
      {
        dht11_dat[i / 8] |= 1;
      }
    }
    //dht11_dat[0] umidità
    //dht11_dat[1] i centesimi dell'umidità
    //umidita = dht11_dat[0].dht11_dat[1]
    //stesso discorso per temperatura (2 e 3)
    //dht11_dat[4] checksum
    if (i == 40 && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)))
    {
      return dht11_dat[2];
    }
    else
      return -500;
  //}
  return -99;
}

int lm35dz_temperature(uint8_t pin)
{
  int temp=filtered_analog_read(pin);
  if (temp==0)return 0;
  temp*=500;
  temp/=1024;

  return temp;
}

int mcp9700a_temperature(uint8_t pin)
{
  float vout = (float)filtered_analog_read(pin);
  if(vout==0)return 0;
  vout = ((vout * 5.0 / 1024.0) - 0.5) / 0.01;

  return (int)vout;
}

void create_sensor_list(void)
{
  if (sensor_list == NULL)
  {
    sensor_list = linked_list_init();
  }
}

void delete_sensor_list(void)
{
  if (sensor_list != NULL)
  {
    linked_list_delete(sensor_list,1,1);
  }
  sensor_list = NULL;
}

int8_t add_sensor(uint8_t pin, uint8_t type)
{

  if (linked_list_get_node(sensor_list, pin) != NULL)
  {
    return -1;
  }
  if (type != LM35DZ && type != DHT11 && type != MPC9700A)
  {
    return -2;
  }

  sensor *data = (sensor *)malloc(sizeof(sensor));
  data->pin = pin;
  data->type = type;
  linked_list_add_node(sensor_list, data);
  return pin;
}

void remove_sensor(uint8_t pin)
{

  node *node = linked_list_get_node(sensor_list, pin);
  if (node == NULL)
  {
    return;
  }
  linked_list_remove_node(sensor_list, pin,1);
}

int get_temperature(uint8_t pin)
{
  sensor *data = (sensor *)linked_list_get_data(sensor_list, pin);
  if (data == NULL)
  {
    return -1;
  }
  uint8_t type = data->type;
  int tmp;
  switch (type)
  {
  case LM35DZ:
    return lm35dz_temperature(data->pin);
  break;
  case DHT11:
    tmp = dht11_temperature(data->pin);
    if(tmp>0)dht_temp=tmp;
    return dht_temp;
  break;
  case MPC9700A:
    return mcp9700a_temperature(data->pin);
    break;
  default:
    return -2;
    break;
  }
}

int get_sensor_number(void)
{
  return linked_list_size(sensor_list);
}

void dump_sensor(struct sensor_packet *data)
{
  node *n = sensor_list->head;
  while (n != NULL)
  {
    sensor *s = n->data;
    data->id = s->pin;
    data->pin = s->pin;
    data->type = s->type;
    int temp = get_temperature(s->pin);
    if(temp>=0)data->temperature =  (uint8_t)temp;
    else data->temperature = 0;
    data++;
    n = n->next;
  }
}

uint8_t sensor_is_setted(uint8_t pin)
{

  if (linked_list_get_node(sensor_list,pin) != NULL)
  {
    return 1;
  }
  return 0;
}

node* get_sensor_list_head(void){
    if(sensor_list!=NULL){
      return sensor_list->head;
    }

    return NULL;
}
