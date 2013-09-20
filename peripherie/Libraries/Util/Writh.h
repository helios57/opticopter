/*
 * Writh.h
 *
 *  Created on: Aug 20, 2013
 *      Author: helios
 */

#ifndef WRITH_H_
#define WRITH_H_

#include <stdint.h>
/*
 * Algorithm from N. Wirth's book, implementation by N. Devillard.
 * This code in public domain.
 */
#define ELEM_SWAP(a,b) { uint32_t t=(a);(a)=(b);(b)=t; }

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
uint32_t kth_smallest(uint32_t a[], uint32_t n, uint32_t k) {
	uint32_t i, j, l, m;
	uint32_t x;
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
				ELEM_SWAP(a[i], a[j]);
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
#define median(a,n) kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)))

#endif /* WRITH_H_ */
