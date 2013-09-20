#ifndef __AP_HAL_AVR_GPIO_H__
#define __AP_HAL_AVR_GPIO_H__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <Interrupts/Proc.h>
#include "DigitalSource.h"

static const unsigned char GPIO_INPUT = 0;
static const unsigned char GPIO_OUTPUT = 1;
static const unsigned char GPIO_INTERRUPT_LOW = 0;
static const unsigned char GPIO_INTERRUPT_HIGH = 1;
static const unsigned char GPIO_INTERRUPT_FALLING = 2;
static const unsigned char GPIO_INTERRUPT_RISING = 3;

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogInPinToBit(P) (P)

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
//
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
//#define portOutputRegister(P) ( (volatile unsigned char *)( pgm_read_word( port_to_output_PGM + (P))) )
//#define portInputRegister(P) ( (volatile unsigned char *)( pgm_read_word( port_to_input_PGM + (P))) )
//#define portModeRegister(P) ( (volatile unsigned char *)( pgm_read_word( port_to_mode_PGM + (P))) )

class GPIO {
public:
	GPIO();
	virtual ~GPIO();
	void pinMode(unsigned char pin, unsigned char output);
	unsigned char read(unsigned char pin);
	void write(unsigned char pin, unsigned char value);
	DigitalSource* channel(unsigned short n);

	/* Interrupt interface: */
	bool attach_interrupt(unsigned char interrupt_num, Proc p, unsigned char mode);
};

#endif // __AP_HAL_AVR_GPIO_H__
