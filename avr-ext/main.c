#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT5_vect)
{
    PORTE ^= 1 << PE3;
}

int main(void)
{
    cli();

    DDRE = 1 << DDE3;

    /* Configura INT5 para interrupção na borda de subida. */
    EICRB = (1 << ISC51) | (1 << ISC50);

    /* Habilita interrupções no pino INT5. */
    EIMSK = 1 << INT5;

    /* Habilita interrupções globais. */
    //SREG |= 1 << 7;
    sei();

    while (1) {
        _delay_ms(10);
    }

    return 0;
}

