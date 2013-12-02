/*
 * Filter.h
 *
 *  Created on: Nov 26, 2013
 *      Author: helios
 */

#ifndef FILTER_H_
#define FILTER_H_

namespace opticopter {
	class Filter {
	private:
		float integratedDiff;
		float value;
	public:
		float update(float rate, float measurement, float dt, bool active);
		Filter() {
			value = 0;
			integratedDiff = 0;
		}
		virtual ~Filter() {
		}
	};
}

#endif /* FILTER_H_ */
