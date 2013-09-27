/*
 * Kalman.h
 *
 *  Created on: 27.09.2013
 *      Author: Helios
 */

#ifndef KALMAN_H_
#define KALMAN_H_

class Kalman {
private:
	double q; //process noise covariance
	double r; //measurement noise covariance
	double x; //value
	double p; //estimation error covariance
	double k; //kalman gain
public:
	Kalman(double q, double r, double p, double intial_value) :
			q(q), r(r), p(p), x(intial_value) {
		k = 0;
	}
	double kalman_update(double measurement);
	virtual ~Kalman() {
	}
};

#endif /* KALMAN_H_ */
