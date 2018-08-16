/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file contains the initialization for UART along with the functions to send messages and a character
 *
 **/

#include "uart.h"

inline void initUART(void) {
    SYNC = 0;
    SPEN = TXEN = CREN = BRG16 = BRGH = 1;
    SPBRG = BAUD_CONFIG;
    RCIF = 0;
    RCIE = 1;
}

inline void sendMessage(unsigned char * i) {
    while (*i != '\0') {
        sendChar(*i);
        i++;
    }
}

inline void sendChar(unsigned char outchr) {
    while (TXIF == 0);
    TXREG = outchr;
}