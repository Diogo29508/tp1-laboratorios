#include <avr/io.h>
#include <util/delay.h>

#define led_pin PB5

void setup(){
  DDRB |= (1 << led_pin);

 }

int main()

{
  setup();

  PORTB &= ~(1<<led_pin);
 


 return 0;

}
