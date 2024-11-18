#include "serial_protocol.h"

static const char * header = "FanDuino-Header0";
#define header_size strlen(header)

int serial_order_data(uint8_t * src,uint8_t * dst, size_t size){
  /*
  I dati potrebbero arrivare con un ordine non corretto, perciò allinea i byte
  in modo che i primi 16 byte contengono un header prefissata

  ritorna 1 in caso di successo
  ritorna -1 in caso di insuccesso, header non trovato
  */

  uint8_t * tmp = malloc(size*2);
  memcpy(tmp,src,size);
  memcpy(tmp+size,src,size);
  int i,c;

  for(i = 0; i < size; i++){
    c = memcmp(tmp+i,header,header_size);
    if(c==0)break;
  }
  if(i==size)return -1;
  i+=16;
  for(int j=0;j<size-header_size;j++){
    i=i%size;
    *(dst+j)=*(src+i);
    i++;
  }
  free(tmp);
  return 1;
}

struct data fill_data(uint8_t * data, uint32_t data_size, uint8_t data_type){
  struct data d;
  memset(&d,0,sizeof(struct data));
  d.data_size=data_size;
  d.data_type=data_type;

  uint16_t i;
  for(i = 0; i<data_size ; i++){
    d.data[i]=*data;
    data++;
  }
  for(; i<MAX_DATA;i++){
    d.data[i]=0;
  }

  return d;
}

void parse_data (struct data * message_data, uint8_t * data, uint32_t data_size){
  if(message_data->data_size!=data_size)return;
  for(uint16_t i = 0; i<message_data->data_size; i++){
    *data=message_data->data[i];
    data++;
  }
}
