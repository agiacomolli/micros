#include <avr/io.h>
#include <util/delay.h>

/* Configura o modo. */
void lcd_set_mode(char mode)
{
    /* 0 para comando; 1 para dado. */

    if (mode) {
        PORTG |= 1 << 1;
    } else {
        PORTG &= ~(1 << 1);
    }
}

/* Escreve um dado no barramento do LCD. */
void lcd_write_char(unsigned char c)
{
    /* RS = 1 */
    lcd_set_mode(1);

    /* Escreve um dado no barramento. */
    PORTA = c;

    /* Pulsa o pino do Enable. */
    PORTG |= 1 << 0;
    PORTG &= ~(1 << 0);

    _delay_us(40);
}

/* Escreve um comando no barramento do LCD. */
void lcd_write_cmd(unsigned char cmd)
{
    /* RS = 0 */
    lcd_set_mode(0);

    /* Coloca o valor do comando na porta. */
    PORTA = cmd;

    /* Pulsa o pino do Enable. */
    PORTG |= 1 << 0;
    PORTG &= ~(1 << 0);

    _delay_us(40);
}

/* Limpa o LCD. */
void lcd_clear(void)
{
    lcd_write_cmd(0x01);
    _delay_ms(2);
}

/* Liga ou desliga o cursor. */
void lcd_set_cursor(char on)
{
    char cmd = 0x0C;

    if (on) {
        cmd |= 1 << 1;
    }

    lcd_write_cmd(cmd);
}

/* Escreve uma string no LCD. */
void lcd_write_str(char *str)
{
    while (*str) {
        lcd_write_char(*str);
        str++;
    }
}

/*
 * Inicializa o LCD.
 *
 * Executa os comandos definidos na inicialização
 * do LCD (manual do fabricante).
 */
void lcd_init(void)
{
    /* Configura barramento de comunicação
     * com o LCD como saída. */
    DDRA = 0xFF;

    /* Configura pinos de controle do LCD
     * como saída.
     *
     * PG0 = Enable
     * PG1 = RS
     */
    DDRG = (1 << 1) | (1 << 0);

    /* Configura pinos de controle como 0. */
    PORTG &= ~(1 << 1) & ~(1 << 0);

    /* Espera tempo para estabilização da
     * tensão de alimentação do LCD. */
    _delay_ms(40);

    /* Configura RS como 0. */
    PORTG &= ~(1 << 1);

    /* Escreve o comando no barramento. */
    lcd_write_cmd(0x30);
    _delay_ms(5);

    /* Escreve o comando no barramento. */
    lcd_write_cmd(0x30);
    _delay_ms(1);

    /* Escreve o comando no barramento. */
    lcd_write_cmd(0x30);
    _delay_ms(1);

    /* Configura o LCD para trabalhar com 8
     * bot de barramento, duas linhas de
     * informação e fonte 5x10. */
    lcd_write_cmd(0x3C);

    /* Liga o LCD e configura o cursor. */
    lcd_write_cmd(0x0E);

    /* Limpa a tela do LCD. */
    lcd_clear();

    /* Configura o incremento para direita do
     * cursor em cada escrita na tela. */
    lcd_write_cmd(0x06);
}

/* Posiciona o cursor na tela do LCD. */
void lcd_set_pos(int lin, int col)
{

}

int main(void)
{
    DDRE = 1 << 3;

    lcd_init();

    lcd_set_cursor(0);

    lcd_write_str("casa");

    while (1) {
        PORTE ^= 1 << 3;
        _delay_ms(1000);
    }

    return 0;
}
