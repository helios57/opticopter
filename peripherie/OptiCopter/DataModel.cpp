/*
 * DataModel.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: helios
 */

#include "DataModel.h"
#include "Arduino.h"

namespace opticopter {
	void DataModel::calculateActivation() {
		if (!active && input[3] > activateTop && tActivate == 0) {
			tActivate = millis();
			startActivate();
		} else if (!active && input[3] > activateTop && (tActivate + 5000) < millis()) {
			active = true;
			tActivate = 0;
			onActivate();
		}
		if (active && input[3] < activateBot && tActivate == 0) {
			tActivate = millis();
		} else if (active && input[3] < activateBot && (tActivate + 5000) < millis()) {
			active = false;
			onDeactivate();
			tActivate = 0;
		}
	}

	void DataModel::startActivate() {
		gyro.startBiasRecording();
	}

	void DataModel::onActivate() {
		if (leveling) {
			rollPitchYawLevel[0] = rollPitchYaw[0];
			rollPitchYawLevel[1] = rollPitchYaw[1];
			rollPitchYawLevel[2] = rollPitchYaw[2];
		} else {
			persistence->readRollPitchYawLevel(rollPitchYawLevel);
			rollPitchYawLevel[2] = rollPitchYaw[2];
		}
		rollPitchYawPid[0].resetI();
		rollPitchYawPid[1].resetI();
		rollPitchYawPid[2].resetI();
		gyro.stopBiasRecording();
		ahrs.onActivate();
	}
	void DataModel::onDeactivate() {
		hal->flushData();
	}

	void DataModel::calc(float dt) {
		calcAhrs(dt);
		calcOutput(dt);
	}

	void DataModel::putMotion6(int16_t *axyzgxyz) {
		for (uint8_t i = 0; i < 6; i++) {
			motion[i] = axyzgxyz[i];
		}
		gyro.update(axyzgxyz);
	}

	void DataModel::putMag(int16_t* imag) {
		for (uint8_t i = 0; i < 3; i++) {
			mag[i] = imag[i];
		}
	}

	void DataModel::calcAhrs(float dt) {
		for (int i = 0; i < 3; i++) {
			magScaled[i] = (((float) ((mag[i])) - magMin[i]) / (magMax[i] - magMin[i])) * 2 - 1.0;
		}
		magScaled[2] = -magScaled[2];
		ahrs.update(gyro.getX(), gyro.getY(), gyro.getZ(), motion[0], motion[1], motion[2], magScaled[0], magScaled[1], magScaled[2], dt);
		rollPitchYaw[0] = ahrs.getRoll();
		rollPitchYaw[1] = ahrs.getPitch();
		rollPitchYaw[2] = ahrs.getYaw();
	}

	void DataModel::calcOutput(float dt) {
		float rollLevel = rollPitchYawLevel[0];
		float pitchLevel = rollPitchYawLevel[1];
		float yawLevel = rollPitchYawLevel[2];

		rollLevel += inputRoll;
		pitchLevel += inputPitch;
		yawLevel += inputYaw;

		float yawCurrent = rollPitchYaw[2];
		while (yawLevel > PI) {
			yawLevel -= PI * 2;
		}
		while (yawLevel < -PI) {
			yawLevel += PI * 2;
		}
		double diff = yawLevel - yawCurrent;
		if (diff < -PI) {
			yawLevel += 2 * PI;
		}
		if (diff > PI) {
			yawLevel -= 2 * PI;
		}

		float rollA = rollPitchYawPid[0].updatePID(rollLevel, rollPitchYaw[0], gyro.getY(), dt);
		float pitchA = rollPitchYawPid[1].updatePID(pitchLevel, rollPitchYaw[1], gyro.getX(), dt);
		float yawA = -rollPitchYawPid[2].updatePID(yawLevel, rollPitchYaw[2], gyro.getZ(), dt);

		logging->getEntry()->roll = rollPitchYaw[0];
		logging->getEntry()->pitch = rollPitchYaw[1];
		logging->getEntry()->yaw = rollPitchYaw[2];
		logging->getEntry()->rollLevel = rollLevel;
		logging->getEntry()->pitchLevel = pitchLevel;
		logging->getEntry()->yawLevel = yawLevel;
		logging->getEntry()->rollPid = rollA;
		logging->getEntry()->pitchPid = pitchA;
		logging->getEntry()->yawPid = yawA;
		logging->getEntry()->accelX = motion[0];
		logging->getEntry()->accelY = motion[1];
		logging->getEntry()->accelZ = motion[2];
		logging->getEntry()->gyroX = gyro.getX();
		logging->getEntry()->gyroY = gyro.getY();
		logging->getEntry()->gyroZ = gyro.getZ();
		logging->getEntry()->magX = magScaled[0];
		logging->getEntry()->magY = magScaled[1];
		logging->getEntry()->magZ = magScaled[2];
		logging->getEntry()->timestamp = millis();
		calcMotorThrust(rollA, pitchA, yawA);
	}

	void DataModel::calcMotorThrust(float rollA, float pitchA, float yawA) {
		thrust[0] = inputThrust + rollA + yawA;
		thrust[1] = inputThrust - rollA + yawA;
		thrust[2] = inputThrust - pitchA - yawA;
		thrust[3] = inputThrust + pitchA - yawA;

		/*float factor = 1.0;
		 thrust[0] = inputThrust * (1.0 + factor * (-rollA - yawA));
		 thrust[1] = inputThrust * (1.0 + factor * (rollA + yawA));
		 thrust[2] = inputThrust * (1.0 + factor * (rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 - yawA));
		 thrust[3] = inputThrust * (1.0 + factor * (-rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 + yawA));
		 thrust[4] = inputThrust * (1.0 + factor * (-rollA * SIN_60_COS_30 + pitchA * SIN_60_COS_30 + yawA));
		 thrust[5] = inputThrust * (1.0 + factor * (rollA * SIN_60_COS_30 - pitchA * SIN_60_COS_30 - yawA));
		 */
		for (int i = 0; i < 4; i++) {
			if (thrust[i] > 1) {
				thrust[i] = 1;
			} else if (thrust[i] < 0) {
				thrust[i] = 0;
			}

			if (active && inputThrust > 0.01) {
				output[i] = (inputMin[0] + ((inputMax[0] - inputMin[0]) * (thrust[i])));
			} else {
				output[i] = inputMin[0];
			}
		}
		hal->setPmw(hal->OUT0, output[0]);
		hal->setPmw(hal->OUT1, output[1]);
		hal->setPmw(hal->OUT2, output[2]);
		hal->setPmw(hal->OUT3, output[3]);
		//hal->setPmw(hal->OUT4, output[4]);
		//hal->setPmw(hal->OUT5, output[5]);

		logging->getEntry()->output0 = output[0];
		logging->getEntry()->output1 = output[1];
		logging->getEntry()->output2 = output[2];
		logging->getEntry()->output3 = output[3];
		if (active) {
			logging->save();
		}
	}

	void DataModel::putBaro50ms(float altitude) {
	}

	void DataModel::putInput50ms(uint8_t ch, uint16_t pwm) {
		input[ch] = pwm;
	}

	void DataModel::calc50ms() {
		inputRoll = ((float) input[0] - inputDefault[0]) / (float) (inputMax[0] - inputMin[0]);
		inputPitch = ((float) input[1] - inputDefault[1]) / (float) (inputMax[1] - inputMin[1]);
		inputThrust = ((float) input[2] - inputDefault[2]) / (float) (inputMax[2] - inputMin[2]);
		inputYaw = ((float) input[3] - inputDefault[3]) / (float) (inputMax[3] - inputMin[3]);
		bool newLeveling = input[4] < 1500;

		//Falling edge on leveling -> persist current state
		if (leveling && !newLeveling) {
			persistence->saveRollPitchYawLevel(rollPitchYawLevel);
		}

		leveling = newLeveling;
		if (leveling) {
			calcLeveling();
		}
	}

	void DataModel::calcLeveling() {
		if (inputRoll > 0.1 || inputRoll < 0.1) {
			rollPitchYawLevel[0] += inputRoll * 0.01;
		}
		if (inputPitch > 0.1 || inputPitch < 0.1) {
			rollPitchYawLevel[1] += inputPitch * 0.01;
		}
	}

	static long count = 0;

	void DataModel::debugLog() {
		if (count++ >= 0) {
			Serial.print(millis());
			Serial.print(",");
			count = 0;
			for (uint8_t i = 0; i < 6; i++) {
				Serial.print(motion[i]);
				Serial.print(",");
			}
			for (uint8_t i = 0; i < 3; i++) {
				Serial.print((int) (magScaled[i] * 1000));
				Serial.print(",");
			}
			for (uint8_t i = 0; i < 4; i++) {
				Serial.print(output[i]);
				Serial.print(",");
			}
			Serial.print((int) (inputRoll * 1000));
			Serial.print(",");
			Serial.print((int) (inputPitch * 1000));
			Serial.print(",");
			Serial.print((int) (inputThrust * 1000));
			Serial.print(",");
			Serial.println((int) (inputYaw * 1000));
		}
	}

	void DataModel::initPID(float* rollPitchYawPidParams) {
		rollPitchYawPid[0].init(rollPitchYawPidParams[0], rollPitchYawPidParams[1], rollPitchYawPidParams[2]);
		rollPitchYawPid[1].init(rollPitchYawPidParams[3], rollPitchYawPidParams[4], rollPitchYawPidParams[5]);
		rollPitchYawPid[2].init(rollPitchYawPidParams[6], rollPitchYawPidParams[7], rollPitchYawPidParams[8]);
	}
}
