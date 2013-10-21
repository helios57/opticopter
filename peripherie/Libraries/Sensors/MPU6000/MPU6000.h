/*
 * MPU6000.h
 *
 *  Created on: Jul 20, 2013
 *      Author: helios
 */

#ifndef MPU6000_H_
#define MPU6000_H_

#include <stdint.h>
#include <Stream.h>
#include <wiring_private.h>

#include <Interfaces/DigitalSource.h>
#include <Interfaces/SPIDeviceDriver.h>

class MPU6000 {

private:
	SPIDeviceDriver *spi;
	Stream *console;
	uint8_t regBuffer[14];
	bool initialised;
	void delay(unsigned long ms);
	uint8_t register_read(uint8_t reg);
	void register_write(uint8_t reg, uint8_t val);
	uint8_t initialize();
	uint8_t getIntStatus();

public:
	MPU6000(SPIDeviceDriver *spi, Stream *console) :
			spi(spi), console(console) {
		initialised = false;
	}
	~MPU6000() {
	}
	void init();
	/** Get raw 6-axis motion sensor readings (accel/gyro).
	 * Retrieves all currently available motion sensor values.
	 * | +/- 16g          | 1024 LSB/mg
	 * @param ax 16-bit signed integer container for accelerometer X-axis value
	 * @param ay 16-bit signed integer container for accelerometer Y-axis value
	 * @param az 16-bit signed integer container for accelerometer Z-axis value
	 * +/- 2000 degrees/s | 16.4 LSB/deg/s
	 * @param gx 16-bit signed integer container for gyroscope X-axis value
	 * @param gy 16-bit signed integer container for gyroscope Y-axis value
	 * @param gz 16-bit signed integer container for gyroscope Z-axis value
	 * @see MPU6050_RA_ACCEL_XOUT_H
	 */
	void getMotion6(int16_t *axyzgxyz);
}
;

static const uint8_t MPUREG_PWR_MGMT_1 = 0x6B;
static const uint8_t BIT_PWR_MGMT_1_CLK_XGYRO = 0x01;
static const uint8_t BIT_USER_CTRL_I2C_IF_DIS = 0x10;
static const uint8_t MPU6050_PWR1_DEVICE_RESET_BIT = 7;
static const uint8_t MPU6050_PWR1_SLEEP_BIT = 6;
static const uint8_t MPU6050_RA_USER_CTRL = 0x6A;
static const uint8_t MPU6050_RA_BANK_SEL = 0x6D;
static const uint8_t MPU6050_RA_MEM_START_ADDR = 0x6E;
static const uint8_t MPU6050_RA_MEM_R_W = 0x6F;
static const uint8_t MPU6050_RA_XG_OFFS_TC = 0x00; //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
static const uint8_t MPU6050_RA_YG_OFFS_TC = 0x01; //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
static const uint8_t MPU6050_RA_ZG_OFFS_TC = 0x02; //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
static const uint8_t MPU6050_RA_INT_ENABLE = 0x38;
static const uint8_t MPU6050_RA_SMPLRT_DIV = 0x19;
static const uint8_t MPU6050_DLPF_BW_42 = 0x03;
static const uint8_t MPU6050_RA_CONFIG = 0x1A;
static const uint8_t MPU6050_RA_DMP_CFG_1 = 0x70;
static const uint8_t MPU6050_RA_DMP_CFG_2 = 0x71;
static const uint8_t MPU6050_RA_FIFO_COUNTH = 0x72;
static const uint8_t MPU6050_RA_FIFO_COUNTL = 0x73;
static const uint8_t MPU6050_RA_FIFO_R_W = 0x74;
static const uint8_t MPU6050_RA_MOT_THR = 0x1F;
static const uint8_t MPU6050_RA_MOT_DUR = 0x20;
static const uint8_t MPU6050_RA_ZRMOT_THR = 0x21;
static const uint8_t MPU6050_RA_ZRMOT_DUR = 0x22;
static const uint8_t MPU6050_RA_INT_STATUS = 0x3A;
static const uint8_t MPU6050_RA_GYRO_CONFIG = 0x1B;
static const uint8_t MPU6050_GYRO_FS_250 = 0x00;
static const uint8_t MPU6050_GYRO_FS_2000 = 0x03;
static const uint8_t MPU6050_RA_ACCEL_CONFIG = 0x1C;
static const uint8_t MPU6050_ACCEL_FS_2 = 0x00;
static const uint8_t MPU6050_ACCEL_FS_16 = 0x03;
static const uint8_t MPU6050_RA_ACCEL_XOUT_H = 0x3B;
static const uint8_t MPU6050_TC_OTP_BNK_VLD_BIT = 0;
static const uint8_t MPU6050_USERCTRL_DMP_EN_BIT = 7;
static const uint8_t MPU6050_USERCTRL_FIFO_EN_BIT = 6;
static const uint8_t MPU6050_USERCTRL_DMP_RESET_BIT = 3;
static const uint8_t MPU6050_USERCTRL_FIFO_RESET_BIT = 2;

#endif /* MPU6000_H_ */
