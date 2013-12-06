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

	// write size bytes of data to a page. The caller must ensure that
	// the data fits within the page, otherwise it will wrap to the
	// start of the page
	// If pHeader is not NULL then write the header bytes before the data
	void BlockWrite(uint8_t BufferNum, uint16_t IntPageAdr, const uint8_t *pHeader, uint8_t hdr_size, const uint8_t *pBuffer, uint16_t size);

	// read size bytes of data to a page. The caller must ensure that
	// the data fits within the page, otherwise it will wrap to the
	// start of the page
	bool BlockRead(uint8_t BufferNum, uint16_t IntPageAdr, void *pBuffer, uint16_t size);
	uint8_t BufferRead(uint8_t BufferNum, uint16_t IntPageAdr);

	void PageErase(uint16_t PageAdr);
	void BlockErase(uint16_t BlockAdr);
	void ChipErase();
	uint32_t getPage(uint32_t pos);

	GPIO* gpio;
	SPIDeviceDriver3* spi;
	uint32_t pageSize;
	uint32_t pages;
	uint32_t pageInBufferA;
	uint32_t pageInBufferB;
	bool bufferChangedA;
	bool bufferChangedB;
public:
	DataFlash(GPIO* gpio, SPIDeviceDriver3* spi) :
			gpio(gpio), spi(spi) {
		pageSize = 0;
		pages = 0;
		pageInBufferA = (uint32_t) 99999;
		pageInBufferB = (uint32_t) 99999;
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
