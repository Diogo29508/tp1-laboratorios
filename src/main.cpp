#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL

#define BUTTON_UP PD2       // Botão para aumentar a frequência
#define BUTTON_DOWN PD3     // Botão para diminuir a frequência
#define LED_PIN PB2         // Pino do LED

#define DOT_TIME 100        // Tempo para um ponto no código Morse (ms)
#define DASH_TIME 300       // Tempo para um traço (ms)
#define LETTER_SPACE_TIME 700   // Espaço entre letras no Morse (ms)
#define WORD_SPACE_TIME 1400    // Espaço entre palavras no Morse (ms)

void USART_init() {
    unsigned int ubrr = F_CPU / 16 / 9600 - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita recepção e transmissão
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de dados
}

void USART_send(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

char USART_receive_nonblocking() {
    if (UCSR0A & (1 << RXC0)) {
        return UDR0;
    }
    return 0; // Retorna 0 se não há dados disponíveis
}

void setupPWM() {
    DDRB |= (1 << PB1); // Configura PB1 como saída para o buzzer
    DDRB |= (1 << LED_PIN); // Configura PB2 como saída para o LED
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
}

void setFrequency(uint16_t frequency) {
    uint16_t topValue = (16000000 / (8 * frequency)) - 1;
    ICR1 = topValue;
    OCR1A = topValue / 2;
}

void onPWM() {
    TCCR1A |= (1 << COM1A1); // Liga o buzzer (PWM)
    PORTB |= (1 << LED_PIN); // Liga o LED
}

void offPWM() {
    TCCR1A &= ~(1 << COM1A1); // Desliga o buzzer (PWM)
    PORTB &= ~(1 << PB1);     // Garante que o buzzer está em nível baixo
    PORTB &= ~(1 << LED_PIN); // Desliga o LED
}

void setupButtons() {
    DDRD &= ~(1 << BUTTON_UP);
    PORTD |= (1 << BUTTON_UP);
    DDRD &= ~(1 << BUTTON_DOWN);
    PORTD |= (1 << BUTTON_DOWN);
}

const char* morseCode(char c) {
    switch (c) {
        case 'A': return ".-"; case 'B': return "-..."; case 'C': return "-.-.";
        case 'D': return "-.."; case 'E': return "."; case 'F': return "..-.";
        case 'G': return "--."; case 'H': return "...."; case 'I': return "..";
        case 'J': return ".---"; case 'K': return "-.-"; case 'L': return ".-..";
        case 'M': return "--"; case 'N': return "-."; case 'O': return "---";
        case 'P': return ".--."; case 'Q': return "--.-"; case 'R': return ".-.";
        case 'S': return "..."; case 'T': return "-"; case 'U': return "..-";
        case 'V': return "...-"; case 'W': return ".--"; case 'X': return "-..-";
        case 'Y': return "-.--"; case 'Z': return "--..";
        case '1': return ".----"; case '2': return "..---"; case '3': return "...--";
        case '4': return "....-"; case '5': return "....."; case '6': return "-....";
        case '7': return "--..."; case '8': return "---.."; case '9': return "----.";
        case '0': return "-----"; case ' ': return " ";
        default: return "";
    }
}

void transmitMorse(const char* message) {
    while (*message) {
        const char* morse = morseCode(*message);
        while (*morse) {
            if (*morse == '.') {
                onPWM();
                _delay_ms(DOT_TIME);
            } else if (*morse == '-') {
                onPWM();
                _delay_ms(DASH_TIME);
            }
            offPWM();
            _delay_ms(DOT_TIME);
            morse++;
        }
        _delay_ms(LETTER_SPACE_TIME);
        message++;
    }
    _delay_ms(WORD_SPACE_TIME);
}

int main() {
    USART_init();
    setupPWM();
    setupButtons();

    uint16_t frequency = 1000;
    setFrequency(frequency);

    char receivedCommand = 0;

    while (1) {
        if (!(PIND & (1 << BUTTON_UP))) {
            frequency += 100;
            if (frequency > 2000) frequency = 2000;
            setFrequency(frequency);
            _delay_ms(200);
        }

        if (!(PIND & (1 << BUTTON_DOWN))) {
            frequency -= 100;
            if (frequency < 500) frequency = 500;
            setFrequency(frequency);
            _delay_ms(200);
        }

        receivedCommand = USART_receive_nonblocking();
        if (receivedCommand) {
            if (receivedCommand == 'l') {
                onPWM();
            } else if (receivedCommand == 'd') {
                offPWM();
            } else {
                char buffer[2] = {receivedCommand, '\0'};
                transmitMorse(buffer);
            }
        }
    }
}