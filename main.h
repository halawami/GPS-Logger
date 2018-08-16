/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * Header file for main.c
 *
 **/

#include <xc.h>
#include "stdint.h"
#include "lcd.h"
#include "stdio.h"
#include "uart.h"
#include "tmr1.h"
#include "buttons.h"
#include "configbits.h"
#include "eeprom.h"
#include "gps.h"

#ifndef MAIN_H
#define	MAIN_H

// Preprocessor Directives
#define KEEP    0
#define CLEAR   1

#define CHANGE(X)   X ^= 1; updateConfig(&settings);
#define INC(X, Y)   X++; X %= Y;

typedef struct {
    uint8_t rate;
    uint8_t coord;
    uint8_t alt;
    uint8_t sats;
} SETTINGS;

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t lat_int;
    uint16_t lat_dec;
    uint16_t long_int;
    uint16_t long_dec;
    char lat_symbol;
    char long_symbol;

    uint8_t sats;
    uint16_t alt_int;
    uint8_t alt_dec;
} GPS;

uint8_t routine(void);

#endif	/* MAIN_H */

