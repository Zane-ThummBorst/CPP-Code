#include "sum.h"

/**
 * TODO:
 * Takes in an array of integers and its length.
 * Returns the sum of integers in the array.
 */
int sum_array(int *array, const int length) {
	int s = 0;
	for( int i = 0; i < length; i++){
		s = s + array[i];
	}
	return s;
}
