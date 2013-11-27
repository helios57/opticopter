#ifndef __AP_HAL_AVR_RC_INPUT_H__
#define __AP_HAL_AVR_RC_INPUT_H__

#define AVR_RC_INPUT_NUM_CHANNELS 8

#include "AQ_Platform_APM/Interfaces/GPIO.h"

class RCInput {
private:
	GPIO *gpio;
public:
	RCInput(GPIO *gpio) :
			gpio(gpio) {
	}
	virtual ~RCInput() {
	}
	virtual void init();
	/* Read a single channel at a time */
	virtual uint16_t read(uint8_t ch);
	static volatile uint16_t _pulse_capt[AVR_RC_INPUT_NUM_CHANNELS];
};

#endif // __AP_HAL_AVR_RC_INPUT_H__
