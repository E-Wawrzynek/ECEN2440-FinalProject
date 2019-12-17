/*
 * tempsens1.h
 *
 *  Created on: Dec 3, 2019
 *      Author: E
 */

#ifndef TEMPSENS13_H_
#define TEMPSENS13_H_

void data_pin(void);
void delay(int useconds);
unsigned char ow_reset(void);
unsigned char read_bit(void);
void write_bit(char bitval);
unsigned char read_byte(void);
void write_byte(char val);
void Read_Temperature(void);
void Read_ScratchPad(void);
void Read_ROMCode(void);

unsigned int DQ;

#endif /* TEMPSENS13_H_ */
