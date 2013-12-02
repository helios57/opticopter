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

class SPIDeviceDriver0 {
private:
	GPIO *gpio;
	Stream *console;
	const DigitalSource* miso;
	const DigitalSource* mosi;
	const DigitalSource* sck;
	const DigitalSource* cs;
	volatile bool active;
public:
	SPIDeviceDriver0(GPIO *gpio, Stream *console, const DigitalSource* miso, const DigitalSource* mosi, const DigitalSource* sck, const DigitalSource* cs) :
			gpio(gpio), console(console), miso(miso), mosi(mosi), sck(sck), cs(cs) {
		active = false;
	}
	virtual ~SPIDeviceDriver0();
	void init();
	void begin();
	void end();
	unsigned char transfer(unsigned char data);
	void setHighRate(bool high);
};

#endif /* SPIDEVICEDRIVER_H_ */
