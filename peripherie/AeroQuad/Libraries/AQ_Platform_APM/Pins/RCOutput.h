#ifndef __AP_HAL_AVR_RC_OUTPUT_H__
#define __AP_HAL_AVR_RC_OUTPUT_H__

/* Define the CH_n names, indexed from 1, if we don't have them already */
#ifndef CH_1
#define CH_1 0
#define CH_2 1
#define CH_3 2
#define CH_4 3
#define CH_5 4
#define CH_6 5
#define CH_7 6
#define CH_8 7
#define CH_9 8
#define CH_10 9
#define CH_11 10
#endif

#include "AQ_Platform_APM/Interfaces/GPIO.h"

class RCOutput {
private:
	GPIO *gpio;
	uint16_t _timer_period(uint16_t speed_hz);
public:
	RCOutput(GPIO *gpio) :
			gpio(gpio) {
	}
	~RCOutput() {
	}
	void init();

	/* Output freq (1/period) control */
	void set_freq(uint32_t chmask, uint16_t freq_hz);
	uint16_t get_freq(uint8_t ch);

	/* Output active/highZ control, either by single channel at a time
	 * or a mask of channels */
	void enable_ch(uint8_t ch);
	void enable_mask(uint32_t chmask);

	void disable_ch(uint8_t ch);
	void disable_mask(uint32_t chmask);

	/* Output, either single channel or bulk array of channels */
	void write(uint8_t ch, uint16_t period_us);
	void write(uint8_t ch, uint16_t* period_us, uint8_t len);

	/* Read back current output state, as either single channel or
	 * array of channels. */
	uint16_t read(uint8_t ch);
	void read(uint16_t* period_us, uint8_t len);
};

#endif // __AP_HAL_AVR_RC_OUTPUT_H__
