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
	static double norm(int32_t *xyz);
	static double norm(double *wxyz);
	static void normalizeVec(int32_t *xyz, double out[]);
	static void normalize(double *quat);
	static void conjugate(const double *i_wxyz, double *o_wxyz);
	static void multiply(const double *a_wxyz, const double *b_wxyz, double *o_wxyz);
	static void multiplyVec(const double *a_xyz, const double *b_wxyz, double *o_xyz);
	static void fromEuler(const double pitch, const double yaw, const double roll, double *quat);
	virtual ~Math() {
	}
}
;

#endif /* MATH_H_ */
