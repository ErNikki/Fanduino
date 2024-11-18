#include "port.h"

void set_mode(int pin, int mode)
{

  if (pin >= 0 && pin <= 3)
  {
    uint8_t number = pin;
    if (pin > 1)
      number = pin + 2;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRE &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRE &= ~mask;
      PORTE |= mask;
    }

    else
      DDRE |= mask;
  }
  else if (pin == 4)
  {
    const uint8_t mask = (1 << PG5);
    if (mode == INPUT)
      DDRG &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRG &= ~mask;
      PORTG |= mask;
    }
    else
      DDRG |= mask;
  }
  else if (pin == 5)
  {
    const uint8_t mask = (1 << PE3);
    if (mode == INPUT)
      DDRE &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRE &= ~mask;
      PORTE |= mask;
    }
    else
      DDRE |= mask;
  }
  else if (pin >= 6 && pin <= 9)
  {
    const uint8_t number = pin - 3;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRH &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRH &= ~mask;
      PORTH |= mask;
    }
    else
      DDRH |= mask;
  }
  else if (pin >= 10 && pin <= 13)
  {
    const uint8_t number = pin - 6;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRB &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRB &= ~mask;
      PORTB |= mask;
    }
    else
      DDRB |= mask;
  }
  else if (pin == 14 || pin == 15)
  {
    const uint8_t number = 15 - pin;
    const uint8_t mask = (1 << number);

    if (mode == INPUT)
      DDRJ &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRJ &= ~mask;
      PORTJ |= mask;
    }
    else
      DDRJ |= mask;
  }
  else if (pin == 16 || pin == 17)
  {
    const uint8_t number = 17 - pin;
    const uint8_t mask = (1 << number);

    if (mode == INPUT)
      DDRH &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRH &= ~mask;
      PORTH |= mask;
    }
    else
      DDRH |= mask;
  }
  else if (pin >= 18 && pin <= 21)
  { //Port D
    const uint8_t number = (21 - pin) * -1;
    const uint8_t mask = (1 << number);

    if (mode == INPUT)
      DDRD &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRD &= ~mask;
      PORTD |= mask;
    }
    else
      DDRD |= mask;
  }
  else if (pin >= 22 && pin <= 29) //Port A
  {
    const uint8_t number = pin - 22;
    const uint8_t mask = (1 << number);

    if (mode == INPUT)
      DDRA &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRA &= ~mask;
      PORTA |= mask;
    }
    else
      DDRA |= mask;
  }
  else if (pin >= 30 && pin <= 37) //Port C
  {
    const uint8_t number = (pin - 37) * -1;
    const uint8_t mask = (1 << number);

    if (mode == INPUT)
      DDRC &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRC &= ~mask;
      PORTC |= mask;
    }
    else
      DDRC |= mask;
  }
  else if (pin == 38) //Port D
  {
    const uint8_t mask = (1 << 7);
    if (mode == INPUT)
      DDRD &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRD &= ~mask;
      PORTD |= mask;
    }
    else
      DDRD |= mask;
  }
  else if (pin >= 39 && pin <= 41) //Port G
  {
    const uint8_t number = (pin - 41) * -1;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRG &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRG &= ~mask;
      PORTG |= mask;
    }
    else
      DDRG |= mask;
  }
  else if (pin >= 42 && pin <= 49) //Port L
  {
    const uint8_t number = (pin - 49) * -1;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRL &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRL &= ~mask;
      PORTL |= mask;
    }
    else
      DDRL |= mask;
  }
  else if (pin >= 50 && pin <= 53) //Port B
  {
    const uint8_t number = (pin - 53) * -1;
    const uint8_t mask = (1 << number);
    if (mode == INPUT)
      DDRB &= ~mask;
    else if (mode == INPUTPULLUP)
    {
      DDRB &= ~mask;
      PORTB |= mask;
    }
    else
      DDRB |= mask;
  }
}

void set_level(int pin, int level)
{
  if (pin >= 0 && pin <= 3)
  {
    uint8_t number = pin;
    if (pin > 1)
      number += 2;
    const uint8_t mask = 1 << number;
    if (level == HIGH)
      PORTE |= mask;
    else
      PORTE &= ~mask;
  }
  else if (pin == 4)
  {
    const uint8_t mask = (1 << PG5);
    if (level == HIGH)
      PORTG |= mask;
    else
      PORTG &= ~mask;
  }
  else if (pin == 5)
  {
    const uint8_t mask = (1 << PE3);
    if (level == HIGH)
      PORTE |= mask;
    else
      PORTE &= ~mask;
  }
  else if (pin >= 6 && pin <= 9)
  {
    const uint8_t number = pin - 3;
    const uint8_t mask = (1 << number);
    if (level == HIGH)
      PORTH |= mask;
    else
      PORTH &= ~mask;
  }
  else if (pin >= 10 && pin <= 13)
  {
    const uint8_t number = pin - 6;
    const uint8_t mask = (1 << number);
    if (level == HIGH)
      PORTB |= mask;
    else
      PORTB &= ~mask;
  }
  else if (pin == 14 || pin == 15)
  {
    const uint8_t number = 15 - pin;
    const uint8_t mask = (1 << number);

    if (level == HIGH)
      PORTJ |= mask;
    else
      PORTJ &= ~mask;
  }
  else if (pin == 16 || pin == 17)
  {
    const uint8_t number = 17 - pin;
    const uint8_t mask = (1 << number);

    if (level == HIGH)
      PORTH |= mask;
    else
      PORTH &= ~mask;
  }
  else if (pin >= 18 && pin <= 21)
  { //Port D
    const uint8_t number = (21 - pin) * -1;
    const uint8_t mask = (1 << number);

    if (level == HIGH)
      PORTD |= mask;
    else
      PORTD &= ~mask;
  }
  else if (pin >= 22 && pin <= 29) //Port A
  {
    const uint8_t number = pin - 22;
    const uint8_t mask = (1 << number);

    if (level == HIGH)
      PORTA |= mask;
    else
      PORTA &= ~mask;
  }
  else if (pin >= 30 && pin <= 37) //Port C
  {
    const uint8_t number = (pin - 37) * -1;
    const uint8_t mask = (1 << number);

    if (level == HIGH)
      PORTC |= mask;
    else
      PORTC &= ~mask;
  }
  else if (pin == 38) //Port D
  {
    const uint8_t mask = (1 << 7);
    if (level == HIGH)
      PORTD |= mask;
    else
      PORTD &= ~mask;
  }
  else if (pin >= 39 && pin <= 41) //Port G
  {
    const uint8_t number = (pin - 41) * -1;
    const uint8_t mask = (1 << number);
    if (level == HIGH)
      PORTG |= mask;
    else
      PORTG &= ~mask;
  }
  else if (pin >= 42 && pin <= 49) //Port L
  {
    const uint8_t number = (pin - 49) * -1;
    const uint8_t mask = (1 << number);
    if (level == HIGH)
      PORTL |= mask;
    else
      PORTL &= ~mask;
  }
  else if (pin >= 50 && pin <= 53) //Port B
  {
    const uint8_t number = (pin - 53) * -1;
    const uint8_t mask = (1 << number);
    if (level == HIGH)
      PORTB |= mask;
    else
      PORTB &= ~mask;
  }
}

int get_level(int pin)
{
  if (pin >= 0 && pin <= 3)
  {
    uint8_t number = pin;
    if (pin > 1)
      number += 2;

    const uint8_t mask = 1 << number;
    return (PINE & mask) != 0;
  }
  else if (pin == 4)
  {
    const uint8_t mask = (1 << PG5);
    return (PING & mask) != 0;
  }
  else if (pin == 5)
  {
    const uint8_t mask = (1 << PE3);
    return (PINE & mask) != 0;
  }
  else if (pin >= 6 && pin <= 9)
  {
    const uint8_t number = pin - 3;
    const uint8_t mask = (1 << number);
    return (PINH & mask) != 0;
  }
  else if (pin >= 10 && pin <= 13)
  {
    const uint8_t number = pin - 6;
    const uint8_t mask = (1 << number);
    return (PINB & mask) != 0;
  }
  else if (pin == 14 || pin == 15)
  {
    const uint8_t number = 15 - pin;
    const uint8_t mask = (1 << number);

    return (PINJ & mask) != 0;
  }
  else if (pin == 16 || pin == 17)
  {
    const uint8_t number = 17 - pin;
    const uint8_t mask = (1 << number);

    return (PINH & mask) != 0;
  }
  else if (pin >= 18 && pin <= 21)
  { //Port D
    const uint8_t number = (21 - pin) * -1;
    const uint8_t mask = (1 << number);

    return (PIND & mask) != 0;
  }
  else if (pin >= 22 && pin <= 29) //Port A
  {
    const uint8_t number = pin - 22;
    const uint8_t mask = (1 << number);

    return (PINA & mask) != 0;
  }
  else if (pin >= 30 && pin <= 37) //Port C
  {
    const uint8_t number = (pin - 37) * -1;
    const uint8_t mask = (1 << number);

    return (PINC & mask) != 0;
  }
  else if (pin == 38) //Port D
  {
    const uint8_t mask = (1 << 7);
    return (PIND & mask) != 0;
  }
  else if (pin >= 39 && pin <= 41) //Port G
  {
    const uint8_t number = (pin - 41) * -1;
    const uint8_t mask = (1 << number);

    return (PING & mask) != 0;
  }
  else if (pin >= 42 && pin <= 49) //Port L
  {
    const uint8_t number = (pin - 49) * -1;
    const uint8_t mask = (1 << number);

    return (PINL & mask) != 0;
  }
  else if (pin >= 50 && pin <= 53) //Port B
  {
    const uint8_t number = (pin - 53) * -1;
    const uint8_t mask = (1 << number);

    return (PINB & mask) != 0;
  }
  else
    return -1;
}

void set_pwm(int pin, uint8_t value)
{

  if ((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46))
    set_mode(pin, OUTPUT);
  else
    return;

  //SET PWM
  //freq = 16000/(255*prescalar*2)
  //Impostazioni per i timer a 8 bit
  const uint8_t sett_a_8 = (1 << WGM11) | (1 << WGM10); //PWM setting
  const uint8_t sett_b_8 = (1 << CS00);                 //Prescalar /\, frequenza 31.3 khz
  //Impostazioni per timer a 16 bit
  const uint8_t sett_a_16 = 1 << WGM30;
  const uint8_t sett_b_16 = (sett_b_8 | (1 << WGM12));

  switch (pin)
  {
  case 2: //PE4 OC3B 16 bit
    TCCR3A |= (1 << COM3B1) | sett_a_16;
    TCCR3B |= sett_b_16;
    OCR3BH = 0;
    OCR3BL = value;
    break;
  case 3: //PE5 OC3C 16bit
    TCCR3A |= 1 << COM3C1 | sett_a_16;
    TCCR3B |= sett_b_16;
    OCR3CH = 0;
    OCR3CL = value;
    break;
  case 4: //OC0B  8 bit
    TCCR0A |= 1 << COM0B1 | sett_a_8;
    TCCR0B |= sett_b_8;
    OCR0B = value;
    break;
  case 5: //PE3 OC3A  16 bit
    TCCR3A |= 1 << COM3A1 | sett_a_16;
    TCCR3B |= sett_b_16;
    OCR3AH = 0;
    OCR3AL = value;
    break;
  case 6: //Ph3 OC4A 16 bit
    TCCR4A |= 1 << COM4A1 | sett_a_16;
    TCCR4B |= sett_b_16;
    OCR4AH = 0;
    OCR4AL = value;
    break;
  case 7: //Ph4 OC4b 16 bit
    TCCR4A |= 1 << COM4B1 | sett_a_16;
    TCCR4B |= sett_b_16;
    OCR4BH = 0;
    OCR4BL = value;
    break;
  case 8: //Ph5 OC4c 16bit
    TCCR4A |= 1 << COM4C1 | sett_a_16;
    TCCR4B |= sett_b_16;
    OCR4CH = 0;
    OCR4CL = value;
    break;
  case 9: //Ph6 OC2b 8 bit
    TCCR2A |= 1 << COM2B1 | sett_a_8;
    TCCR2B |= sett_b_8;
    OCR2B = value;
    break;
  case 10: //PB4 OC2A 8 bit
    TCCR2A |= 1 << COM2A1 | sett_a_8;
    TCCR2B |= sett_b_8;
    OCR2A = value;
    break;
  case 11: //PB5 OC1A 16 bit
    TCCR1A |= 1 << COM1A1 | sett_a_16;
    TCCR1B |= sett_b_16;
    OCR1AH = 0;
    OCR1AL = value;
    break;
  case 12: //PB6 OC1B 16 bit
    TCCR1A |= 1 << COM1B1 | sett_a_16;
    TCCR1B |= sett_b_16;
    OCR1BH = 0;
    OCR1BL = value;
    break;
  case 13: //PB7 OC0A 8 bit
    TCCR0A |= 1 << COM0A1 | sett_a_8;
    TCCR0B |= sett_b_8;
    OCR0A = value;
    break;
  case 44: //PL5 OC5C
    TCCR5A |= 1 << COM5C1 | sett_a_16;
    TCCR5B |= sett_b_16;
    OCR5CH = 0;
    OCR5CL = value;
    break;
  case 45: //PL4 OC5B
    TCCR5A |= 1 << COM5B1 | sett_a_16;
    TCCR5B |= sett_b_16;
    OCR5BH = 0;
    OCR5BL = value;
    break;
  case 46: //PL3 OC5A
    TCCR5A |= 1 << COM5A1 | sett_a_16;
    TCCR5B |= sett_b_16;
    OCR5AH = 0;
    OCR5AL = value;
    break;
  }
}

int analog_read(uint8_t pin)
{

  //determina il fattore di divisione tra il clock adc e xtral attualmente 128
  ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  //prende come riferimento il voltaggio interno
  ADCSRB = 0;
  ADMUX = 1 << REFS0; //| 1<<REFS1;

  ADMUX &= ~(1 << MUX4) & ~(1 << MUX3);

  //abilità l'adc
  ADCSRA |= (1 << ADEN);

  if (pin == 97 || pin == 89)
  {

    if (pin == 97)
    {
      //A0
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A8
      ADCSRB |= (1 << MUX5);
    }
    //000
    ADMUX &= ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
  }
  else if (pin == 96 || pin == 88)
  {

    if (pin == 96)
    {
      //A1
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A9
      ADCSRB |= (1 << MUX5);
    }

    //001
    ADMUX &= ~(1 << MUX2) & ~(1 << MUX1);
    ADMUX |= (1 << MUX0);
  }
  else if (pin == 95 || pin == 87)
  {

    if (pin == 95)
    {
      //A2
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A10
      ADCSRB |= (1 << MUX5);
    }

    //010
    ADMUX &= ~(1 << MUX2) & ~(1 << MUX0);
    ADMUX |= (1 << MUX1);
  }
  else if (pin == 94 || pin == 86)
  {

    if (pin == 94)
    {
      //A3
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A11
      ADCSRB |= (1 << MUX5);
    }

    //011
    ADMUX &= ~(1 << MUX2);
    ADMUX |= (1 << MUX1) | (1 << MUX0);
  }
  else if (pin == 93 || pin == 85)
  {

    if (pin == 93)
    {
      //A4
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A12
      ADCSRB |= (1 << MUX5);
    }

    //100

    ADMUX |= (1 << MUX2);
    ADMUX &= ~(1 << MUX1) & ~(1 << MUX0);
  }
  else if (pin == 92 || pin == 84)
  {

    if (pin == 92)
    {
      //A5
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A13
      ADCSRB |= (1 << MUX5);
    }

    //101
    ADMUX |= (1 << MUX2) | (1 << MUX0);
    ADMUX &= ~(1 << MUX1);
  }
  else if (pin == 91 || pin == 83)
  {

    if (pin == 91)
    {
      //A6
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A14
      ADCSRB |= (1 << MUX5);
    }

    //110
    ADMUX |= (1 << MUX2) | (1 << MUX1);
    ADMUX &= ~(1 << MUX0);
  }
  else if (pin == 90 || pin == 82)
  {

    if (pin == 90)
    {
      //A7
      ADCSRB &= ~(1 << MUX5);
    }
    else
    {
      //A15
      ADCSRB |= (1 << MUX5);
    }

    //111
    ADMUX |= (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  }
  else
  {
    //Pin senza adc
    return -1;
  }
  //creo una maschera che poi userò per controllare che adsc sia tornato a 0
  //cioè quando la conversione è finita
  uint8_t mask = 0;
  mask |= (1 << ADSC);

  //imposto il triggger a 1
  ADCSRA |= (1 << ADSC);

  //devo aspettare che adsc in adcsra sia posto a 0
  while (ADCSRA & mask)
    ;

  //bisogna leggere prima adcl se no adch si blocca
  const uint8_t low = ADCL;
  const uint8_t high = ADCH;

  return (high << 8) | low;
}

void remove_min_and_max(int *data, size_t size, int replace_with)
{
  int min = 9999;
  int max = 0;
  int min_index = 0, max_index = 0;
  for (int i = 0; i < size; i++)
  {
    if (data[i] != -1 && data[i] > max)
    {
      max = data[i];
      max_index = i;
    }
    if (data[i] != -1 && data[i] < min)
    {
      min = data[i];
      min_index = i;
    }
  }
  data[min_index] = replace_with;
  data[max_index] = replace_with;
}

int filtered_analog_read(uint8_t pin)
{
  uint32_t best_read = 0;
  int reads[32];
  for (int i = 0; i < 32; i++)
    reads[i] = analog_read(pin);

  for (int i = 0; i < 8; i++)
    remove_min_and_max(reads, 32, -1);

  uint8_t k = 0;
  for (int i = 0; i < 32; i++)
  {
    if (reads[i] != -1)
    {
      best_read += reads[i];
      k++;
    }
  }

  return best_read / k;
}
