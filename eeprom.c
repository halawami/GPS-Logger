/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file contains functions to read from and write data to the EEPROM using Microchip's EEPROM functions
 *
 **/

#include "eeprom.h"

uint8_t buffer[20];

inline void sendData(SETTINGS * settings, uint8_t * eepromPointer) {
    // Send Data in Memory
    sendMessage("Memory Dump:\n");

    uint8_t new = 1;
    uint8_t remaining = SEND_TIME_AMT;
    uint8_t current = SEND_TIME;

    for (int i = 2; i <= *eepromPointer; i++) {

        // Label which data item is being sent
        if (new) {
            new = 0;
            switch (current) {
                case SEND_TIME:
                    sendMessage("New Data Point\n");
                    sendMessage("Time:\n");
                    break;
                case SEND_ALT:
                    sendMessage("Altitude:\n");
                    break;
                case SEND_COORD:
                    sendMessage("Coordinates:\n");
                    break;
                case SEND_SATS:
                    sendMessage("Satellites:\n");
                    break;
            }
        }

        // Prepare the item to send
        sprintf(buffer, "%d\n", EEPROM_READ(i));

        remaining--;
        
        // Set variables for new data item to send
        if (!remaining) {
            switch (current) {
                case SEND_TIME:
                    remaining = ALT ? SEND_ALT_AMT : COORD ? SEND_COORD_AMT : SATS ? SEND_SATS_AMT : SEND_TIME_AMT;
                    current = ALT ? SEND_ALT : COORD ? SEND_COORD : SATS ? SEND_SATS : SEND_TIME;
                    break;
                case SEND_ALT:
                    remaining = COORD ? SEND_COORD_AMT : SATS ? SEND_SATS_AMT : SEND_TIME_AMT;
                    current = COORD ? SEND_COORD : SATS ? SEND_SATS : SEND_TIME;
                    break;
                case SEND_COORD:
                    remaining = SATS ? SEND_SATS_AMT : SEND_TIME_AMT;
                    current = SATS ? SEND_SATS : SEND_TIME;
                    break;
                case SEND_SATS:
                    remaining = SEND_TIME_AMT;
                    current = SEND_TIME;
                    break;
            }
            new = 1;
        }

        // Send the item
        sendMessage(buffer);
    }

    sendChar(0x0A);
    sendChar(0x0D);
}

// Calculates the length of a data point log based on the enabled settings
inline uint8_t getLength(SETTINGS * settings) {
    uint8_t length = SEND_TIME_AMT;
    if (settings->alt) length += SEND_ALT_AMT;
    if (settings->coord) length += SEND_COORD_AMT;
    if (settings->sats) length += SEND_SATS_AMT;
    return length;
}

inline uint8_t log(GPS * gps, SETTINGS * settings, uint8_t * eepromPointer) {

    // Check if there is enough space in EEPROM
    if (*eepromPointer <= (255 - getLength(settings))) {

        // Send Time
        WRITE(gps->hours);
        WRITE(gps->minutes);
        WRITE(gps->seconds);

        // Send Altitude
        if (settings->alt) {
            WRITE(UPPER(gps->alt_int));
            WRITE(LOWER(gps->alt_int));
            WRITE(gps->alt_dec);
        }

        // Send Coordinates
        if (settings->coord) {
            WRITE(UPPER(gps->lat_int));
            WRITE(LOWER(gps->lat_int));
            WRITE(UPPER(gps->lat_dec));
            WRITE(LOWER(gps->lat_dec));
            WRITE(gps->lat_symbol);
            WRITE(UPPER(gps->long_int));
            WRITE(LOWER(gps->long_int));
            WRITE(UPPER(gps->long_dec));
            WRITE(LOWER(gps->long_dec));
            WRITE(gps->long_symbol);
        }

        if (settings->sats) {
            WRITE(gps->sats);
        }

    }

    EEPROM_WRITE(1, *eepromPointer);
    return 0;
}

// Writes configuration to EEPROM address 0
inline void readConfig(SETTINGS * settings, uint8_t * eepromPointer) {
    uint8_t config = EEPROM_READ(0);
    settings->rate = (config & 0b11100000) >> 5;
    settings->coord = (config & 0b00010000) >> 4;
    settings->alt = (config & 0b00001000) >> 3;
    settings->sats = (config & 0b00000100) >> 2;
    *eepromPointer = EEPROM_READ(1);
}

// Reads configuration from EEPROM address 0
inline void updateConfig(SETTINGS * settings) {
    uint8_t config = settings->rate << 5;
    config |= settings->coord << 4;
    config |= settings->alt << 3;
    config |= settings->sats << 2;
    EEPROM_WRITE(0, config);
}