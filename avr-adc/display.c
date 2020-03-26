#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

/* Define a tabela de conversão de valores para a combinação de acionamento dos
 * segmentos do mostrador. */
const unsigned char table[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6,
	0xBE, 0xE0, 0xFE, 0xF6 };

/* Pulso de clock no 74595. */
void clock(void)
{
	PORTB |= 1 << 7;
	PORTB &= ~(1 << 7);
}

/* Pulso de latch no 74595. */
void latch(void)
{
	PORTB |= 1 << 4;
	PORTB &= ~(1 << 4);
}

/* Envia o bit indicado por pos do valor val. */
void send_bit(unsigned char val, int pos)
{
	if (val & (1 << pos)) {
		PORTB |= 1 << 5;
	} else {
		PORTB &= ~(1 << 5);
	}
}

/* Envia um valor de 8 bits com a combinação de códigos para acionar os
 * segmentos do mostrador. */
void write_code(unsigned char code)
{
	int i;

	/* Envia os 8 bits. */
	for (i = 0; i < 8; i++) {
		send_bit(code, i);
		clock();
	}

	/* Pulso no latch para aparecer o resultado. */
	latch();
}

/* Escreve o número indicado por num na posição do mostrador indicado por
 * pos. */
void display_write(int num, int pos)
{
	/* No caso de pos diferente de zero, seta o bit zero (QH) do valor de
	 * conversão da tabela. */
	if (pos == 0) {
		write_code(table[num]);
	} else {
		write_code(table[num] | 0x01);
	}
}

void display_init(void)
{
	/* Configuração dos pinos de saída.
	 * PB4 = LATCH
	 * PB5 = INPUT
	 * PB7 = CLOCK
	 */
	DDRB |= (1 << 4) | (1 << 5) | (1 << 7);
}

