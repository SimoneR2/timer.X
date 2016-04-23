#include <xc.h>
#include "timer.h"
#define  _XTAL_FREQ 16000000
void configurazione(void);
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
    if (INTCONbits.INT0IF == 1) {
        if (PORTBbits.RB1 == 1) {
            secondi++;
            if (secondi > 60) {
                minuti++;
                secondi = 0;
            }
        } else {
            secondi--;
            if (secondi < 0) {
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
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        INTCONbits.TMR0IF = 0;
    }
}

unsigned char disp[4] = 0;

void main(void) {
    configurazione();
    while (1) {
        disp[1] = secondi / 10;
        disp[0] = secondi - (disp[1] * 10);
        disp[3] = minuti / 10;
        disp[2] = minuti - (disp[3] * 10);

        for (unsigned char i = 0; i < 4; i++) {
            PORTD = numeri[disp[i]];
            PORTC = porte[i];
            __delay_ms(2);
        }
        if ((PORTBbits.RB2 == 0)&&((secondi != 0) || (minuti != 0))) {
            PORTAbits.RA5 = 1;
            INTCONbits.INT0IE = 0; //disabilito possibilità di modificare il tempo
            INTCONbits.TMR0IF = 0;
            INTCONbits.TMR0IE = 1;
            TMR0H = 0x0B;
            TMR0L = 0xDC;
            T0CONbits.TMR0ON = 1;
        }
        if ((T0CONbits.TMR0ON == 1)&&(secondi == 0)&(minuti == 0)) {
            PORTAbits.RA5 = 0;
            for (unsigned char c = 0; c<10; c++){
            for (unsigned char i = 0; i < 4; i++) {
                PORTD = numeri[0];
                PORTC = porte[i];
                __delay_ms(2);
            }
            for (unsigned char b = 0; b > 10; b++) {
                __delay_ms(10);
            }
            PORTC = 0x00;
            for (unsigned char f = 0; f > 10; f++) {
                __delay_ms(10);
            }
        }
        }
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