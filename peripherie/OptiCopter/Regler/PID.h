/*
 * PID.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef PID_H_
#define PID_H_

#include <Arduino.h>

namespace opticopter {
	class PID {
	private:
		float diffSum;
		float windupGuard;
		float max;
		float lastPos;
		float lastResult;
		float lastRate;
		float accelToRateFactor;
	public:
		float kp;
		float ki;
		float kd;
		PID() {
			lastPos = 0;
			lastResult = 0;
			lastRate = 0;
			accelToRateFactor = 0;
			diffSum = 0;
			kp = 0.14; //reduzieren
			ki = 0.0;
			kd = -0.035; //erhöhen -> dämpfung
			windupGuard = PI / 2;
			max = PI / 4;
		}
		void init(float ikp, float iki, float ikd) {
			//kp = ikp;
			//ki = iki;
			//kd = ikd;
			resetI();
		}
		void resetI();
		float updatePID(float target, float cur, float rate, float dt);
		virtual ~PID() {
		}
	};
}
#endif /* PID_H_ */
