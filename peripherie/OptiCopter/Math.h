/*
 * Math.h
 *
 *  Created on: Sep 15, 2013
 *      Author: helios
 */

#ifndef MATH_H_
#define MATH_H_
#include <stdint.h>
class Math {
public:
	Math() {
	}
	static float norm(int32_t *xyz);
	static float norm(float *wxyz);
	static void normalizeVec(int32_t *xyz, float out[]);
	static void normalize(float *quat);
	static void conjugate(const float *i_wxyz, float *o_wxyz);
	static void multiply(const float *a_wxyz, const float *b_wxyz, float *o_wxyz);
	static void multiplyVec(const float *a_xyz, const float *b_wxyz, float *o_xyz);
	static void fromEuler(const float pitch, const float yaw, const float roll, float *quat);
	virtual ~Math() {
	}
}
;

#endif /* MATH_H_ */
