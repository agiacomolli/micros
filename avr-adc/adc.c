#include <avr/io.h>
#include "adc.h"

void adc_init(void)
{
    ADMUX = 0x00;

    /* Configura a referência de tensão para usar a tensão de alimentação 5 V.
     * REFS0 = 1
     * REFS1 = 0
     */
    ADMUX |= 1 << 6;

    /* Confiura o resultado alinhado à direita.
     * ADLAR = 0
     */
    ADMUX &= ~(1 << 5);

    /* Inicializa registradores zerados. */
    ADCSRA = 0x00;

    /* Configura a taxa de conversão com fator de divisão do clock para 128.
     * ADPS2 = 1
     * ADPS1 = 1
     * ADPS0 = 1
     */
    ADCSRA |= (1 << 2) | (1 << 1) | (1 << 0);

    /* Inicializa configuração dos módulos digitais dos canais analógicos. */
    DIDR0 = 0x00;
    DIDR2 = 0x00;

    /* Desabilita o circuito digital dos canais CH0, CH1 e CH2. */
    DIDR0 |= (1 << 2) | (1 << 1) | (1 << 0);

    /* Ligar o conversor analógico.
     * ADEN = 1
     */
    ADCSRA |= 1 << 7;
}

unsigned short adc_read(int channel)
{
    unsigned short value;

    /* Limpar os bits de configuração MUX do ADMUX. */
    ADMUX &= 0xE0; //1110 0000

    /* Escreve o valor do canal selecionado. */
    ADMUX |= channel & 0x1F; // 0001 1111

    /* Inicia uma nova conversão no canal selecionado.
     * ADSC = 1
     */
    ADCSRA |= 1 << 6;

    /* Aguarda a conversão terminar.
     * Enquanto ADIF == 0
     */
    while (!(ADCSRA & (1 << 4))) {}

    /* Lê o resultado da conversão. */
    value = ADCL;
    value = (ADCH << 8) | value;

    return value;
}

