/*
 * HalApm1.h
 *
 *  Created on: Jun 16, 2013
 *      Author: helios
 */

#ifndef HALAPM1_H_
#define HALAPM1_H_

#include <Interfaces/GPIO.h>
#include "Pins/RCInput.h"
#include "Pins/RCOutput.h"
#include "Sensors/MPU6000/MPU6000.h"
#include "Sensors/MS5611/MS5611.h"
#include "Sensors/HMC5883L/HMC5883L.h"
#include "Sensors/GPS/GPS_MTK.h"
#include "Util/DebugStream.h"

namespace arducopterNg {

	class HalApm {
	private:
		Stream *console;
		DebugStream *debug;
		GPIO *gpio;
		RCInput *rcInput;
		RCOutput *rcOutput;
		MPU6000 *mpu6k;
		MS5611 *ms5611;
		HMC5883L *hmc5883l;
		GPS_MTK *gps;

	public:
		const static char IN0 = 0x0;
		const static char IN1 = 0x1;
		const static char IN2 = 0x2;
		const static char IN3 = 0x3;
		const static char IN4 = 0x4;
		const static char IN5 = 0x5;
		const static char IN6 = 0x6;
		const static char IN7 = 0x7;

		const static char OUT0 = 0x8;
		const static char OUT1 = 0x9;
		const static char OUT2 = 0xA;
		const static char OUT3 = 0xB;
		const static char OUT4 = 0xC;
		const static char OUT5 = 0xD;
		const static char OUT6 = 0xE;
		const static char OUT7 = 0xF;
		HalApm(Stream *console, DebugStream *debug);
		virtual ~HalApm();
		uint16_t getPmw(const uint8_t channel) const;
		void setPmw(const uint8_t channel, const uint16_t pmw);
		void getQuat(int32_t *quat);
		/*
		 * <pre>
		 * FS_SEL | Full Scale Range   | LSB Sensitivity
		 * -------+--------------------+----------------
		 * 0      | +/- 250 degrees/s  | 131 LSB/deg/s
		 * 1      | +/- 500 degrees/s  | 65.5 LSB/deg/s
		 * 2      | +/- 1000 degrees/s | 32.8 LSB/deg/s
		 * 3      | +/- 2000 degrees/s | 16.4 LSB/deg/s
		 * </pre>
		 * use: (float) gyro[0] / 16.4 / (1L << 16) to get deg/s
		 * use: (float) (gyro[0] / 16.4) / (1L << 16) * 0.0174532925 to get rad/s
		 * */
		void getGyro(int32_t *gyro);

		/* <pre>
		 * AFS_SEL | Full Scale Range | LSB Sensitivity
		 * --------+------------------+----------------
		 * 0 | +/- 2g | 8192 LSB/mg
		 * 1 | +/- 4g | 4096 LSB/mg
		 * 2 | +/- 8g | 2048 LSB/mg
		 * 3 | +/- 16g | 1024 LSB/mg
		 * </pre>
		 * use (float) accel[0] / 1024 * 9.81 / (1L << 16) to get g1 / float
		 * */
		void getAccel(int32_t *accel);

		//Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)
		//bsp:1000.09 mbar
		float getPressure();

		//Actual temperature (-40…85°C with 0.01°C resolution)
		//bsp: 20.07 °C
		float getTemperature();

		float getBarometerAltitude();

		/** Get 3-axis heading measurements.
		 * In the event the ADC reading overflows or underflows for the given channel,
		 * or if there is a math overflow during the bias measurement, this data
		 * register will contain the value -4096.
		 * @param xyz[0] 16-bit signed integer container for X-axis heading
		 * @param xyz[1] 16-bit signed integer container for Y-axis heading
		 * @param xyz[2] 16-bit signed integer container for Z-axis heading
		 * @see HMC5883L_RA_DATAX_H
		 */
		void getHeading(int16_t xyz[]);

		/*	struct diyd_mtk_msg {
		 int32_t latitude;
		 int32_t longitude;
		 int32_t altitude;
		 int32_t ground_speed;
		 int32_t ground_course;
		 uint8_t satellites;
		 uint8_t fix_type;
		 uint32_t utc_date;
		 uint32_t utc_time;
		 uint16_t hdop;
		 };*/

		uint8_t* getGPSDataBuffer();

		/*
		 * 200 hz
		 */
		bool pollAccel();
		/*
		 * max 100 hz
		 */
		bool pollBaro();
		/*
		 * 100 hz
		 */
		bool pollMag();
		/*
		 * 10 hz
		 */
		bool pollGPS();
	};

} /* namespace arducopterNg */
#endif /* HALAPM1_H_ */
