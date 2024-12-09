#include <Arduino.h>

// Configuração do PWM no pino PB1 (Digital 9)
void PWM_init() {
  DDRB |= (1 << PB1); // Configura o pino PB1 (D9) como saída
  
  // Configura o Timer1 no modo Fast PWM com TOP = ICR1
  TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Modo não-inversor no OC1A
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8, Fast PWM com ICR1
  ICR1 = 2000;  // Define o TOP (freq = 16 MHz / (8 * 2000) = 1000 Hz)
  OCR1A = 1000; // Define o duty cycle (50%)
}

// Função para desligar o PWM
void offPWM() {
  TCCR1A &= ~(1 << COM1A1); // Desativa o PWM no OC1A
  PORTB &= ~(1 << PB1);     // Garante que PB1 está em LOW
}

// Função para ligar o PWM
void onPWM() {
  TCCR1A |= (1 << COM1A1);  // Ativa o PWM no OC1A
}

// Configuração inicial
void setup() {
  PWM_init(); // Inicializa o PWM
}

// Loop principal
void loop() {
  onPWM();             // Liga o PWM
  delay(2000);         // Aguarda 2 segundos
  offPWM();            // Desliga o PWM
  delay(2000);         // Aguarda mais 2 segundos
}
