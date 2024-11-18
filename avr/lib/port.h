#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT 1
#define INPUT 0
#define INPUTPULLUP 2

#define HIGH 1
#define LOW 0

#define A0 97
#define A1 96
#define A2 95
#define A3 94
#define A4 93
#define A5 92
#define A6 91
#define A7 90
#define A8 89
#define A9 88
#define A10 87
#define A11 86
#define A12 85
#define A13 84
#define A14 83
#define A15 82


/*
Questa libreria permette di gestire i pin dal 22 al 53 in modo digitale
E in pin da 2 al 13, e il 44,45,46 in pwm.
dell' atmega2560 [Arduino Mega]
in maniera molto semplice. [Alla arduino IDE :-) ]
*/

void set_mode(int pin, int mode);
/*
  Disponile per i pin dal 22 al 53 e dal 0 al 13

  pin rappresenta il pin
  Mode:
    Se uguale a 1 lo imposta come OUTPUT
    Se uguale a 0 lo imposta come INPUT
*/
void set_level(int pin, int level);
/*
Disponile per i pin dal 22 al 53 e dal 0 al 13

  se level == HIGH e il pin è impostato su OUTPUT imposta il pin a +5v
  se level == HIGH e il pin è impostato su INPUT imposta il pin alla resistenza di pull-up

  se level == LOW e il pin è impostato su OUTPUT imposta l'uscita a 0v
*/
int get_level(int pin);
/*
  Disponile per i pin dal 22 al 53 e dal 0 al 13

  Ritorna 1 se sul pin è applicata una tensione vicina ai 5v
  Ritorna 0 se sul pin è applicata una tensione vicina ai 0v

  Ritorna -1 se il pin è fuori dal range
*/

void set_pwm(int pin, uint8_t value);
/*
Imposta un segnale pwm nel pin indicato.
è disponibile solamente per i pin 2,3,4,5,6,7,8,9,10,11,12,13,44,45,46
*/

int analog_read(uint8_t pin);
/*
lettura analogica di un pin
disponibile per qualsiasi ingresso analogico.
Restituisce un valore di 10 bit, proporzionale alla tensione in ingresso.
Restituisce -1 se il pin non dispone di un adc
*/

int filtered_analog_read(uint8_t pin);
