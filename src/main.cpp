#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define LED_PIN PB5

unsigned int ubrr = F_CPU / 16 / BAUD - 1;

int main(void) {
  DDRB |= (1 << LED_PIN);  // Configura PB5 como saída
  UBRR0H = ubrr >> 8;      // Configura a taxa de baud
  UBRR0L = ubrr;
  UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);  // Habilita RX, TX e interrupção
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                // 8 bits de dados
  sei();  // Habilita interrupções globais

  while(1);  // Espera por interrupções

  return 0;
}

ISR(USART_RX_vect) {
  if (UDR0 == 'D') PORTB &= ~(1 << LED_PIN);  // Desliga LED
  if (UDR0 == 'L') PORTB |= (1 << LED_PIN);   // Liga LED
}
