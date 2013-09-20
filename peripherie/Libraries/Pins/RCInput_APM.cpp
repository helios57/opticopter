#include <avr/io.h>
#include <avr/interrupt.h>

#include "RCInput.h"

volatile uint16_t RCInput::_pulse_capt[AVR_RC_INPUT_NUM_CHANNELS] = { 0 };

extern "C" ISR(TIMER5_CAPT_vect) //interrupt.
{
	static uint16_t icr5_prev;
	static uint8_t channel_ctr;

	const uint16_t icr5_current = ICR5;
	uint16_t pulse_width;
	if (icr5_current < icr5_prev) {
		/* ICR5 rolls over at TOP=40000 */
		pulse_width = icr5_current + 40000 - icr5_prev;
	} else {
		pulse_width = icr5_current - icr5_prev;
	}

	if (pulse_width > 8000) {
		/* sync pulse detected */
		channel_ctr = 0;
	} else {
		if (channel_ctr < AVR_RC_INPUT_NUM_CHANNELS) {
			RCInput::_pulse_capt[channel_ctr] = pulse_width;
			channel_ctr++;
		}
	}
	icr5_prev = icr5_current;
}

void RCInput::init() {
	/* Arduino pin 48 is ICP5 / PL1,  timer 5 input capture */
	gpio->pinMode(48, GPIO_INPUT);
	/**
	 * WGM: 1 1 1 1. Fast WPM, TOP is in OCR5A
	 * COM all disabled
	 * CS51: prescale by 8 => 0.5us tick
	 * ICES5: input capture on rising edge
	 * OCR5A: 40000, 0.5us tick => 2ms period / 50hz freq for outbound
	 * fast PWM.
	 */
	TCCR5A = _BV(WGM50) | _BV(WGM51);
	TCCR5B = _BV(WGM53) | _BV(WGM52) | _BV(CS51) | _BV(ICES5);
	OCR5A = 40000;

	/* OCR5B and OCR5C will be used by RCOutput_APM2. init to nil output */
	OCR5B = 0xFFFF;
	OCR5C = 0xFFFF;

	/* Enable input capture interrupt */
	TIMSK5 |= _BV(ICIE5);
}

uint16_t RCInput::read(uint8_t ch) {
	/* constrain ch */
	if (ch >= AVR_RC_INPUT_NUM_CHANNELS)
		return 0;
	/* grab channel from isr's memory in critical section*/
	cli();
	uint16_t capt = _pulse_capt[ch];
	sei();
	/* scale _pulse_capt from 0.5us units to 1us units. */
	uint16_t pulse = capt >> 1;
	return pulse;
}
