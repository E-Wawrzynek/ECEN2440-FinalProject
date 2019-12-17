/*
 * tempsens1.c
 *
 *  Created on: Dec 3, 2019
 *      Author: E
 */

//code adapted from maxim integrated
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <tempsens13.h>

void delay(int useconds) {
    int s;
    for (s=0; s<useconds;s++);
}

void data_pin(void) {
    P5->DIR &= ~BIT0;
    P5->SEL0 &= ~BIT0;
    P5->SEL1 &= ~BIT0;
//    P5->REN |= BIT0;
//    P5->OUT |= BIT0;
}


unsigned char ow_reset(void) {
    unsigned char presence;
    P5->DIR |= BIT0;
    P5->SEL0 &= ~BIT0;
    P5->SEL1 &= ~BIT0;
    P5->OUT &= ~BIT0;
    //DQ = 0; //pull DQ line low
    delay(29); // leave it low for 480us
    P5->OUT |= BIT0;
    //DQ = 1; // allow line to return high

    data_pin();
    delay(3); // wait for presence
    presence = P5->IN; // get presence signal
    delay(25); // wait for end of timeslot
    return presence & 1U; // presence signal returned
  } //if 0 sensor detected


unsigned char read_bit(void) {
    unsigned char i;
    P5->DIR |= BIT0;
    P5->SEL0 &= ~BIT0;
    P5->SEL1 &= ~BIT0;
    P5->OUT &= ~BIT0;
    //DQ = 0; // pull DQ low to start timeslot
    P5->OUT |= BIT0;
    //DQ = 1; // then return high

    data_pin();
    for (i=0; i<5; i++); // delay 15us from start of timeslot
    return(P5->IN); // return value of DQ line
}


void write_bit(char bitval) {
    P5->DIR |= BIT0;
    P5->SEL0 &= ~BIT0;
    P5->SEL1 &= ~BIT0;
    P5->OUT &= ~BIT0;
    //DQ = 0; // pull DQ low to start timeslot
    if(bitval==1) {
        P5->OUT |= BIT0;
        //DQ =1; // return DQ high if write 1
    }
    delay(5); // hold value for remainder of timeslot
    P5->OUT |= BIT0;
    //DQ = 1;
}// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us



unsigned char read_byte(void) {
    unsigned char i;
    unsigned char value = 0;
    for (i=0;i<8;i++) {
    if(read_bit()) value|=0x01<<i; // reads byte in, one byte at a time and then
    // shifts it left
    delay(6); // wait for rest of timeslot
    }
    return(value);
}


void write_byte(char val) {
    unsigned char i;
    unsigned char temp;
    for (i=0; i<8; i++) // writes byte, one bit at a time
    {
    temp = val>>i; // shifts val right 'i' spaces
    temp &= 0x01; // copy that bit to temp
    write_bit(temp); // write bit in temp into
    }
    delay(5);
}

void Read_Temperature(void) {
    char get[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char temp_lsb,temp_msb;
    int k;
    char temp_f,temp_c;

    ow_reset();
    write_byte(0xCC); //Skip ROM
    write_byte(0x44); // Start Conversion

    ow_reset();
    write_byte(0xCC); // Skip ROM
    write_byte(0xBE); // Read Scratch Pad
    for (k=0;k<9;k++){get[k]=read_byte();}

    printf("\n ScratchPAD DATA = %d%d%d%d%d%d%d%d%d\n",get[8],get[7],get[6],get[5],get[4],get[3],get[2],get[1],get[0]);
    temp_msb = get[1]; // Sign byte + lsbit
    temp_lsb = get[0]; // Temp data plus lsb

    if (temp_msb <= 0x80){temp_lsb = (temp_lsb/2);} // shift to get whole degree
    temp_msb = temp_msb & 0x80; // mask all but the sign bit
    if (temp_msb >= 0x80) {temp_lsb = (~temp_lsb)+1;} // twos complement
    if (temp_msb >= 0x80) {temp_lsb = (temp_lsb/2);}// shift to get whole degree
    if (temp_msb >= 0x80) {temp_lsb = ((-1)*temp_lsb);} // add sign bit
    printf( "\nTempC= %d degrees C\n", (int)temp_lsb ); // print temp. C

    temp_c = temp_lsb; // ready for conversion to Fahrenheit
    temp_f = (((int)temp_c)* 9)/5 + 32;
    printf( "\nTempF= %d degrees F\n", (int)temp_f ); // print temp. F
}

void Read_ScratchPad(void) {
    int j;
    char pad[10];

    printf("\nReading ScratchPad Data\n");
    write_byte(0xBE);
    for (j=0;j<9;j++){pad[j]=read_byte();}
    printf("\n ScratchPAD DATA = %d%d%d%d%d%d%d%d%d\n",pad[8],pad[7],pad[6],pad[5],pad[4],pad[3],pad[2],pad[1],pad[0]);
}

void Read_ROMCode(void) {
    int n;
    char dat[9];

    printf("\nReading ROM Code\n");
    ow_reset();
    write_byte(0x33);
    for (n=0;n<8;n++){dat[n]=read_byte();}
    printf("\n ROM Code = %d%d%d%d%d%d%d%d\n",dat[7],dat[6],dat[5],dat[4],dat[3],dat[2],dat[1],dat[0]);
}
