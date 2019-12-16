/*
 * uart.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Zach Sents
 */
#include "msp.h"
#include "uart.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void delay(int useconds) {
    int i;
    for(i = 0; i < useconds; i++);
}

void initUART() {

    P1SEL0 |= BIT3;
    uart_transmit(0);

    setBit(&EUSCI_A0->CTLW0, UART_RESET, 1);  // reset

    delay(100);  // delay

    // Configure UART settings
    setBit(&EUSCI_A0->CTLW0, PARITY, 0);      // disable parity
    setBit(&EUSCI_A0->CTLW0, LSB_FIRST, 0);   // LSB first
    setBit(&EUSCI_A0->CTLW0, WORD_LENGTH, 0);       // 8-bit word length
    setBit(&EUSCI_A0->CTLW0, STOP_BIT, 0);    // 1 stop bit
    setBit(&EUSCI_A0->CTLW0, MODE_0, 0);      // UART mode
    setBit(&EUSCI_A0->CTLW0, MODE_1, 0);      // "
    setBit(&EUSCI_A0->CTLW0, CLK_SEL_0, 1);   // use system clock
    setBit(&EUSCI_A0->CTLW0, CLK_SEL_1, 0);   // "

    setBit(&EUSCI_A0->CTLW1, DEGLITCH_0, 0);  // 5ns deglitch time
    setBit(&EUSCI_A0->CTLW1, DEGLITCH_1, 0);  // "

    setBit(&EUSCI_A0->MCTLW, OVERSAMPLING, 1);    // enable oversampling

    // Baud rate control
    float n = (float)SYSTEM_CLOCK / (float)BAUD;
    EUSCI_A0->BRW = (int)(n / 16);
    EUSCI_A0->MCTLW |= 0xf0 & (int)(((n / 16.0f) - (int)(n / 16.0f)) * 16);  // BRF
    EUSCI_A0->MCTLW |= 0xAA << 8;   // BRS

    delay(100);  // delay

    setBit(&EUSCI_A0->CTLW0, UART_RESET, 0);   // clear reset bit
}

void uart_transmit(uint8_t data) {
    while(!((EUSCI_A0->IFG >> 1) & 1U)){}
//    EUSCI_A0->TXBUF &= !(0xff);
    EUSCI_A0->TXBUF = data;
}

void uart_print(char *msg) {
    int i;
    for(i = 0; i < strlen(msg); i++) {
        uart_transmit(msg[i]);
        delay(2000);
    }
}

void send_setting(uint8_t setting) {
    uart_transmit(0x7C);
    delay(2000);
    uart_transmit(setting);
    delay(2000);
}

void clear_display() {
    //send_setting(0x08);
    send_setting(0x2D);
}

bool uart_busy() {
    return EUSCI_A0->STATW & 1U;
}

void setBit(volatile uint16_t *reg, uint8_t n, uint8_t val) {
    *reg = (*reg & ~(1UL << n)) | (val << n);
}




