/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/* ************************************************************ */
/* DataFlash_APM2 Log library                                 */
/* ************************************************************ */
#ifndef __DATAFLASH_H__
#define __DATAFLASH_H__

#include "DataFlash.h"
#include <Interfaces/DigitalSource.h>
#include <Interfaces/GPIO.h>
#include <Interfaces/SPIDeviceDriver3.h>

class DataFlash {
private:

	//Methods
	void BufferToPage(uint8_t BufferNum, uint16_t PageAdr, bool wait);
	void PageToBuffer(uint8_t BufferNum, uint16_t PageAdr);
	void WaitReady();
	uint8_t ReadStatusReg();
	uint8_t ReadStatus();
	uint16_t PageSize();
	void ChipErase();

	uint16_t getPage(uint32_t pos);

	GPIO* gpio;
	SPIDeviceDriver3* spi;
	uint16_t pageSize;
	uint16_t pageInBufferA;
	uint16_t pageInBufferB;
	bool bufferChangedA;
	bool bufferChangedB;
public:
	DataFlash(GPIO* gpio, SPIDeviceDriver3* spi) :
			gpio(gpio), spi(spi) {
		pageSize = 0;
		pageInBufferA = (uint16_t) 9999;
		pageInBufferB = (uint16_t) 9999;
		bufferChangedA = false;
		bufferChangedB = false;
	}
	void Init();
	void read(uint8_t* data, const uint32_t start, const uint32_t length);
	void write(const uint8_t* data, const uint32_t start, const uint32_t length);
	uint32_t writeBuffer(uint8_t BufferNum, const uint8_t* data, const uint32_t offset, const uint32_t bufferStart, const uint32_t length);
	bool isReady();
	void flush();
};

#endif
