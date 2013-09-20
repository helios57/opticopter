/*
 * Math.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: helios
 */

#include "Math.h"
#include <math.h>

double Math::norm(int32_t *xyz) {
	return sqrt(square(xyz[0]) + square(xyz[1]) + square(xyz[2]));
}
void Math::normalizeVec(int32_t *xyz, double *out) {
	double n = norm(xyz);
	out[0] = xyz[0] / n;
	out[1] = xyz[1] / n;
	out[2] = xyz[2] / n;
}
double Math::norm(double *wxyz) {
	return sqrt(square(wxyz[0]) + square(wxyz[1]) + square(wxyz[2]) + square(wxyz[3]));
}
void Math::normalize(double *quat) {
	double n = norm(quat);
	quat[0] = quat[0] / n;
	quat[1] = quat[1] / n;
	quat[2] = quat[2] / n;
	quat[3] = quat[3] / n;
}
void Math::conjugate(const double *i_wxyz, double *o_wxyz) {
	o_wxyz[0] = i_wxyz[0];
	o_wxyz[1] = -i_wxyz[1];
	o_wxyz[2] = -i_wxyz[2];
	o_wxyz[3] = -i_wxyz[3];
}

void Math::multiply(const double *a_wxyz, const double *b_wxyz, double *o_wxyz) {
	o_wxyz[0] = a_wxyz[0] * b_wxyz[0] - a_wxyz[1] * b_wxyz[1] - a_wxyz[2] * b_wxyz[2] - a_wxyz[3] * b_wxyz[3];
	o_wxyz[1] = a_wxyz[0] * b_wxyz[1] + a_wxyz[1] * b_wxyz[0] + a_wxyz[2] * b_wxyz[3] - a_wxyz[3] * b_wxyz[2];
	o_wxyz[2] = a_wxyz[0] * b_wxyz[2] - a_wxyz[1] * b_wxyz[3] + a_wxyz[2] * b_wxyz[0] + a_wxyz[3] * b_wxyz[1];
	o_wxyz[3] = a_wxyz[0] * b_wxyz[3] + a_wxyz[1] * b_wxyz[2] - a_wxyz[2] * b_wxyz[1] + a_wxyz[3] * b_wxyz[0];
}

void Math::multiplyVec(const double *a_xyz, const double *b_wxyz, double *o_xyz) {
	double vec[] = { 0, a_xyz[0], a_xyz[1], a_xyz[2] };
	double quat[] = { b_wxyz[0], b_wxyz[1], b_wxyz[2], b_wxyz[3] };
	multiply(quat, vec, vec);
	conjugate(quat, quat);
	multiply(vec, quat, vec);
	o_xyz[0] = vec[1];
	o_xyz[1] = vec[2];
	o_xyz[2] = vec[3];
}
/**
 *
 * @param pitch
 *            in radiants
 * @param yaw
 *            in radiants
 * @param roll
 *            in radiants
 * @return
 */
void Math::fromEuler(const double pitch, const double yaw, const double roll, double *quat) {
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one
	// for roll
	// and multiply those together.
	// the calculation below does the same, just shorter

	double p = (pitch / 2.0);
	double y = (yaw / 2.0);
	double r = (roll / 2.0);

	double sinp = sin(p);
	double siny = sin(y);
	double sinr = sin(r);
	double cosp = cos(p);
	double cosy = cos(y);
	double cosr = cos(r);

	quat[0] = cosr * cosp * cosy + sinr * sinp * siny;
	quat[1] = sinr * cosp * cosy - cosr * sinp * siny;
	quat[2] = cosr * sinp * cosy + sinr * cosp * siny;
	quat[3] = cosr * cosp * siny - sinr * sinp * cosy;
	normalize(quat);
}
