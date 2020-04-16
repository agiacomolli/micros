#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void eeprom_write(int addr, unsigned char data)
{
    cli();

    /* Aguarda a finalização de uma possível escrita. */
    while (EECR & (1 << EEPE));

    /* Configura o endereço de escrita e o valor a ser escrito. */
    EEARH = addr >> 8;
    EEARL = addr;
    EEDR = data;

    /* Habilita memória para uma nova escrita. */
    EECR |= 1 << EEMPE;

    /* Inicia a escrita na memória EEPROM. */
    EECR |= 1 << EEPE;

    sei();
}

unsigned char eeprom_read(int addr)
{
    unsigned char data;

    cli();

    /* Aguarda a finalização de uma possível escrita. */
    while (EECR & (1 << EEPE));

    /* Configura o endereço de leitura. */
    EEARH = addr >> 8;
    EEARL = addr;

    /* Inicia uma nova leitura da memória EEPROM. */
    EECR |= 1 << EERE;

    /* Retorna o resultado da leitura da memória. */
    data = EEDR;

    sei();

    return data;
}

int main(void)
{
    unsigned int addr = 0;
    unsigned char data = 10;

    cli();

    DDRE = 1 << DDE3;

    sei();

	while (1) {
        eeprom_write(addr, data);

        addr++;
        data++;

        PORTE ^= 1 << PE3;

        _delay_ms(1000);
	}

	return 0;
}

