/*
 * SPIDeviceDriver.h
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#ifndef SPIDEVICEDRIVER_H_
#define SPIDEVICEDRIVER_H_

#include "GPIO.h"
#include <Stream.h>

class SPIDeviceDriver {
private:
	GPIO *gpio;
	Stream *console;
	void cs_release();
	void cs_assert();
	const unsigned char miso;
	const unsigned char mosi;
	const unsigned char sck;
	const unsigned char cs;
	volatile bool active;
public:
	SPIDeviceDriver(GPIO *gpio, Stream *console, const unsigned char miso, const unsigned char mosi, const unsigned char sck, const unsigned char cs) :
			gpio(gpio), console(console), miso(miso), mosi(mosi), sck(sck), cs(cs) {
		active = false;
	}
	virtual ~SPIDeviceDriver();
	void init();
	void begin();
	void end();
	unsigned char transfer(unsigned char data);
};

#endif /* SPIDEVICEDRIVER_H_ */
