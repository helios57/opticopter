/*
 * Gyro.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: helios
 */

#include "Gyro.h"

Gyro::Gyro() {
	for (int i = 0; i < 3; i++) {
		gyroSummed[i] = 0;
		gyroBias[i] = 0;
	}
	gyroCount = 0;
	recording = false;
}

void Gyro::update(int16_t* motion6) {
	for (int i = 0; i < 3; i++) {
		gyro[i] = motion6[3 + i];
		if (recording) {
			gyroSummed[i] += gyro[i];
		}
	}
}

void Gyro::startBiasRecording() {
	for (int i = 0; i < 3; i++) {
		gyroSummed[i] = 0;
	}
	gyroCount = 0;
	recording = true;
}

void Gyro::stopBiasRecording() {
	for (int i = 0; i < 3; i++) {
		gyroBias[i] = gyroSummed[i] / gyroCount;
		gyroSummed[i] = 0;
	}
	gyroCount = 0;
	recording = false;
}

Gyro::~Gyro() {
}

float Gyro::getRoll() {
	return (gyro[1] - gyroBias[1]) * gyroScaleFactor;
}

float Gyro::getPitch() {
	return (gyro[0] - gyroBias[0]) * gyroScaleFactor;
}

float Gyro::getYaw() {
	return (gyro[2] - gyroBias[2]) * gyroScaleFactor;
}
