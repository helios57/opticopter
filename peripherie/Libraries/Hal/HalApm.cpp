/*
 * HalApm.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: helios
 */

#include "HalApm.h"
#include "Interfaces/DigitalSource.h"
#include "Interfaces/SPIDeviceDriver.h"

static const uint8_t SPI0_MISO_PIN = 50;
static const uint8_t SPI0_MOSI_PIN = 51;
static const uint8_t SPI0_SCK_PIN = 52;
static const uint8_t MPU6000_CS_PIN = 53; //Accel and Gyro - APM pin connected to mpu6000's chip select pin
static const uint8_t MS5611_CS_PIN = 40; //Barometer

namespace arducopterNg {

	HalApm::HalApm(Stream *console, DebugStream *debug) :
			console(console), debug(debug) {
		gpio = new GPIO();
		gpio->channel(1);
		rcInput = new RCInput(gpio);
		rcOutput = new RCOutput(gpio);
		rcInput->init();
		rcOutput->init();

		/* mpu6k: divide clock by 32 to 500khz
		 * spcr gets bit SPR1,  spsr gets bit SPI2X */
		SPIDeviceDriver *mpu6k_spi = new SPIDeviceDriver(gpio, &Serial, SPI0_MISO_PIN, SPI0_MOSI_PIN, SPI0_SCK_PIN, MPU6000_CS_PIN);
		mpu6k_spi->init();

		/* ms5611: divide clock by 32 to 500khz
		 * spcr gets bit SPR1,  spsr gets bit SPI2X */
		SPIDeviceDriver *ms5611_spi = new SPIDeviceDriver(gpio, &Serial, SPI0_MISO_PIN, SPI0_MOSI_PIN, SPI0_SCK_PIN, MS5611_CS_PIN);
		ms5611_spi->init();

		//Since both (MPU6000 and MS5611) are on the same SPI-Bus, their ChipSelect has to be set before Driver-Initialisation
		mpu6k = new MPU6000(mpu6k_spi, console);
		mpu6k->init();

		ms5611 = new MS5611(ms5611_spi, console);
		ms5611->init();

		hmc5883l = new HMC5883L(console);
		hmc5883l->init();

		gps = new GPS_MTK(&Serial1, false, debug);
		gps->init();
	}

	HalApm::~HalApm() {
		delete gpio;
		delete rcInput;
		delete rcOutput;
		delete debug;
		delete mpu6k;
		delete ms5611;
		delete hmc5883l;
		delete gps;
	}

	uint16_t HalApm::getPmw(const uint8_t channel) const {
		if (channel >= IN0 && channel <= IN7) {
			return rcInput->read(channel);
		}
		if (channel >= OUT0 && channel <= OUT7) {
			return rcOutput->read(channel - OUT0);
		}
		return 0;
	}

	void HalApm::setPmw(const uint8_t channel, const uint16_t pmw) {
		if (channel >= OUT0 && channel <= OUT7 && pmw >= 1000 && pmw <= 2000) {
			rcOutput->write(channel - OUT0, pmw);
		} else if (channel >= OUT0 && channel <= OUT7 && pmw < 1000) {
			rcOutput->write(channel - OUT0, 1000);
		} else if (channel >= OUT0 && channel <= OUT7 && pmw > 2000) {
			rcOutput->write(channel - OUT0, 2000);
		}
	}

	bool HalApm::pollMotion() {
		return mpu6k->poll();
	}
	bool HalApm::pollBaro() {
		return ms5611->poll();
	}
	bool HalApm::pollMag() {
		return hmc5883l->poll();
	}
	bool HalApm::pollGPS() {
		return gps->poll();
	}
	void HalApm::getMotion6(int16_t *axyzgxyz) {
		mpu6k->getMotion6(axyzgxyz);
	}
	float HalApm::getPressure() {
		return ms5611->getPressure();
	}
	float HalApm::getTemperature() {
		return ms5611->getTemperature();
	}
	float HalApm::getBarometerAltitude() {
		return ms5611->getBarometerAltitude();
	}
	void HalApm::getHeading(int16_t xyz[]) {
		hmc5883l->getHeading(xyz);
	}
	uint8_t * HalApm::getGPSDataBuffer() {
		return gps->getDataBuffer();
	}
} /* namespace arducopterNg */
