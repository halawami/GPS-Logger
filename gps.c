/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file contains functions to read from the GPS and parse the GPS data
 *
 **/

#include "gps.h"

uint8_t data[65];
uint8_t enabled, counter = 0;

inline void readGPS(uint8_t * shouldUpdate, uint8_t * hasGPS) {
    uint8_t in = RCREG;
    if (!enabled) {
        // Start reading if the input character is $
        if (in == '$') enabled = 1;
    } else {
        // Read until the input character is *
        if (in == '*') {
            enabled = counter = 0;
            *shouldUpdate = *hasGPS = 1;
        } else {
            data[counter++] = in - 0x30;
        }
    }
}

inline void parseGPS(GPS * gps) {
    gps->hours = PARSE_2(6);
    gps->minutes = PARSE_2(8);
    gps->seconds = PARSE_2(10);
    gps->lat_int = PARSE_4(17);
    gps->lat_dec = PARSE_4(22);
    gps->lat_symbol = data[27] + 0x30;
    gps->long_int = PARSE_4(30);
    gps->long_dec = PARSE_4(35);
    gps->long_symbol = data[40] + 0x30;

    uint8_t offset = !(data[45] == 252);

    gps->sats = offset ? PARSE_2(44) : data[44];
    gps->alt_int = PARSE_3(51 + offset);
    gps->alt_dec = data[55 + offset];
}