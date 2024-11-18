#ifndef COLOR
#define COLOR

#include <stdint.h>

#define TURNOFF 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define AZURE 5
#define MAGENTA 6
#define WHITE 7

char * get_color_name(uint8_t color);

#endif
