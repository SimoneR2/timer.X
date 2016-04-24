#include <xc.h>
#include "timer.h"
#define  _XTAL_FREQ 16000000
#define buzzer PORTCbits.RC6
#define segnale PORTAbits.RA0
void configurazione(void);
void delay(unsigned char ritardo);
volatile signed char secondi, minuti = 0;
unsigned char numeri [] = {
    0b11001111,
    0b10000010,
    0b01100111,
    0b10100111,
    0b10101010,
    0b10101101,
    0b11101101,
    0b10000110,
    0b11101111,
    0b10101111
};
unsigned char porte [] = {
    0b00000001,
    0b00000010,
    0b00000100,
    0b10000000
};

__interrupt(high_priority) void ISR_alta(void) {
    if ((INTCONbits.INT0IF == 1)&&(INTCONbits.TMR0IF == 0)) {
        if (PORTBbits.RB1 == 0) {
            secondi++;
            if (secondi > 59) {
                minuti++;
                secondi = 0;
            }
        } else {
            secondi--;

            if ((secondi < 0)&&(minuti > 0)) {
                minuti--;
                secondi = 59;
            }
        }
        INTCONbits.INT0IF = 0;
    }
    
    if (INTCONbits.TMR0IF == 1) {
        secondi--;
        if (secondi < 0) {

            minuti--;
            secondi = 59;
        }
        segnale = ~segnale;
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        INTCONbits.TMR0IF = 0;
    }
}

unsigned char disp[4] = 0;

void main(void) {
    configurazione();
    minuti = eeprom_read(0xE5);
    secondi = eeprom_read(0xE6);
    buzzer = 1;
    for (unsigned char i = 0; i < 200; i++) {
        __delay_ms(10);
    }
    buzzer = 0;

    while (1) {
        disp[3] = secondi / 10;
        disp[2] = secondi - (disp[3] * 10);
        disp[0] = minuti / 10;
        disp[1] = minuti - (disp[0] * 10);

        for (unsigned char i = 0; i < 4; i++) {
            PORTD = numeri[disp[i]];
            if (i == 4) {
                PORTDbits.RD4 = 1;
            }
            PORTC = porte[i];
            __delay_ms(2);
        }
        if ((PORTBbits.RB2 == 0)&&((secondi != 0) || (minuti != 0))) {
            eeprom_write(0xE5, minuti);
            eeprom_write(0xE6, secondi);
            PORTAbits.RA5 = 1;
            INTCONbits.INT0IE = 0; //disabilito possibilità di modificare il tempo
            INTCONbits.TMR0IF = 0;
            INTCONbits.TMR0IE = 1;
            TMR0H = 0x0B;
            TMR0L = 0xDC;
            T0CONbits.TMR0ON = 1;
        }
        if ((T0CONbits.TMR0ON == 1)&&(secondi == 0)&(minuti == 0)) {
            T0CONbits.TMR0ON = 0;
            PORTAbits.RA5 = 0;
            for (unsigned char c = 0; c < 10; c++) {
                for (unsigned char s = 0; s < 100; s++) {
                    for (unsigned char i = 0; i < 4; i++) {
                        PORTD = numeri[0];
                        PORTC = porte[i];
                        __delay_ms(2);
                    }
                }
                for (unsigned char i = 0; i < 4; i++) {
                    PORTD = 0x00;
                    PORTC = porte[i];
                    __delay_ms(2);
                }
                delay(100);
                INTCONbits.INT0IE = 1; //riabilito i tempi
            }
        }
    }
}

void delay(unsigned char ritardo) {
    ritardo = ritardo / 10;
    while (ritardo > 0) {
        __delay_ms(10);
        ritardo--;
    }
}

void configurazione(void) {
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1;
    LATA = 0;
    TRISA = 0b11000000;

    LATB = 0;
    TRISB = 0xFF;

    LATC = 0;
    TRISC = 0x00;

    LATD = 0;
    TRISD = 0x00;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 0;
    RCONbits.IPEN = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 1;
    T0CON = 0x85;
    T0CONbits.TMR0ON = 0; //disattiva il timer

    //CMCON = 0x07;

}