/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * Header file for eeprom.c
 *
 **/

#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h>
#include "main.h"

// Preprocessor Directives
#define ALT         settings->alt
#define COORD       settings->coord
#define SATS        settings->sats
#define UPPER(X)    (uint8_t) ((X & 0xFF00) >> 8)
#define LOWER(X)    (uint8_t) ((X & 0x00FF))
#define WRITE(X)    EEPROM_WRITE(*eepromPointer, X); *eepromPointer = *eepromPointer + 1;

// Enums for sending data
typedef enum {
    SEND_TIME_AMT = 3,
    SEND_ALT_AMT = 3,
    SEND_COORD_AMT = 10,
    SEND_SATS_AMT = 1
} SEND_AMT;

typedef enum {
    SEND_TIME = 0,
    SEND_ALT,
    SEND_COORD,
    SEND_SATS
} SEND;

// Function Prototypes
inline void sendData(SETTINGS *, uint8_t *);
inline uint8_t getLength(SETTINGS *);
inline uint8_t log(GPS *, SETTINGS *, uint8_t *);
inline void readConfig(SETTINGS *, uint8_t *);
inline void updateConfig(SETTINGS *);

#endif	/* EEPROM_H */

