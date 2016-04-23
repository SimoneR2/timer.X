#include <xc.h>
#include "timer.h"
#define  _XTAL_FREQ 16000000
void configurazione(void);
volatile unsigned char i = 0;
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
    if (PORTBbits.RB1 == 1) {
            i++;
    } else {
            i--;
    }
    INTCONbits.INT0IF = 0;
}
unsigned char centinaia, decine, unita = 0;
void main(void) {
    configurazione();
    PORTCbits.RC0 = 1;
    PORTD = 0xff;
    while (1) {
        for (unsigned char a = 0; a < 4; a++) {
            PORTD = numeri[i];
            PORTC = porte[a];
            __delay_us(10);
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

    //CMCON = 0x07;

}