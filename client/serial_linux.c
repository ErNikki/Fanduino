#include "serial_linux.h"

static const char * header = "FanDuino-Header0";
#define header_size strlen(header)


int serial_set_interface_attribs(int fd, int speed, int parity) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    printf ("error %d from tcgetattr", errno);
    return -1;
  }
  switch (speed){
  case 9600:
    speed=B9600;
  break;
  case 19200:
    speed=B19200;
    break;
  case 57600:
    speed=B57600;
    break;
  case 115200:
    speed=B115200;
    break;
  default:
    printf("cannot sed baudrate %d\n", speed);
    return -1;
  }
  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);
  cfmakeraw(&tty);
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);               // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;      // 8-bit chars

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    printf ("error %d from tcsetattr", errno);
    return -1;
  }
  return 0;
}

void serial_set_blocking(int fd, int should_block) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
      printf ("error %d from tggetattr", errno);
      return;
  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    printf ("error %d setting term attributes", errno);
}

int serial_open(const char* name) {
  int fd = open (name, O_RDWR | O_NOCTTY | O_SYNC );
  if (fd < 0) {
    fprintf (stderr,"error %d opening serial, fd %d\n", errno, fd);
  }
  return fd;
}


void serial_read(int fd, uint8_t * buf, size_t size){
  uint8_t b;
  size=size+header_size;
  uint8_t * ibuf = malloc(size);

  for(int i = 0 ;i<size;i++){
    if(read(fd,&b,sizeof(uint8_t))<=0) {
      perror("Read Error!");
      exit(1);
      break;
    }
    *(ibuf+i)=b;
  }
  serial_order_data(ibuf,buf,size);
  free(ibuf);
}

void serial_write(int fd,uint8_t * buf, size_t size){
  uint8_t * msg = malloc(size+header_size);
  memcpy(msg,header,header_size);
  memcpy(msg+header_size,buf,size);

  for(int i = 0;i<size+header_size;i++){
    if(write(fd,msg+i,sizeof(uint8_t))==-1)
    {
      perror("WRITE ERROR");
      break;
    }
    //else printf("%d w-> %d\n",i,msg[i]);
  }
  free(msg);
}

void serial_putData(int fd, uint8_t * data, uint32_t data_size, uint8_t data_type){
  struct data d = fill_data(data,data_size,data_type);
  serial_write(fd,(uint8_t*)&d,sizeof(struct data));
}

int cmp(uint8_t * s1 ,uint8_t * s2 ,size_t size){
  for(int i = 0; i<size; i++){
    if(*(s1+i)!=*(s2+i))return -1;
  }
  return 1;
}
