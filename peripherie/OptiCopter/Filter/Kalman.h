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
	double p; //estimation error covariance
	double x; //value
	double k; //kalman gain
public:
	void init(double i_q, double i_r, double i_p, double intial_value) {
		q = i_q;
		r = i_r;
		p = i_p;
		x = intial_value;
	}
	double kalman_update(double measurement);
	Kalman() {
		q = r = p = x = k = 0;
	}
	virtual ~Kalman() {
	}
}
;

#endif /* KALMAN_H_ */
