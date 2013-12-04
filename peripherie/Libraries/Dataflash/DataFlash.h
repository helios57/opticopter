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
	unsigned int getPage(unsigned int pos);

	GPIO* gpio;
	SPIDeviceDriver3* spi;
	unsigned int pageSize;
	unsigned int pages;
	unsigned int pageInBufferA;
	unsigned int pageInBufferB;
	bool bufferChangedA;
	bool bufferChangedB;
public:
	DataFlash(GPIO* gpio, SPIDeviceDriver3* spi) :
			gpio(gpio), spi(spi) {
		pageSize = 0;
		pages = 0;
		pageInBufferA = (unsigned int) 99999;
		pageInBufferB = (unsigned int) 99999;
		bufferChangedA = false;
		bufferChangedB = false;
	}
	void Init();
	void read(uint8_t* data, const unsigned int start, const unsigned int length);
	void write(const uint8_t* data, const unsigned int start, const unsigned int length);
	unsigned int writeBuffer(uint8_t BufferNum, const uint8_t* data, const unsigned int offset, const unsigned int bufferStart, const unsigned int length);
	bool isReady();
	void flush();
};

#endif
