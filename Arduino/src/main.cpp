/*
Sets up a variable pwm on PB1 - Digital 9
through OC1A pin 🙂
first on 1000 Hz
*/
void PWM_init()
{
    DDRB |= (1 << PB1); // Sets portB 1 as output
                        // Configure Timer1 for Fast PWM mode with ICR1 as TOP
    TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Non-inverting mode on OC1A/OC1B
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8, Fast PWM with ICR1
    ICR1 = 2000;
    OCR1A = 1000;
}

void offPWM()
{
    TCCR1A &= ~(1 << COM1A1);
    // Set output pins low
    PORTB &= ~(1 << PB1); // Ensure PB1 and PB2 are low
}
void onPWM()
{
    TCCR1A |= (1 << COM1A1);
}