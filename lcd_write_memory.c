/*
 * Copyright (c) 2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include <stdlib.h>
#include <string.h>

#define ASCII_DIGIT_OFFSET (48)
#define ASCII_LETTER_OFFSET (65)

typedef struct {
    uint32_t pin1;
    uint32_t pin2;
    uint32_t pin3;
    uint32_t pin4;
} LCD_pin;

/* Onboard LCD positions 1-6 */
LCD_pin gLCDPinPosition1;
LCD_pin gLCDPinPosition2;
LCD_pin gLCDPinPosition3;
LCD_pin gLCDPinPosition4;
LCD_pin gLCDPinPosition5;
LCD_pin gLCDPinPosition6;

/* LCD memory map for numeric digits */
const char digit[10][4] = {
    {0x07, 0x09, 0x08, 0x0A}, /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x00, 0x00, 0x00, 0x0A}, /* "1" */
    {0x03, 0x0A, 0x00, 0x0C}, /* "2" */
    {0x01, 0x0A, 0x00, 0x0E}, /* "3" */
    {0x04, 0x02, 0x00, 0x0E}, /* "4" */
    {0x05, 0x0A, 0x01, 0x00}, /* "5" */
    {0x07, 0x0A, 0x00, 0x06}, /* "6" */
    {0x00, 0x08, 0x00, 0x0A}, /* "7" */
    {0x07, 0x0A, 0x00, 0x0E}, /* "8" */
    {0x05, 0x0A, 0x00, 0x0E}  /* "9" */
};

/* LCD memory map for uppercase letters */
const char alphabetUpper[26][4] = {
    {0x06, 0x0A, 0x00, 0x0E}, /* "A" LCD segments a+b+c+e+f+g+m */
    {0x01, 0x08, 0x06, 0x0E}, /* "B" */
    {0x07, 0x08, 0x00, 0x00}, /* "C" */
    {0x01, 0x08, 0x06, 0x0A}, /* "D" */
    {0x07, 0x0A, 0x00, 0x00}, /* "E" */
    {0x06, 0x0A, 0x00, 0x00}, /* "F" */
    {0x07, 0x08, 0x00, 0x06}, /* "G" */
    {0x06, 0x02, 0x00, 0x0E}, /* "H" */
    {0x01, 0x08, 0x06, 0x00}, /* "I" */
    {0x03, 0x00, 0x00, 0x0A}, /* "J" */
    {0x06, 0x02, 0x09, 0x00}, /* "K" */
    {0x07, 0x00, 0x00, 0x00}, /* "L" */
    {0x06, 0x04, 0x08, 0x0A}, /* "M" */
    {0x06, 0x04, 0x01, 0x0A}, /* "N" */
    {0x07, 0x08, 0x00, 0x0A}, /* "O" */
    {0x06, 0x0A, 0x00, 0x0C}, /* "P" */
    {0x07, 0x08, 0x01, 0x0A}, /* "Q" */
    {0x06, 0x0A, 0x01, 0x0C}, /* "R" */
    {0x05, 0x0A, 0x00, 0x06}, /* "S" */
    {0x00, 0x08, 0x06, 0x00}, /* "T" */
    {0x07, 0x00, 0x00, 0x0A}, /* "U" */
    {0x06, 0x01, 0x08, 0x00}, /* "V" */
    {0x06, 0x01, 0x01, 0x0A}, /* "W" */
    {0x00, 0x05, 0x09, 0x00}, /* "X" */
    {0x05, 0x02, 0x00, 0x0E}, /* "Y" */
    {0x01, 0x09, 0x08, 0x00}  /* "Z" */
};

void LCD_showChar(LCD_Regs *lcd, char ch, LCD_pin lcdPinPosition)
{
    uint32_t pin1 = lcdPinPosition.pin1;
    uint32_t pin2 = lcdPinPosition.pin2;
    uint32_t pin3 = lcdPinPosition.pin3;
    uint32_t pin4 = lcdPinPosition.pin4;

    /* Divide by 2 because mux rate < 5 */
    uint32_t pin1memIdx = pin1 / 2;
    uint32_t pin2memIdx = pin2 / 2;
    uint32_t pin3memIdx = pin3 / 2;
    uint32_t pin4memIdx = pin4 / 2;

    uint8_t mem;
    uint32_t memMask;

    if (ch >= '0' && ch <= '9') {
        /* Write digits */
        if (pin1 % 2) {
            /* Even memory location */
            mem     = DL_LCD_getMemory(LCD, pin1memIdx) & 0x0F;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][0] << 4);
        } else {
            /* Odd memory location */
            mem     = DL_LCD_getMemory(LCD, pin1memIdx) & 0xF0;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][0]);
        }
        DL_LCD_writeMemory(LCD, pin1memIdx, memMask);

        if (pin2 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin2memIdx) & 0x0F;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][1] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin2memIdx) & 0xF0;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][1]);
        }
        DL_LCD_writeMemory(LCD, pin2memIdx, memMask);

        if (pin3 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin3memIdx) & 0x0F;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][2] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin3memIdx) & 0xF0;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][2]);
        }
        DL_LCD_writeMemory(LCD, pin3memIdx, memMask);

        if (pin4 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin4memIdx) & 0x0F;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][3] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin4memIdx) & 0xF0;
            memMask = (mem | digit[ch - ASCII_DIGIT_OFFSET][3]);
        }
        DL_LCD_writeMemory(LCD, pin4memIdx, memMask);

    } else if (ch >= 'A' && ch <= 'Z') {
        /* Write letters */
        if (pin1 % 2) {
            /* Even memory location */
            mem     = DL_LCD_getMemory(LCD, pin1memIdx) & 0x0F;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][0] << 4);
        } else {
            /* Odd memory location */
            mem     = DL_LCD_getMemory(LCD, pin1memIdx) & 0xF0;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][0]);
        }
        DL_LCD_writeMemory(LCD, pin1memIdx, memMask);

        if (pin2 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin2memIdx) & 0x0F;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][1] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin2memIdx) & 0xF0;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][1]);
        }
        DL_LCD_writeMemory(LCD, pin2memIdx, memMask);

        if (pin3 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin3memIdx) & 0x0F;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][2] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin3memIdx) & 0xF0;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][2]);
        }
        DL_LCD_writeMemory(LCD, pin3memIdx, memMask);

        if (pin4 % 2) {
            mem     = DL_LCD_getMemory(LCD, pin4memIdx) & 0x0F;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][3] << 4);
        } else {
            mem     = DL_LCD_getMemory(LCD, pin4memIdx) & 0xF0;
            memMask = (mem | alphabetUpper[ch - ASCII_LETTER_OFFSET][3]);
        }
        DL_LCD_writeMemory(LCD, pin4memIdx, memMask);
    }
}


char *cipher(char * unencrypted_text) {
 // create cipher here
}



int main(void)
{
    SYSCFG_DL_init();

    /* Map LCD segments to position on onboard LCD (1-6) */
    gLCDPinPosition1.pin1 = DL_LCD_SEGMENT_LINE_58;
    gLCDPinPosition1.pin2 = DL_LCD_SEGMENT_LINE_57;
    gLCDPinPosition1.pin3 = DL_LCD_SEGMENT_LINE_56;
    gLCDPinPosition1.pin4 = DL_LCD_SEGMENT_LINE_55;

    gLCDPinPosition2.pin1 = DL_LCD_SEGMENT_LINE_36;
    gLCDPinPosition2.pin2 = DL_LCD_SEGMENT_LINE_37;
    gLCDPinPosition2.pin3 = DL_LCD_SEGMENT_LINE_38;
    gLCDPinPosition2.pin4 = DL_LCD_SEGMENT_LINE_18;

    gLCDPinPosition3.pin1 = DL_LCD_SEGMENT_LINE_19;
    gLCDPinPosition3.pin2 = DL_LCD_SEGMENT_LINE_20;
    gLCDPinPosition3.pin3 = DL_LCD_SEGMENT_LINE_23;
    gLCDPinPosition3.pin4 = DL_LCD_SEGMENT_LINE_39;

    gLCDPinPosition4.pin1 = DL_LCD_SEGMENT_LINE_40;
    gLCDPinPosition4.pin2 = DL_LCD_SEGMENT_LINE_41;
    gLCDPinPosition4.pin3 = DL_LCD_SEGMENT_LINE_54;
    gLCDPinPosition4.pin4 = DL_LCD_SEGMENT_LINE_53;

    gLCDPinPosition5.pin1 = DL_LCD_SEGMENT_LINE_52;
    gLCDPinPosition5.pin2 = DL_LCD_SEGMENT_LINE_51;
    gLCDPinPosition5.pin3 = DL_LCD_SEGMENT_LINE_50;
    gLCDPinPosition5.pin4 = DL_LCD_SEGMENT_LINE_49;

    gLCDPinPosition6.pin1 = DL_LCD_SEGMENT_LINE_48;
    gLCDPinPosition6.pin2 = DL_LCD_SEGMENT_LINE_47;
    gLCDPinPosition6.pin3 = DL_LCD_SEGMENT_LINE_46;
    gLCDPinPosition6.pin4 = DL_LCD_SEGMENT_LINE_45;
    char * text = "lbuctf";
    char* encrypted = cipher(text);

    while(1){
        /* Write all numbers cycling through positions 1-6 */
    LCD_showChar(LCD, encrypted[0], gLCDPinPosition1);
    delay_cycles(32000000);
    LCD_showChar(LCD, encrypted[1], gLCDPinPosition2);
    delay_cycles(32000000);
    LCD_showChar(LCD, encrypted[2], gLCDPinPosition3);
    delay_cycles(32000000);
    LCD_showChar(LCD,encrypted[3], gLCDPinPosition4);
    delay_cycles(32000000);
    LCD_showChar(LCD, encrypted[4], gLCDPinPosition5);
    delay_cycles(32000000);
    LCD_showChar(LCD, encrypted[5], gLCDPinPosition6);
    delay_cycles(32000000);
    }
    
}
