/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *       DataFlash_APM2.cpp - DataFlash log library for AT45DB321D
 *       Code by Jordi Muñoz and Jose Julio. DIYDrones.com
 *       This code works only on ATMega2560. It uses Serial port 3 in SPI MSPI mdoe.
 *
 *       Dataflash library for AT45DB321D flash memory
 *       Memory organization : 8192 pages of 512 bytes or 528 bytes
 *
 *       Maximun write bandwidth : 512 bytes in 14ms
 *       This code is written so the master never has to wait to write the data on the eeprom
 *
 *       Methods:
 *               Init() : Library initialization (SPI initialization)
 *               StartWrite(page) : Start a write session. page=start page.
 *               StartRead(page) : Start a read on (page)
 *               GetWritePage() : Returns the last page written to
 *               GetPage() : Returns the last page read
 *
 *       Properties:
 *
 */
#include <Arduino.h>
#include "DataFlash.h"

#define DF_RESET 41             // RESET  (PG0)
#define DF_CARDDETECT 33        // PC4
// AT45DB321D Commands (from Datasheet)
#define DF_TRANSFER_PAGE_TO_BUFFER_1   0x53
#define DF_TRANSFER_PAGE_TO_BUFFER_2   0x55
#define DF_STATUS_REGISTER_READ   0xD7
#define DF_READ_MANUFACTURER_AND_DEVICE_ID   0x9F
#define DF_PAGE_READ   0xD2
#define DF_BUFFER_1_READ   0xD4
#define DF_BUFFER_2_READ   0xD6
#define DF_BUFFER_1_WRITE   0x84
#define DF_BUFFER_2_WRITE   0x87
#define DF_BUFFER_1_TO_PAGE_WITH_ERASE   0x83
#define DF_BUFFER_2_TO_PAGE_WITH_ERASE   0x86
#define DF_PAGE_ERASE   0x81
#define DF_BLOCK_ERASE   0x50
#define DF_SECTOR_ERASE   0x7C
#define DF_CHIP_ERASE_0   0xC7
#define DF_CHIP_ERASE_1   0x94
#define DF_CHIP_ERASE_2   0x80
#define DF_CHIP_ERASE_3   0x9A

#define DF_READ 0x03
#define DF_BINARY_PAGESIZE 512

// Public Methods //////////////////////////////////////////////////////////////
void DataFlash::Init() {
	gpio->pinMode(DF_RESET, GPIO_OUTPUT);
	gpio->pinMode(DF_CARDDETECT, GPIO_INPUT);

// Reset the chip
	gpio->write(DF_RESET, 0);
	delayMicroseconds(1000);
	gpio->write(DF_RESET, 1);

// get page size: 512 or 528  (by default: 528)
	pageSize = PageSize();
}

// Read the status register
uint8_t DataFlash::ReadStatusReg() {
	uint8_t tmp;
	spi->begin();
// Read status command
	spi->transfer(DF_STATUS_REGISTER_READ);
	tmp = spi->transfer(0x00);      // We only want to extract the READY/BUSY bit
	spi->end();
	return tmp;
}

// Read the status of the DataFlash
inline uint8_t DataFlash::ReadStatus() {
	return (ReadStatusReg() & 0x80);  // We only want to extract the READY/BUSY bit
}

bool DataFlash::isReady() {
	return ReadStatus();
}

inline uint16_t DataFlash::PageSize() {
	return (528 - ((ReadStatusReg() & 0x01) << 4));  // if first bit 1 then 512 else 528 bytes
}

// Wait until DataFlash is in ready state...
void DataFlash::WaitReady() {
	while (!ReadStatus())
		;
}

void DataFlash::PageToBuffer(uint8_t BufferNum, uint16_t PageAdr) {
	spi->begin();
	uint8_t cmd[3];
	cmd[0] = BufferNum ? DF_TRANSFER_PAGE_TO_BUFFER_2 : DF_TRANSFER_PAGE_TO_BUFFER_1;
	if (pageSize == DF_BINARY_PAGESIZE) {
		cmd[1] = (uint8_t) (PageAdr >> 7);
		cmd[2] = (uint8_t) (PageAdr << 1);
	} else {
		cmd[1] = (uint8_t) (PageAdr >> 6);
		cmd[2] = (uint8_t) (PageAdr << 2);
	}
	spi->transfer(cmd[0]);
	spi->transfer(cmd[1]);
	spi->transfer(cmd[2]);
	spi->transfer(0);

//initiate the transfer
	spi->end();
	while (!ReadStatus())
		;     //monitor the status register, wait until busy-flag is high
}

void DataFlash::BufferToPage(uint8_t BufferNum, uint16_t PageAdr, bool wait) {
	spi->begin();
	spi->transfer(BufferNum ? DF_BUFFER_2_TO_PAGE_WITH_ERASE : DF_BUFFER_1_TO_PAGE_WITH_ERASE);
	if (pageSize == DF_BINARY_PAGESIZE) {
		spi->transfer((uint8_t) (PageAdr >> 7));
		spi->transfer((uint8_t) (PageAdr << 1));
	} else {
		spi->transfer((uint8_t) (PageAdr >> 6));
		spi->transfer((uint8_t) (PageAdr << 2));
	}
	spi->transfer(0);
	spi->end();

//TODO check or delete
// Check if we need to wait to write the buffer to memory or we can continue...
	if (wait)
		while (!ReadStatus())
			;  //monitor the status register, wait until busy-flag is high
}

void DataFlash::BlockWrite(uint8_t BufferNum, uint16_t IntPageAdr, const uint8_t* pHeader, uint8_t hdr_size, const uint8_t* pBuffer, uint16_t size) {
	spi->begin();
	spi->transfer((uint8_t) (BufferNum ? DF_BUFFER_2_WRITE : DF_BUFFER_1_WRITE));
	spi->transfer(0);
	spi->transfer((uint8_t) (IntPageAdr >> 8));
	spi->transfer((uint8_t) (IntPageAdr));

// transfer header, if any
	if (hdr_size != 0) {
		for (unsigned int i = 0; i < hdr_size; i++) {
			spi->transfer((uint8_t) pHeader[i]);
		}
	}

// transfer data
	for (unsigned int i = 0; i < size; i++) {
		spi->transfer((uint8_t) pBuffer[i]);
	}
	spi->end();
}

bool DataFlash::BlockRead(uint8_t BufferNum, uint16_t IntPageAdr, void *pBuffer, uint16_t size) {
	spi->begin();
	if (BufferNum == 0)
		spi->transfer(DF_BUFFER_1_READ);
	else
		spi->transfer(DF_BUFFER_2_READ);

	spi->transfer(0x00);
	spi->transfer((uint8_t) (IntPageAdr >> 8)); //upper part of internal buffer address
	spi->transfer((uint8_t) (IntPageAdr));      //lower part of internal buffer address
	spi->transfer(0x00);                        //don't cares

	uint8_t *pData = (uint8_t *) pBuffer;
	while (size--) {
		*pData++ = spi->transfer(0x00);
	}
	spi->end();
	return true;
}

uint8_t DataFlash::BufferRead(uint8_t BufferNum, uint16_t IntPageAdr) {
	uint8_t tmp;
	if (!BlockRead(BufferNum, IntPageAdr, &tmp, 1)) {
		return 0;
	}
	return tmp;
}

void DataFlash::PageErase(uint16_t PageAdr) {
	spi->begin();
// Send page erase command
	spi->transfer(DF_PAGE_ERASE);
	if (pageSize == DF_BINARY_PAGESIZE) {
		spi->transfer((uint8_t) (PageAdr >> 7));
		spi->transfer((uint8_t) (PageAdr << 1));
	} else {
		spi->transfer((uint8_t) (PageAdr >> 6));
		spi->transfer((uint8_t) (PageAdr << 2));
	}
	spi->transfer(0x00);
	//initiate flash page erase
	spi->end();
	while (!ReadStatus())
		;
}

void DataFlash::ChipErase() {
	spi->begin();

// opcodes for chip erase
	spi->transfer(DF_CHIP_ERASE_0);
	spi->transfer(DF_CHIP_ERASE_1);
	spi->transfer(DF_CHIP_ERASE_2);
	spi->transfer(DF_CHIP_ERASE_3);

//initiate flash page erase
	spi->end();
}

void DataFlash::read(uint8_t* data, const unsigned int start, const unsigned int length) {
	unsigned int page = getPage(start);
	unsigned int offset = start % pageSize;
	spi->begin();
	/*To read send 03H opcode followed by three address bytes.
	 * The first 13 bits (PA12 - PA0) of the 23-bit address =page
	 * The last  10 bits (BA9 - BA0) of the 23-bit address = the starting byte address within the page.
	 */
	spi->transfer(DF_READ);
	if (pageSize == DF_BINARY_PAGESIZE) {
		spi->transfer((uint8_t) (page >> 7));
		spi->transfer((uint8_t) (((page << 1) & 0xfe) | ((offset >> 8) & 0x01)));
	} else {
		spi->transfer((uint8_t) (page >> 6));
		spi->transfer((uint8_t) (((page << 2) & 0xfc) | ((offset >> 8) & 0x03)));
	}
	spi->transfer((uint8_t) (offset & 0xff));

	for (unsigned int i = 0; i < length; i++) {
		data[i] = spi->transfer(0x00);
	}
	spi->end();
}

void DataFlash::write(const uint8_t* data, const unsigned int start, unsigned int length) {
	unsigned int startPage = getPage(start);
	unsigned int lastPage = getPage(start + length);
	unsigned int offset = 0;
	for (unsigned int page = startPage; page <= lastPage; page++) {
		unsigned int lengthInPage = min(pageSize,length-offset);
		unsigned int startInPage = 0;
		if (page == startPage) {
			startInPage = start % pageSize;
		}
		if (pageInBufferA == page) {
			offset += writeBuffer(0, data, offset, startInPage, lengthInPage);
			bufferChangedA = true;
		} else if (pageInBufferB == page) {
			offset += writeBuffer(1, data, offset, startInPage, lengthInPage);
			bufferChangedB = true;
		} else if (page % 2 == 0) {
			if (bufferChangedB) {
				BufferToPage(1, (uint16_t) pageInBufferB, false);
			}
			PageToBuffer(0, page);
			pageInBufferA = page;
			offset += writeBuffer(0, data, offset, startInPage, lengthInPage);
			bufferChangedA = true;
		} else {
			if (bufferChangedA) {
				BufferToPage(0, (uint16_t) pageInBufferA, false);
			}
			PageToBuffer(1, page);
			pageInBufferB = page;
			offset += writeBuffer(1, data, offset, startInPage, lengthInPage);
			bufferChangedB = true;
		}
	}
}

unsigned int DataFlash::writeBuffer(uint8_t BufferNum, const uint8_t* data, const unsigned int offset, const unsigned int bufferStart, const unsigned int length) {
	spi->begin();
	spi->transfer((uint8_t) (BufferNum ? DF_BUFFER_2_WRITE : DF_BUFFER_1_WRITE));
	spi->transfer(0);
	spi->transfer((uint8_t) ((bufferStart >> 8) & 0xff));
	spi->transfer((uint8_t) ((bufferStart) & 0xff));

// transfer data
	for (unsigned int i = 0; i < length; i++) {
		spi->transfer(data[offset + i]);
	}
	spi->end();
	return length;
}
unsigned int DataFlash::getPage(unsigned int pos) {
	return pos / pageSize;
}

void DataFlash::flush() {
	if (bufferChangedA) {
		BufferToPage(0, (uint16_t) pageInBufferA, false);
	}
	while (!isReady())
		;
	if (bufferChangedB) {
		BufferToPage(1, (uint16_t) pageInBufferB, false);
	}
}
