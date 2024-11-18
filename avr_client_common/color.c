#include "color.h"

char * get_color_name(uint8_t color){
  switch (color) {
    case 0:
      return "spento";
    case 1:
      return "rosso";
    case 2:
      return "verde";
    case 3:
      return "blu";
    case 4:
      return "giallo";
    case 5:
      return "azzurro";
    case 6:
      return "magenta";
    break;
    case 7:
      return "bianco";
    default:
      return "";
  }
}
