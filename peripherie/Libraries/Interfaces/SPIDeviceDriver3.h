/*
 * SPIDeviceDriver.h
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#ifndef SPIDEVICEDRIVER_3_H_
#define SPIDEVICEDRIVER_3_H_

#include "GPIO.h"
#include <Stream.h>

class SPIDeviceDriver3 {
private:
	GPIO *gpio;
	Stream *console;
	const DigitalSource* miso;
	const DigitalSource* mosi;
	const DigitalSource* sck;
	const DigitalSource* cs;
	volatile bool active;
public:
	SPIDeviceDriver3(GPIO *gpio, Stream *console, const DigitalSource* miso, const DigitalSource* mosi, const DigitalSource* sck, const DigitalSource* cs) :
			gpio(gpio), console(console), miso(miso), mosi(mosi), sck(sck), cs(cs) {
		active = false;
	}
	virtual ~SPIDeviceDriver3() {

	}
	void init();
	void begin();
	void end();
	unsigned char transfer(unsigned char data);
};

#endif /* SPIDEVICEDRIVER_H_ */
