/*
 * uart.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Zach Sents
 */

#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

#define BAUD            9600
#define SYSTEM_CLOCK    3000000

#define UART_RESET      0      // set reset bit

// Configuration
#define PARITY          15U   // disable parity
#define LSB_FIRST       13U   // LSB first
#define WORD_LENGTH     12U   // 8-bit word length
#define STOP_BIT        11U   // 1 stop bit
#define MODE_0          10U   // UART mode
#define MODE_1          9U    // "
#define CLK_SEL_0       7U    // use system clock
#define CLK_SEL_1       6U    // "

#define DEGLITCH_0      1U    // 5ns deglitch time
#define DEGLITCH_1      0     // "

#define OVERSAMPLING    0     // oversampling

void delay(int useconds);

void initUART();

void uart_transmit(uint8_t data);
void uart_print(char *msg);

void clear_display();
void send_setting(uint8_t setting);

void setBit(volatile uint16_t *reg, uint8_t n, uint8_t val);
bool uart_busy();

#endif /* UART_H_ */
