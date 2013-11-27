/*
 * Writh.h
 *
 *  Created on: Aug 20, 2013
 *      Author: helios
 */

#ifndef WRITH_H_
#define WRITH_H_

#include <stdint.h>

class WirthMedianSInt16 {
public:
	/*
	 * Algorithm from N. Wirth's book, implementation by N. Devillard.
	 * This code in public domain.
	 */
#define ELEM_SWAPS(a,b) { int16_t t=(a);(a)=(b);(b)=t; }

	/*---------------------------------------------------------------------------
	 Function : kth_smallest()
	 In : array of elements, # of elements in the array, rank k
	 Out : one element
	 Job : find the kth smallest element in the array
	 Notice : use the median() macro defined below to get the median.
	 Reference:
	 Author: Wirth, Niklaus
	 Title: Algorithms + data structures = programs
	 Publisher: Englewood Cliffs: Prentice-Hall, 1976
	 Physical description: 366 p.
	 Series: Prentice-Hall Series in Automatic Computation
	 ---------------------------------------------------------------------------*/
	static int16_t kth_smallest(int16_t a[], int16_t n, int16_t k) {
		int16_t i, j, l, m;
		int16_t x;
		l = 0;
		m = n - 1;
		while (l < m) {
			x = a[k];
			i = l;
			j = m;
			do {
				while (a[i] < x)
					i++;
				while (x < a[j])
					j--;
				if (i <= j) {
					ELEM_SWAPS(a[i], a[j]);
					i++;
					j--;
				}
			} while (i <= j);
			if (j < k)
				l = i;
			if (k < i)
				m = j;
		}
		return a[k];
	}
};
#endif /* WRITH_H_ */
