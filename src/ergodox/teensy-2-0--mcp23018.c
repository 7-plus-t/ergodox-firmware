/* ----------------------------------------------------------------------------
 * ergodox controller (Teensy 2.0 and MCP23018) specific stuff
 * - public things are prefixed by `controller_` or `CONTROLLER_`
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak
 * Released under The MIT License (MIT) (see "license.md") at
 * <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~ documentation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ----------------------------------------------------------------------------
 * Pinouts and Pin assignments
 * - '+' indicates pin
 * - 'o' indicates unused pin
 * - '-'s inserted between some of the pin functions for readability
 * - 'OC**' pins enclosed in parenthesis had lines over them in the pinout
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 *  Teensy 2.0
 *  ==========               GND +---.....---+ VCC
 *                        SS PB0 +           + PF0 ADC0
 *                      SCLK PB1 +           + PF1 ADC1
 *                      MOSI PB2 +           + PF4 ADC4
 *                      MISO PB3 +  +     +  + PF5 ADC5
 * RTS  OC1C  OC0A --------- PB7 + PE6  AREF + PF6 ADC6
 *            OC0B  INT0 SCL PD0 + AIN0      + PF7 ADC7
 *                  INT1 SDA PD1 + INT6      + PB6 ADC13 OC1B  OC4B
 * RXD1 ----------- INT2 --- PD2 +           + PB5 ADC12 OC1A (OC4B)
 * TXD1 ----------- INT3 --- PD3 +           + PB4 ADC11
 *      OC3A (OC4A) -------- PC6 +           + PD7 ADC10 T0 -- OC4D
 * ICP3 ----- OC4A --------- PC7 +-+-+-+-+-+-+ PD6 ADC9  T1 - (OC4D) onboardLED
 *                  CTS XCK1 PD5 --/ | | | \-- PD4 ADC8 ------------ ICP1
 *             VCC ------------------/ | \-------------- RST
 *             GND --------------------/
 * ----------------------------------------------------------------------------
 *  MCP23018
 *  ========            Vss(GND) +01---.---28+ NC
 *                            NC +02       27+ GPA7
 *                          GPB0 +03       26+ GPA6
 *                          GPB1 +04       25+ GPA5
 *                          GPB2 +05       24+ GPA4
 *                          GPB3 +06       23+ GPA3
 *                          GPB4 +07       22+ GPA2
 *                          GPB5 +08       21+ GPA1
 *                          GPB6 +09       20+ GPA0
 *                          GPB7 +10       19+ INTA
 *                      Vdd(Vcc) +11       18+ INTB
 *                           SCL +12       17+ NC
 *                           SDA +13       16+ RESET
 *                            NC +14-------15+ ADDR
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 *  Teensy 2.0 Pin Assignments
 *  ==========================
 *           power_negative  GND +---.....---+ Vcc   power_positive
 *                 column6   PB0 +           + PF0   row6
 *                               o           + PF1   row7
 *                               o           + PF4   row8
 *                               o  o     o  + PF5   row9
 *                    LED3  OC1C +           + PF6   rowA
 *                     I2C   SCL +           + PF7   rowB
 *                     I2C   SDA +           + OC1B  LED2
 *                 column3   PD2 +           + OC1A  LED1
 *                 column4   PD3 +           + PB4   column0
 *                 column1   PC6 +           + PD7   column5
 *                 column2   PC7 +-o-o-o-o-o-o
 *
 *  notes:
 *  - SCL and SDA: Need external pull-up resistors.  Sometimes the internal
 *    pull-ups are enough (see datasheet section 20.5.1), but i think for this
 *    project we'll want external ones, in case people want to separate the
 *    halves very far.
 * ----------------------------------------------------------------------------
 *  MCP32018 Pin Assignments
 *  ========================
 *      power_negative  Vss(GND) +01---.---28o
 *                               o02       27o
 *             column0      GPB0 +03       26o
 *             column1      GPB1 +04       25+ GPA5   rowB
 *             column2      GPB2 +05       24+ GPA4   rowA
 *             column3      GPB3 +06       23+ GPA3   row9
 *             column4      GPB4 +07       22+ GPA2   row8
 *             column5      GPB5 +08       21+ GPA1   row7
 *             column6      GPB6 +09       20+ GPA0   row6
 *                               o10       19o
 *      power_positive  Vdd(Vcc) +11       18o
 *                 I2C       SCL +12       17o
 *                 I2C       SDA +13       16o
 *                               o14-------15+ ADDR   (see note)
 *
 *  notes:
 *  - ADDR (pin15): Set slave address to 0b0100000 by connecting to Vss(GND).
 *    (The user-defined bits are the three least significant).
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Notes about Registers
 * ----------------------------------------------------------------------------
 * General I/O (see datasheet section 10.2.1)
 *
 *     DDRxn  function  PINxn  function
 *     1      output    1      drive high
 *                      0      drive low
 *     0      input     1      internal pull-up on
 *                      0      internal pull-up off
 *
 *  notes:
 *  - Unused pins should be set as input with internal pullup enabled (see
 *    datasheet section 10.2.6).
 * ----------------------------------------------------------------------------
 * PWM on ports OC1(A|B|C) (see datasheet section 14.10)
 *
 *  notes:
 *  - PWM pins should be set as outputs.
 *  - we want Waveform Generation Mode 15
 *    (fast PWM, TOP = OCRnA)
 *    (see table 14-5)
 *    - set (TCCRB[4,3],TCCRA[1,0]) to (1,1,1,1)
 *  - we want "Compare Output Mode, Fast PWM" to be 0b10
 *    "Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at TOP"
 *    (see table 14-3)
 *    this way higher values of OCR1(A|B|C) will mean longer 'on' times for the
 *    LEDs
 *    - when in a fast PWM mode, set (TCCR1A[7,6,5,4,3,2]) to (1,0,1,0,1,0)
 *  - we want "Clock Select Bit Description" to be 0b001
 *    "clkI/O/1 (No prescaling)"
 *    (see table 14-6)
 *    - set (TCCR1B[2,1,0]) to (0,0,1)
 *  - LEDs will be at minimum brightness until OCR1(A|B|C) are changed (since
 *    the default value of all the bits in those registers is 0)
 *
 *  abbreviations:
 *  - OCR = Output Compare Register
 *  - TCCR = Timer/Counter Control Register
 * ------------------------------------------------------------------------- */


#include "lib/twi.h"

#include "teensy-2-0--mcp23018.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~ macros ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// processor frequency (from <http://www.pjrc.com/teensy/prescaler.html>)
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

// TWI frequency
#define TWI_FREQ 400000  // (see lib/twi.(h|c))

// pins
// --- rows ?TODO
// --- columns ?TODO


// mcp23018 pins
// TODO
// I2C         SCL 
// I2C         SDA 
// ROW_6       GPA0 
// ROW_7       GPA1 
// ROW_8       GPA2 
// ROW_9       GPA3 
// ROW_A       GPA4 
// ROW_B       GPA5 
// COLUMN_0_LH GPB0 
// COLUMN_1_LH GPB1 
// COLUMN_2_LH GPB2 
// COLUMN_3_LH GPB3 
// COLUMN_4_LH GPB4 
// COLUMN_5_LH GPB5 
// COLUMN_6_LH GPB6 


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void controller_init() {
	teensy_init();    // must be first
	mcp23018_init();  // must be second
}

void teensy_init() {
	CPU_PRESCALE(CPU_16MHz);  // speed should match F_CPU in makefile

	// unused pins
	DDRB  &= ~0b00001110;  // set B(1,2,3) as input
	PORTB |=  0b00001110;  // set B(1,2,3) internal pull-up enabled
	DDRD  &= ~0b01110000;  // set D(4,5,6) as input
	PORTD |=  0b01110000;  // set D(4,5,6) internal pull-up enabled
	DDRE  &= ~0b01000000;  // set E(6)     as input
	PORTE |=  0b01000000;  // set E(6)     internal pull-up enabled

	// LEDs
	DDRB   |= 0b11100000;  // set B(5,6,7) as output
	TCCR1A  = 0b10101011;  // set and configure fast PWM
	TCCR1B  = 0b00011001;  // set and configure fast PWM

	// rows
	DDRF  |= 0b11110011;  // set F(0,1,4,5,6,7) as output
	PORTF |= 0b11110011;  // set F(0,1,4,5,6,7) drive high

	// columns
	DDRB  &= ~0b00010001;  // set B(0,4)   as input
	PORTB |=  0b00010001;  // set B(0,4)   internal pull-up enabled
	DDRC  &= ~0b11000000;  // set C(6,7)   as input
	PORTC |=  0b11000000;  // set C(6,7)   internal pull-up enabled
	DDRD  &= ~0b10001100;  // set D(2,3,7) as input
	PORTD |=  0b10001100;  // set D(2,3,7) internal pull-up enabled

	// I2C (TWI)
	twi_init();  // (on pins D(0,1))
	twi_setAddress(0b0100000);
}

// TODO
void mcp23018_init() {
}

