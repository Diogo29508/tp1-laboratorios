#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BUTTON PD2  // Botão para ativar Morse
#define BUZZER PB1  // Pino do buzzer
#define LED PB2      // LED para indicar transmissão
#define DOT_TIME 100
#define DASH_TIME 300
#define LETTER_SPACE_TIME 700

void setup() {
    DDRB |= (1 << BUZZER) | (1 << LED); // Configura o buzzer e LED como saída
    DDRD &= ~(1 << BUTTON); // Configura o botão como entrada
    PORTD |= (1 << BUTTON); // Habilita pull-up no botão
}

void beep(int duration) {
    PORTB |= (1 << BUZZER) | (1 << LED);
    _delay_ms(duration);
    PORTB &= ~(1 << BUZZER) & ~(1 << LED);
    _delay_ms(DOT_TIME);
}

void transmitMorse(char c) {
    switch (c) {
        case 'A': beep(DOT_TIME); beep(DASH_TIME); break;
        case 'B': beep(DASH_TIME); beep(DOT_TIME); beep(DOT_TIME); beep(DOT_TIME); break;
        case 'C': beep(DASH_TIME); beep(DOT_TIME); beep(DASH_TIME); beep(DOT_TIME); break;
        default: break;
    }
    _delay_ms(LETTER_SPACE_TIME);
}

char detectButtonPress() {
    if (!(PIND & (1 << BUTTON))) {
        _delay_ms(50); // Debounce
        if (!(PIND & (1 << BUTTON))) {
            return 'A'; // Simula a letra 'A'
        }
    }
    return 0;
}

int main() {
    setup();
    while (1) {
        char letter = detectButtonPress();
        if (letter) {
            transmitMorse(letter);
            _delay_ms(1000);
        }
    }
}
