#include <Arduino.h>

// Pinos
#define BUTTON_PIN 8 // Pino digital para o botão
#define PWM_PIN 9    // Pino PWM (PB1) para o buzzer
#define LED_PIN 10   // Pino digital para o LED

// Variáveis de controle do pitch
int pitch = 2000; // Frequência inicial (TOP do Timer1)

// Inicialização do PWM no pino PB1 (Digital 9)
void PWM_init() {
  DDRB |= (1 << PB1); // Configura PB1 (D9) como saída

  // Configura Timer1 no modo Fast PWM com ICR1 como TOP
  TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Modo não inversor no OC1A
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8, Fast PWM com ICR1
  ICR1 = pitch;  // Define o TOP inicial
  OCR1A = pitch / 2; // Define o duty cycle (50%)
}

// Função para atualizar o pitch
void updatePitch() {
  if (pitch < 100) pitch = 100;   // Limite inferior do pitch
  if (pitch > 4000) pitch = 4000; // Limite superior do pitch
  ICR1 = pitch;                   // Atualiza a frequência
  OCR1A = pitch / 2;              // Mantém o duty cycle em 50%
}

// Função para desligar o PWM
void offPWM() {
  TCCR1A &= ~(1 << COM1A1); // Desativa o PWM no OC1A
  PORTB &= ~(1 << PB1);     // Garante que PB1 está em LOW
  digitalWrite(LED_PIN, LOW); // Desliga o LED
}

// Função para ligar o PWM
void onPWM() {
  TCCR1A |= (1 << COM1A1); // Ativa o PWM no OC1A
  digitalWrite(LED_PIN, HIGH); // Liga o LED
}

// Mapeamento dos caracteres para código Morse
const char *morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // a-j
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",  // k-t
  "..-", "...-", ".--", "-..-", "-.--", "--.."                          // u-z
};

// Função para tocar um sinal curto (.)
void dot() {
  onPWM();
  delay(200); // 200 ms para um ponto
  offPWM();
  delay(200); // Espaço entre sinais
}

// Função para tocar um sinal longo (-)
void dash() {
  onPWM();
  delay(600); // 600 ms para um traço
  offPWM();
  delay(200); // Espaço entre sinais
}

// Função para reproduzir um código Morse
void playMorse(const char *code) {
  while (*code) {
    if (*code == '.') {
      dot();
    } else if (*code == '-') {
      dash();
    }
    code++;
  }
  delay(600); // Pausa entre caracteres
}

// Configuração inicial
void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o botão como entrada com pull-up
  pinMode(LED_PIN, OUTPUT); // Configura o LED como saída
  PWM_init(); // Inicializa o PWM
}

// Loop principal
void loop() {
  if (Serial.available() > 0) { // Verifica se há dados recebidos
    char received = Serial.read(); // Lê o caractere recebido

    if (received >= 'a' && received <= 'z') { // Verifica se é uma letra minúscula
      const char *code = morseCode[received - 'a']; // Obtém o código Morse da letra
      playMorse(code); // Reproduz o código Morse
    } else if (received == 'L') { // Comando para ligar o buzzer continuamente
      onPWM(); // Liga o buzzer continuamente
    } else if (received == 'D') { // Comando para desligar o buzzer
      offPWM(); // Desliga o buzzer
    } else if (received == '+') { // Comando para aumentar o pitch
      pitch -= 100; // Reduz o valor de ICR1 para aumentar a frequência
      updatePitch();
    } else if (received == '-') { // Comando para diminuir o pitch
      pitch += 100; // Aumenta o valor de ICR1 para diminuir a frequência
      updatePitch();
    }
  }
}
