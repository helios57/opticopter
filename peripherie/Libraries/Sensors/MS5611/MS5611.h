/*
 * MS5611.h
 *
 *  Created on: Aug 17, 2013
 *      Author: helios
 */

#ifndef MS5611_H_
#define MS5611_H_

#include <stdint.h>
#include <Stream.h>
#include <wiring_private.h>

#include <Interfaces/DigitalSource.h>
#include <Interfaces/SPIDeviceDriver.h>

class MS5611 {
private:
	SPIDeviceDriver *spi;
	Stream *console;
	bool initialised;
	bool calculated;
	uint16_t register_read(uint8_t reg);
	void register_write(uint8_t reg);
	void readPressure();
	void readTemperature();
	void calculate();
	void delay(unsigned long ms);
	void read4BytesIntoBuffer();
	void sendTempD2Command();
	void sendPressD1Command();

	uint32_t digitalPress_D1; //Digital pressure value
	uint32_t digitalTemp_D2; //Digital temperature value
	uint32_t digitalPress_D1_avg; //Digital pressure value
	uint32_t digitalTemp_D2_avg; //Digital temperature value
	float dT; //Difference between actual and reference temperature
	float dT2; //Difference between actual and reference temperature
	float temp; //Actual temperature (-40…85°C with 0.01°C resolution)
	float off; //Offset at actual temperature
	float off2; //Offset at actual temperature
	float sens; //Sensitivity at actual temperature
	float sens2; //Sensitivity at actual temperature
	float press; //Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)
	uint32_t pressSens_C1; //Pressure sensitivity | SENS
	uint32_t pressOffset_C2; //Pressure offset | OFF
	uint16_t tempCoeffPressSens_C3; //Temperature coefficient of pressure sensitivity | TCS
	uint16_t tempCoeffPressOffset_C4; //Temperature coefficient of pressure offset | TCO
	uint16_t refTemp_C5; //Reference temperature | T
	uint16_t tempCoeffTemp_C6; //Temperature coefficient of the temperature | TEMPSENS
	uint16_t buff[4];
	uint32_t digitalPress_D1_ring[10];
	int8_t digitalPress_D1_ringIndex;
	uint32_t digitalTemp_D2_ring[10];
	int8_t digitalTemp_D2_ringIndex;
	int8_t readState;

	static const float sea_press = 1013.25;

	static const uint8_t CMD_MS5611_RESET = 0x1E;
	static const uint8_t CMD_MS5611_PROM_Setup = 0xA0;
	static const uint8_t CMD_MS5611_PROM_C1 = 0xA2; //Pressure sensitivity | SENS
	static const uint8_t CMD_MS5611_PROM_C2 = 0xA4; //Pressure offset | OFF
	static const uint8_t CMD_MS5611_PROM_C3 = 0xA6; //Temperature coefficient of pressure sensitivity | TCS
	static const uint8_t CMD_MS5611_PROM_C4 = 0xA8; //Temperature coefficient of pressure offset | TCO
	static const uint8_t CMD_MS5611_PROM_C5 = 0xAA; //Reference temperature | T
	static const uint8_t CMD_MS5611_PROM_C6 = 0xAC; //Temperature coefficient of the temperature | TEMPSENS
	static const uint8_t CMD_MS5611_PROM_CRC = 0xAE;
	static const uint8_t CMD_CONVERT_D1_OSR4096 = 0x48;   // Maximum resolution (oversampling)
	static const uint8_t CMD_CONVERT_D2_OSR4096 = 0x58;   // Maximum resolution (oversampling)

public:
	MS5611(SPIDeviceDriver *spi, Stream *console) :
			spi(spi), console(console) {
		initialised = false;
		calculated = false;
		digitalPress_D1 = 0;
		digitalTemp_D2 = 0;
		pressSens_C1 = 0;
		pressOffset_C2 = 0;
		tempCoeffPressSens_C3 = 0;
		tempCoeffPressOffset_C4 = 0;
		refTemp_C5 = 0;
		tempCoeffTemp_C6 = 0;
		dT = 0;
		dT2 = 0;
		temp = 0;
		off = 0;
		off2 = 0;
		sens = 0;
		sens2 = 0;
		press = 0;
		readState = 1;
		digitalPress_D1_ringIndex = 0;
		digitalTemp_D2_ringIndex = 0;
		digitalPress_D1_avg = 0;
		digitalTemp_D2_avg = 0;
		for (int i = 0; i < 9; i++) {
			digitalPress_D1_ring[i] = 0;
			digitalTemp_D2_ring[i] = 0;
		}
	}

	virtual ~MS5611() {

	}
	void init();
	bool poll();
	uint32_t getDigitalPressure();
	uint32_t getDigitalTemperature();

	//Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)
	//100009 = 1000.09 mbar
	float getPressure();

	//Actual temperature (-40…85°C with 0.01°C resolution)
	//2007 = 20.07 °C
	float getTemperature();

	float getBarometerAltitude();
};

#endif /* MS5611_H_ */
