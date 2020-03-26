#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "display.h"

int ldr_read(void)
{
    unsigned short value;

    /*
     * leitura ad           porcentagem
     * 1023                 99
     * 1                    x
     *
     * x = 99/1023
     */

    value = adc_read(0);

    value = value * (99.0 / 1023.0);

    return value;
}

int lm35_read(void)
{
    unsigned short value;

    /* Implementar a lógica para cálculo de temperatura. */
    value = adc_read(2);

    /* Trocar para o cálculo correto. */
    return value;
}

int main(void)
{
    int t = 0;
    int b1_aux = 0;
    int leitura = 0;
    unsigned char dis = 0;
	unsigned char num = 0;
    unsigned short temperatura;
    unsigned short luminosidade;

    /* Configura e inicializa o ADC. */
    adc_init();

    /* Configura e inicializa o display de sete segmentos. */
    display_init();

    /* LEDs como saídas. */
    DDRE |= 1 << 3;
    DDRH |= (1 << 5) | (1 << 3);

	while (1) {
        temperatura = lm35_read();
        luminosidade = ldr_read();

        /* Apresentar informações no mostrador LCD. */
        if (dis) {
            display_write(num % 10, dis);
        } else {
            display_write(num / 10, dis);
        }
        dis ^= 1;

        if (t >= 100) {
            t = 0;
            PORTB ^= 1 << 0;

            /* Configura valor que aparecerá no display. */
            if (leitura == 0) {
                num = temperatura;
            } else if (leitura == 1) {
                num = luminosidade;
            }

            /* LEDs indicativos. */
            if (leitura == 0) {
                PORTE |= 1 << 3;
                PORTH &= ~(1 << 3);
            } else if (leitura == 1) {
                PORTE &= ~(1 << 3);
                PORTH |= 1 << 3;
            }
        }

        if (PINH & (1 << 4) && !b1_aux) {
            b1_aux = 1;
            leitura = !leitura;
        }

        if (!(PINH & (1 << 4)) && b1_aux) {
            b1_aux = 0;
        }

        t++;
		_delay_ms(10);
	}

	return 0;
}

