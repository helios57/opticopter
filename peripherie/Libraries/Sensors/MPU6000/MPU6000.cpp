/*
 * MPU6000.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#include "MPU6000.h"
#include <avr/pgmspace.h>
#include <string.h>

void MPU6000::delay(unsigned long ms) {
	unsigned long start = millis();
	while (start + ms > millis())
		;
}

void MPU6000::init() {
	if (initialised) {
		return;
	}
	initialised = dmpInitialize();
}
void MPU6000::getQuat(int32_t *quat) {
	quat[0] = quatW;
	quat[1] = quatX;
	quat[2] = quatY;
	quat[3] = quatZ;
}
void MPU6000::getGyro(int32_t *gyro) {
	gyro[0] = gyroX;
	gyro[1] = gyroY;
	gyro[2] = gyroZ;
}
void MPU6000::getAccel(int32_t *accel) {
	accel[0] = accelX;
	accel[1] = accelY;
	accel[2] = accelZ;
}

void MPU6000::processFIFOPacket() {
	//quat is way too slow .. useless
	//quatW = (((uint32_t) (fifoBuffer[0]) << 24) + ((uint32_t) (fifoBuffer[1]) << 16) + ((uint32_t) (fifoBuffer[2]) << 8) + fifoBuffer[3]);
	//quatX = (((uint32_t) (fifoBuffer[4]) << 24) + ((uint32_t) (fifoBuffer[5]) << 16) + ((uint32_t) (fifoBuffer[6]) << 8) + (uint32_t) (fifoBuffer[7]));
	//quatY = (((uint32_t) (fifoBuffer[8]) << 24) + ((uint32_t) (fifoBuffer[9]) << 16) + ((uint32_t) (fifoBuffer[10]) << 8) + (uint32_t) (fifoBuffer[11]));
	//quatZ = (((uint32_t) (fifoBuffer[12]) << 24) + ((uint32_t) (fifoBuffer[13]) << 16) + ((uint32_t) (fifoBuffer[14]) << 8) + (uint32_t) (fifoBuffer[15]));
	gyroX = (((uint32_t) (fifoBuffer[16]) << 24) + ((uint32_t) (fifoBuffer[17]) << 16) + ((uint32_t) (fifoBuffer[18]) << 8) + (uint32_t) (fifoBuffer[19]));
	gyroY = (((uint32_t) (fifoBuffer[20]) << 24) + ((uint32_t) (fifoBuffer[21]) << 16) + ((uint32_t) (fifoBuffer[22]) << 8) + (uint32_t) (fifoBuffer[23]));
	gyroZ = (((uint32_t) (fifoBuffer[24]) << 24) + ((uint32_t) (fifoBuffer[25]) << 16) + ((uint32_t) (fifoBuffer[26]) << 8) + (uint32_t) (fifoBuffer[27]));
	accelX = (((uint32_t) (fifoBuffer[28]) << 24) + ((uint32_t) (fifoBuffer[29]) << 16) + ((uint32_t) (fifoBuffer[30]) << 8) + (uint32_t) (fifoBuffer[31]));
	accelY = (((uint32_t) (fifoBuffer[32]) << 24) + ((uint32_t) (fifoBuffer[33]) << 16) + ((uint32_t) (fifoBuffer[34]) << 8) + (uint32_t) (fifoBuffer[35]));
	accelZ = (((uint32_t) (fifoBuffer[36]) << 24) + ((uint32_t) (fifoBuffer[37]) << 16) + ((uint32_t) (fifoBuffer[38]) << 8) + (uint32_t) (fifoBuffer[39]));
	/*
	 quatW = (fifoBuffer[0] << 8) + fifoBuffer[1];
	 quatX = (fifoBuffer[4] << 8) + fifoBuffer[5];
	 quatY = (fifoBuffer[8] << 8) + fifoBuffer[9];
	 quatZ = (fifoBuffer[12] << 8) + fifoBuffer[13];
	 gyroX = (fifoBuffer[16] << 8) + fifoBuffer[17];
	 gyroY = (fifoBuffer[20] << 8) + fifoBuffer[21];
	 gyroZ = (fifoBuffer[24] << 8) + fifoBuffer[25];
	 accelX = (fifoBuffer[28] << 8) + fifoBuffer[29];
	 accelY = (fifoBuffer[32] << 8) + fifoBuffer[33];
	 accelZ = (fifoBuffer[36] << 8) + fifoBuffer[37];
	 */
}

bool MPU6000::poll() {
	uint16_t fIfoCount = getFIFOCount();
	if (fIfoCount > 1020) {
		console->println("Fifo count too high");
		resetFIFO();
		return false;
	}
	if (fIfoCount >= fifoBufferLength) {
		readFIFOBytes(fifoBufferLength, fifoBuffer);
		processFIFOPacket();
		return true;
	} else {
		return false;
	}
}

void MPU6000::printHWRevisionInfos() {
	// get MPU hardware revision
	console->println("Selecting user bank 16...");
	setMemoryBank(0x10, true, true);
	console->println("Selecting memory byte 6...");
	setMemoryStartAddress(0x06);
	console->println("Checking hardware revision...");
	uint8_t hwRevision = register_read(MPU6050_RA_MEM_R_W);
	console->print("Revision @ user[16][6] = ");
	console->println(hwRevision, HEX);
	console->println("Resetting memory bank selection to 0...");
	setMemoryBank(0, false, false);
}

uint8_t MPU6000::dmpInitialize() {
	// reset device
	//console->println("Resetting MPU6050...");
	//reset
	register_write(MPUREG_PWR_MGMT_1, _BV(MPU6050_PWR1_DEVICE_RESET_BIT));
	delay(50); // wait after reset

	// disable sleep mode
	//console->println("Disabling sleep mode and set clock...");
	register_write(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_XGYRO);
	delay(5);

	// get MPU hardware revision
	//printHWRevisionInfos();

	// load DMP code into memory banks
	if (writeMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) {
		//console->println("Success! DMP code written and verified.");

		// write DMP configuration
		//console->print("Writing DMP configuration to MPU memory banks (");
		//console->print(MPU6050_DMP_CONFIG_SIZE);
		//console->println(" bytes in config def)");
		if (writeDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE)) {
			//console->println("Success! DMP configuration written and verified.");

			//console->println("Setting clock source to X Gyro...");
			register_write(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_XGYRO);
			delay(5);

			//console->println("Setting DMP and FIFO_OFLOW interrupts enabled...");
			register_write(MPU6050_RA_INT_ENABLE, 0x01);

			//console->println("Setting sample rate to 200Hz...");
			register_write(MPU6050_RA_SMPLRT_DIV, 4); // 1khz / (1 + 4) = 200 Hz

			//console->print("Setting external frame sync to TEMP_OUT_L[0]...");
			//setExternalFrameSync (MPU6050_EXT_SYNC_TEMP_OUT_L);

			//console->println("Setting DLPF bandwidth to 42Hz...");

			uint8_t oldConfig = register_read(MPU6050_RA_CONFIG);
			oldConfig = (oldConfig & ~0x07) | MPU6050_DLPF_BW_42;
			register_write(MPU6050_RA_CONFIG, oldConfig);

			//console->println("Setting gyro sensitivity to +/- 250 deg/sec...");

			//Gyro to max accuracy (+/- 2000 degrees/sec)
			register_write(MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_2000);

			//console->println("Setting accel sensitivity to +/- 16g ...");
			//Accel to max accuracy (+/- 2g)
			register_write(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_16);

			// Disable I2C bus (recommended on datasheet)
			register_write(MPU6050_RA_USER_CTRL, BIT_USER_CTRL_I2C_IF_DIS);

			//console->println("Setting DMP configuration bytes (function unknown)...");
			register_write(MPU6050_RA_DMP_CFG_1, 0x03);
			register_write(MPU6050_RA_DMP_CFG_2, 0x00);

			//console->println("Clearing OTP Bank flag...");
			register_write(MPU6050_RA_XG_OFFS_TC, 0x00);

			//console->println("Writing final memory update 1/7 (function unknown)...");
			uint8_t dmpUpdate[16], j;
			uint16_t pos = 0;
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Writing final memory update 2/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Resetting FIFO...");
			resetFIFO();

			//console->println("Reading FIFO count...");

			uint16_t fifoCount = getFIFOCount();

			//console->print("Current FIFO count=");
			//console->println(fifoCount);

			readFIFOBytes(fifoBufferLength, fifoBuffer);

			//console->println("Setting motion detection threshold to 2...");
			register_write(MPU6050_RA_MOT_THR, 2);

			//console->println("Setting zero-motion detection threshold to 156...");
			register_write(MPU6050_RA_ZRMOT_THR, 156);

			//console->println("Setting motion detection duration to 80...");
			register_write(MPU6050_RA_MOT_DUR, 80);

			//console->println("Setting zero-motion detection duration to 0...");
			register_write(MPU6050_RA_ZRMOT_DUR, 0);

			//console->println("Resetting FIFO...");
			resetFIFO();

			//console->println("Enabling FIFO...");
			enableFIFO();

			//console->println("Enabling DMP...");
			enableDMP();

			//console->println("Resetting DMP...");
			resetDMP();

			//console->println("Writing final memory update 3/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Writing final memory update 4/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Writing final memory update 5/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Waiting for FIFO count > 2...");
			while ((fifoCount = getFIFOCount()) < 3)
				;

			//console->print("Current FIFO count=");
			//console->println(fifoCount);
			//console->println("Reading FIFO data...");
			readFIFOBytes(fifoCount, fifoBuffer);

			//console->println("Reading interrupt status...");
			//uint8_t mpuIntStatus = getIntStatus();

			//console->print("Current interrupt status=");
			//console->println(mpuIntStatus, HEX);

			//console->println("Reading final memory update 6/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("Waiting for FIFO count > 2...");
			while ((fifoCount = getFIFOCount()) < 3)
				;

			//console->print("Current FIFO count=");
			//console->println(fifoCount);

			//console->println("Reading FIFO data...");
			readFIFOBytes(fifoCount, fifoBuffer);

			//console->println("Reading interrupt status...");
			//mpuIntStatus = getIntStatus();

			//console->println("Current interrupt status=");
			//console->println(mpuIntStatus, HEX);

			//console->println("Writing final memory update 7/7 (function unknown)...");
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = dmpUpdates[pos];
			writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

			//console->println("DMP is good to go! Finally.");

			//console->println("Disabling DMP (you turn it on later)...");
			disableDMP();

			//console->println("Resetting FIFO and clearing INT status one last time...");
			resetFIFO();
			getIntStatus();
			enableDMP();
		} else {
			console->println("ERROR! DMP configuration verification failed.");
			return 2; // configuration block loading failed
		}
	} else {
		console->println("ERROR! DMP code verification failed.");
		return 1; // main binary block loading failed
	}
	return 0; // success
}

uint8_t MPU6000::register_read(uint8_t reg) {
	spi->begin();
	spi->transfer(reg | 0x80);  // Set most significant bit
	uint8_t result = spi->transfer(0);
	spi->end();
	return result;
}

void MPU6000::register_write(uint8_t reg, uint8_t val) {
	spi->begin();
	spi->transfer(reg);
	spi->transfer(val);
	spi->end();
}

void MPU6000::setMemoryBank(uint8_t bank, bool prefetchEnabled, bool userBank) {
	bank &= 0x1F;
	if (userBank)
		bank |= 0x20;
	if (prefetchEnabled)
		bank |= 0x40;
	register_write(MPU6050_RA_BANK_SEL, bank);
}

void MPU6000::setMemoryStartAddress(uint8_t address) {
	register_write(MPU6050_RA_MEM_START_ADDR, address);
}

bool MPU6000::writeMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify) {
	uint8_t addr = address;
	uint8_t bnk = bank;
	for (uint16_t i = 0; i < dataSize; i++) {
		register_write(MPU6050_RA_BANK_SEL, bnk);
		register_write(MPU6050_RA_MEM_START_ADDR, addr);
		register_write(MPU6050_RA_MEM_R_W, *(data + i));
		if (verify) {
			register_write(MPU6050_RA_MEM_START_ADDR, addr);
			uint8_t read = register_read(MPU6050_RA_MEM_R_W);
			if (read != *(data + i)) {
				return false;
			}
		}
		if (addr >= (MPU6050_DMP_MEMORY_BANK_SIZE)) {
			bnk++;
			addr = 0;
		} else {
			addr++;
		}
	}
	return true;
}

void MPU6000::readMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address) {
	setMemoryBank(bank);
	setMemoryStartAddress(address);
	uint8_t chunkSize;
	for (uint16_t i = 0; i < dataSize;) {
		// determine correct chunk size according to bank position and data size
		chunkSize = 16;

		// make sure we don't go past the data size
		if (i + chunkSize > dataSize)
			chunkSize = dataSize - i;

		// make sure this chunk doesn't go past the bank boundary (256 bytes)
		if (chunkSize > 256 - address)
			chunkSize = 256 - address;

		// read the chunk of data as specified
		spi->begin();
		spi->transfer(MPU6050_RA_MEM_R_W | 0x80);
		for (uint8_t j = 0; j < chunkSize; j++) {
			data[i + j] = spi->transfer(0);
		}
		spi->end();

		// increase byte index by [chunkSize]
		i += chunkSize;

		// uint8_t automatically wraps to 0 at 256
		address += chunkSize;

		// if we aren't done, update bank (if necessary) and address
		if (i < dataSize) {
			if (address == 0)
				bank++;
			setMemoryBank(bank);
			setMemoryStartAddress(address);
		}
	}
}

bool MPU6000::writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize) {
	uint8_t *progBuffer, success, special;
	uint16_t i;

	// config set data is a long string of blocks with the following structure:
	// [bank] [offset] [length] [byte[0], byte[1], ..., byte[length]]
	uint8_t bank, offset, length;
	for (i = 0; i < dataSize;) {
		bank = data[i++];
		offset = data[i++];
		length = data[i++];

		// write data or perform special action
		if (length > 0) {
			progBuffer = (uint8_t *) data + i;
			success = writeMemoryBlock(progBuffer, length, bank, offset, true);
			i += length;
		} else {
			// special instruction
			// NOTE: this kind of behavior (what and when to do certain things)
			// is totally undocumented. This code is in here based on observed
			// behavior only, and exactly why (or even whether) it has to be here
			// is anybody's guess for now.
			special = data[i++];
			/*Serial.print("Special command code ");
			 Serial.print(special, HEX);
			 Serial.println(" found...");*/
			if (special == 0x01) {
				// enable DMP-related interrupts

				//setIntZeroMotionEnabled(true);
				//setIntFIFOBufferOverflowEnabled(true);
				//setIntDMPEnabled(true);
				register_write(MPU6050_RA_INT_ENABLE, 0x32);  // single operation

				success = true;
			} else {
				// unknown special command
				success = false;
			}
		}

		if (!success) {
			return false;
		}
	}
	return true;
}

uint16_t MPU6000::getFIFOCount() {
	uint8_t fifoCountH = register_read(MPU6050_RA_FIFO_COUNTH);
	uint8_t fifoCountL = register_read(MPU6050_RA_FIFO_COUNTL);
	uint16_t fifoCount = (((uint16_t) (fifoCountH)) << 8) | fifoCountL;
	return fifoCount;
}

void MPU6000::resetFIFO() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_FIFO_RESET_BIT));
}

void MPU6000::readFIFOBytes(uint16_t fifoCount, uint8_t *fifoBuffer) {
	spi->begin();
	spi->transfer(MPU6050_RA_FIFO_R_W | 0x80);
	for (unsigned short i = 0; i < fifoCount; i++) {
		fifoBuffer[i] = spi->transfer(0);
	}
	spi->end();
}

void MPU6000::enableDMP() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_DMP_EN_BIT));
}

void MPU6000::disableDMP() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) & ~_BV(MPU6050_USERCTRL_DMP_EN_BIT));
}

void MPU6000::resetDMP() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_DMP_RESET_BIT));
}

void MPU6000::enableFIFO() {
	register_write(MPU6050_RA_USER_CTRL, register_read(MPU6050_RA_USER_CTRL) | _BV(MPU6050_USERCTRL_FIFO_EN_BIT));
}

uint8_t MPU6000::getIntStatus() {
	return register_read(MPU6050_RA_INT_STATUS);
}

