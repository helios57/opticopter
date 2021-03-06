/*
 * Gyro.h
 *
 *  Created on: Nov 25, 2013
 *      Author: helios
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <Arduino.h>

namespace opticopter {
	class Gyro {
	private:
		static constexpr float gyroScaleFactor = 0.00106413 * 0.25;
		int16_t gyro[3];
		int16_t gyroBias[3];
		int32_t gyroSummed[3];
		int16_t gyroCount;
		bool recording;
	public:
		void startBiasRecording();
		void stopBiasRecording();
		void update(int16_t* motion6);
		float getY();
		float getX();
		float getZ();
		Gyro();
		virtual ~Gyro();
	};
}
#endif /* GYRO_H_ */
