CC = gcc
CFLAGS = -Wall -lpthread -pthread
SRCS = pc.c serial_linux.c comunication.c print_data.c
HEADERS = serial_linux.h ../avr_client_common/packets.h print_data.h comunication.h
OBJS = $(SRCS:.c=.o)
MAIN = pc
CROSS_PLATFROM += ../avr_client_common/serial_protocol.*
CROSS_PLATFROM += ../avr_client_common/linked_list_with_id.*
CROSS_PLATFROM += ../avr_client_common/color.*


.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) $(HEADERS) $(CROSS_PLATFROM) -o $(MAIN)

*.o: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c

clean:
	rm -rf *.h.gch
	rm -rf *.o
	rm -rf $(MAIN)
