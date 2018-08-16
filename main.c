/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file initializes the PIC and loops over the routine. It also contains the interrupt manager.
 *
 **/

#include "main.h"
#include "gps.h"

// Global Variables
GPS gps;
SETTINGS settings;
const uint16_t dataRates[] = {10, 30, 60, 300, 600};
uint8_t mainScreen = 0, subScreen = 0, shouldUpdate = 1, hasGPS = 0, shouldLog = 0, eepromPointer = 2, oldSeconds;
uint8_t buffer[20];
uint16_t secondsElapsed = 0, lastButtonPress = 0;

void main(void) {

    // Initializations
    initLCD();
    initUART();
    initTMR1();
    initButtons();
    readConfig(&settings, &eepromPointer);

    //Enable Interrupts
    PEIE = 1;
    GIE = 1;

    // Main Loop
    while (1) {

        // Update the LCD if shouldUpdate
        if (shouldUpdate) {
            shouldUpdate = routine();
            parseGPS(&gps);
        }

        // Log data if needed
        if (shouldLog) {
            log(&gps, &settings, &eepromPointer);
            shouldLog = 0;
            // eepromPointer++;
        }

        // RB0 Switches Screen
        if (RB1 == 0) {
            INC(mainScreen, 7);
            while (RB1 == 0);
            DelayMs(8);
            shouldUpdate = 1;
        }

        // RB1 Switch Sub Screens or Action Button
        if (RB0 == 0) {
            switch (mainScreen) {
                case 0:
                    INC(subScreen, 6);
                    break;
                case 1:
                    INC(settings.rate, 5);
                    updateConfig(&settings);
                    break;
                case 2:
                    CHANGE(settings.coord);
                    break;
                case 3:
                    CHANGE(settings.alt);
                    break;
                case 4:
                    CHANGE(settings.sats);
                    break;
                case 5:
                    lcd_goto(0x40);
                    lcdWriteString("Memory Cleared", KEEP);
                    EEPROM_WRITE(0, 0);
                    EEPROM_WRITE(1, 2);
                    eepromPointer = 2;
                    DelayMs(1000);
                    break;
                case 6:
                    lcd_goto(0x40);
                    lcdWriteString("Sending Memory", KEEP);
                    sendData(&settings, &eepromPointer);
                    DelayMs(1000);
                    break;
            }
            while (RB0 == 0);
            DelayMs(8);
            shouldUpdate = 1;
        }

    }

    return;
}

uint8_t routine(void) {
    // Choose what to display
    switch (mainScreen) {
        case 0:
            if (hasGPS) {
                // Choose what sub-screen to display
                switch (subScreen) {
                    case 0:
                        sprintf(buffer, "Time: %02i:%02i:%02i", gps.hours, gps.minutes, gps.seconds);
                        lcdWriteString(buffer, CLEAR);
                        break;
                    case 1:
                        sprintf(buffer, "Lat %d.%d %c", gps.lat_int, gps.lat_dec, gps.lat_symbol);
                        lcdWriteString(buffer, CLEAR);
                        lcd_goto(0x40);
                        sprintf(buffer, "Long %d.%d %c", gps.long_int, gps.long_dec, gps.long_symbol);
                        lcdWriteString(buffer, KEEP);
                        break;
                    case 2:
                        sprintf(buffer, "Satellites: %d", gps.sats);
                        lcdWriteString(buffer, CLEAR);
                        break;
                    case 3:
                        lcdWriteString("Altitude:", CLEAR);
                        lcd_goto(0x40);
                        sprintf(buffer, "%d.%d meters", gps.alt_int, gps.alt_dec);
                        lcdWriteString(buffer, KEEP);
                        break;
                    case 4:
                        lcdWriteString("Memory Usage:", CLEAR);
                        lcd_goto(0x40);
                        sprintf(buffer, "%d/255 bytes", eepromPointer);
                        lcdWriteString(buffer, KEEP);
                        break;
                    case 5:
                        lcdWriteString("Minutes Left:", CLEAR);
                        lcd_goto(0x40);
                        if (255 - eepromPointer < getLength(&settings)) {
                            lcdWriteString("FULL", KEEP);
                        } else {
                            uint16_t time = (255 - eepromPointer) / getLength(&settings) * dataRates[settings.rate];
                            sprintf(buffer, "%d minutes", (time / 60));
                            lcdWriteString(buffer, KEEP);
                        }
                        break;
                }
            } else
                lcdWriteString("No GPS Signal", CLEAR);
            break;
        case 1:
            lcdWriteString("Logging Interval", CLEAR);
            lcd_goto(0x40);
            sprintf(buffer, "%i seconds", dataRates[settings.rate]);
            lcdWriteString(buffer, KEEP);
            break;
        case 2:
            lcdWriteString("Coord Logging", CLEAR);
            lcd_goto(0x40);
            lcdWriteString(settings.coord ? "Enabled" : "Disabled", KEEP);
            break;
        case 3:
            lcdWriteString("Altitude Logging", CLEAR);
            lcd_goto(0x40);
            lcdWriteString(settings.alt ? "Enabled" : "Disabled", KEEP);
            break;
        case 4:
            lcdWriteString("Num Sats Logging", CLEAR);
            lcd_goto(0x40);
            lcdWriteString(settings.sats ? "Enabled" : "Disabled", KEEP);
            break;
        case 5:
            lcdWriteString("Clear Memory?", CLEAR);
            break;
        case 6:
            lcdWriteString("Send Memory?", CLEAR);
            break;
    }
    return 0;
}

void interrupt interrupt_handler(void) {
    if (RCIF == 1) {
        readGPS(&shouldUpdate, &hasGPS);
        RCIF = 0;
    }
    if (CCP1IF == 1) {
        CCPR1 = CCPR1 + 25000;      // Set to interrupt in 0.1 seconds

        secondsElapsed++;

        // Check if the GPS is sending data, if not set hasGPS = 0
        if (secondsElapsed % 15 == 0) {
            if (oldSeconds == gps.seconds) {
                hasGPS = 0;
                shouldUpdate = 1;
            }
            oldSeconds = gps.seconds;
        }

        // Check if the program should log a data point
        if (secondsElapsed % (dataRates[settings.rate] * 10) == 0) {
            shouldLog = 1;
        }

        CCP1IF = 0;
    }
}
