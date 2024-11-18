#ifndef SERIAL_LINUX_H
#define SERIAL_LINUX_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "../avr_client_common/packets.h"
#include "../avr_client_common/serial_protocol.h"

  //! returns the descriptor of a serial port
  int serial_open(const char* name);

  //! sets the attributes
  int serial_set_interface_attribs(int fd, int speed, int parity);

  //! puts the port in blocking/nonblocking mode
  void serial_set_blocking(int fd, int should_block);

  void serial_read (int fd, uint8_t * buf, size_t size);
  void serial_write(int fd, uint8_t * buf, size_t size);
  void serial_putData(int fd,uint8_t * data, uint32_t data_size, uint8_t data_type);

#endif
