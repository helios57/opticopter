package ch.sharpsoft.arducopter.client.model;

import static java.lang.Math.PI;

public class PID2 {
	double errSum;
	double errSumMax;
	double lastInput;
	double kp;
	double ki;
	double kd;
	double sampleTime = 20.0; // 20ms

	/**
	 * input in rad
	 */
	double update(final double setpoint, final double current) {
		double error = setpoint - current;
		errSum += error;
		if (errSum > errSumMax) {
			errSum = errSumMax;
		}
		if (errSum < -errSumMax) {
			errSum = -errSumMax;
		}
		double dInput = (current - lastInput);
		lastInput = current;
		return kp * error + ki * errSum - kd * dInput;
	}

	void setParams(final double p, final double i, final double d) {
		double sampleTimeInSec = sampleTime / 1000;
		kp = p;
		ki = i / sampleTimeInSec;
		kd = d * sampleTimeInSec;
		errSumMax = (PI / ki) / 16;
	}
}
