#include <Arduino.h>

// Pinos
#define BUTTON_PIN 8 // Pino digital para o botão
#define PWM_PIN 9    // Pino PWM (PB1) para o buzzer
#define LED_PIN 10   // Pino digital para o LED

// Configuração do PWM no pino PB1 (Digital 9)
void PWM_init() {
  DDRB |= (1 << PB1); // Configura o pino PB1 (D9) como saída
  
  // Configura o Timer1 no modo Fast PWM com TOP = ICR1
  TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Modo não-inversor no OC1A
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8, Fast PWM com ICR1
  ICR1 = 2000;  // Define o TOP (freq = 16 MHz / (8 * 2000) = 1000 Hz)
  OCR1A = 1000; // Define o duty cycle (50%)
}

// Função para desligar o PWM (buzzer)
void offPWM() {
  TCCR1A &= ~(1 << COM1A1); // Desativa o PWM no OC1A
  PORTB &= ~(1 << PB1);     // Garante que PB1 está em LOW
}

// Função para ligar o PWM (buzzer)
void onPWM() {
  TCCR1A |= (1 << COM1A1);  // Ativa o PWM no OC1A
}

// Configuração inicial
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o botão como entrada com pull-up
  pinMode(LED_PIN, OUTPUT); // Configura o LED como saída
  PWM_init(); // Inicializa o PWM
  onPWM(); // Liga o PWM e o buzzer inicialmente
  digitalWrite(LED_PIN, HIGH); // Liga o LED inicialmente
}

// Loop principal
void loop() {
  // Lê o estado atual do botão
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) { // Botão pressionado
    offPWM(); // Desliga o buzzer
    digitalWrite(LED_PIN, LOW); // Desliga o LED
  } else { // Botão não pressionado
    onPWM(); // Religa o buzzer
    digitalWrite(LED_PIN, HIGH); // Religa o LED
  }
}