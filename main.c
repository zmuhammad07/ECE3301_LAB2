/*
 * File:   main.c
 * Author: <your names here>
 *
 * Lab 2 - Electronic Dice
 * ECE 3301L - Introduction to Microcontrollers Laboratory
 *
 * Description:
 *   Press the button (RB4, active-low) to "roll" two dice.
 *   Each die is shown on 7 LEDs (PORTC = die 1, PORTD = die 2)
 *   using the dice patterns from the lab handout.
 *   Timer0 is used as a random seed source (human press timing).
 */

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>

#include "Configuration.h"

#define _XTAL_FREQ 64000000UL

/* Dice LED bit patterns - fill these in from the dice table
 * in the lab handout. Index 0 = face "1", index 5 = face "6". */
static const uint8_t dicePatterns[6] = {
    0x08,  // RC3 
    0x41, // RC0
    0x49,  // RC2
    0x63,  // Rc1
    0x6B, // RC4
    0x77  // RC6
};

static void init_clock_to_64MHz(void) {
 
    OSCCONbits.IRCF = 0x7;

    OSCCONbits.SCS = 0x2;

    OSCTUNEbits.PLLEN = 0x1;
}

static void init_timer0_free_running(void) {

    
    TMR0H = 0x00; 
    TMR0L = 0x00;
    
    T0CONbits.T08BIT = 0x00;
    T0CONbits.T0CS = 0x00;
    T0CONbits.PSA    = 1;  
    T0CONbits.TMR0ON = 0x1;

}

static void init_io(void) {
    // TODO: Make PORTC and PORTD outputs (dice LEDs) and clear their latches.
    LATC  = 0x00;
    TRISC = 0x00;
    LATD = 0x00;
    TRISD = 0x00;
    


    // TODO: Make RB4 an input (button, active-low).
    
    ANSELBbits.ANSB4 = 0;  
    TRISBbits.TRISB4   = 1;   
    // TODO: PBADEN=ON means PORTB<5:0> wake up as ANALOG pins.
    //       Force RB4 digital (ANSELB) or the button will always read 0.
}

static uint8_t read_button_pressed(void) {
    // TODO: return 1 while the button is pressed (remember: active-low)
    
    return(PORTBbits.RB4 == 0);
}

static void debounce_press(void) {
    // TODO: short delay (~30 ms) to ride out contact bounce
    __delay_ms(30);
}

static void wait_for_release(void) {
    // TODO: block until the button is released, then debounce the release
        while (read_button_pressed()) {
        ;   
    }
    __delay_ms(30);  
}

void main(void) {
    init_clock_to_64MHz();
    init_io();
    init_timer0_free_running();

    while (1) {
        
        if(read_button_pressed()){
        // TODO: When a (debounced) press is detected:
        //   1. Read TMR0H:TMR0L and use it to seed rand() with srand()
        uint8_t low = TMR0L;
        uint8_t high = TMR0H;
      
        srand(((uint16_t)high << 8) | low);
        
        //   2. Roll two dice with rand() % 6
        uint8_t die1 = rand() % 6;
        uint8_t die2 = rand() % 6;
        //   3. Show the two patterns on LATC and LATD
        
        LATC = dicePatterns[die1];
        LATD = dicePatterns[die2];
        //   4. Keep them displayed for 3 seconds, then turn LEDs off
        
        __delay_ms(1000);
        __delay_ms(1000);
        __delay_ms(1000);
        LATC = 0x00;
        LATD = 0x00;
        //   5. Wait for the button to be released (one roll per press)
        wait_for_release();
            
            
        }

    }
}