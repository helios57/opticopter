/*
 * Quaternion.h
 *
 *  Created on: Nov 21, 2013
 *      Author: helios
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

class Quaternion {
public:
	Quaternion() {
	}
	virtual ~Quaternion() {
	}

	/**
	 * a and b may not be the same quat as out
	 */
	void static diff(float* a, float* b, float* out) {
		out[0] = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
		out[1] = a[0] * b[1] - a[1] * b[0] - a[2] * b[3] + a[3] * b[2];
		out[2] = a[0] * b[2] + a[1] * b[3] - a[2] * b[0] - a[3] * b[1];
		out[3] = a[0] * b[3] - a[1] * b[2] + a[2] * b[1] - a[3] * b[0];

		float recipNorm = 1 / sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2] + out[3] * out[3]);
		out[0] *= recipNorm;
		out[1] *= recipNorm;
		out[2] *= recipNorm;
		out[3] *= recipNorm;
	}

	/**
	 * a and b may not be the same quat as out
	 *
	 * return a new Quaternion whose value is (a * b)
	 */
	static void multiply(float* a, float* b, float* out) {
		out[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
		out[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
		out[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
		out[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];

		float recipNorm = 1 / sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2] + out[3] * out[3]);
		out[0] *= recipNorm;
		out[1] *= recipNorm;
		out[2] *= recipNorm;
		out[3] *= recipNorm;
	}

	static void fromEuler(float roll, float pitch, float yaw, float* wxyz) {
		// Basically we create 3 Quaternions, one for pitch, one for yaw, one
		// for roll
		// and multiply those together.
		// the calculation below does the same, just shorter

		float p = (float) (pitch / 2.0);
		float y = (float) (yaw / 2.0);
		float r = (float) (roll / 2.0);

		float sinp = (float) sin(p);
		float siny = (float) sin(y);
		float sinr = (float) sin(r);
		float cosp = (float) cos(p);
		float cosy = (float) cos(y);
		float cosr = (float) cos(r);

		wxyz[0] = cosr * cosp * cosy + sinr * sinp * siny;
		wxyz[1] = sinr * cosp * cosy - cosr * sinp * siny;
		wxyz[2] = cosr * sinp * cosy + sinr * cosp * siny;
		wxyz[3] = cosr * cosp * siny - sinr * sinp * cosy;
	}
};

#endif /* QUATERNION_H_ */
