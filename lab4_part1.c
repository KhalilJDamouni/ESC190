#include "lab4.h"
#include <math.h>


INT_HASH trivial_hash(INT_SIN SIN, INT_HASH num_buckets)
{

	return SIN % num_buckets;
}


INT_HASH pearson_hash(INT_SIN SIN, INT_HASH num_buckets) 
{
	INT_HASH h = 0;

	INT_SIN temp = SIN;
	int size;
	for(size = 0; temp != 0; ++size)
	{	
		temp /= 10;
	}

	INT_HASH digits[size];
	temp = SIN;
	for(int i = 0; i < size; ++i)
	{
		digits[i] = temp % 10 + 48;
		temp /= 10;
	}
	

	INT_HASH d_ascii = 0;

	for(int i = 0; i < size; ++i)
	{
		d_ascii = d_ascii ^ digits[i];
		d_ascii = PEARSON_LOOKUP[d_ascii];
	}

	return d_ascii % num_buckets;

}


INT_HASH fibonacci_hash(INT_SIN SIN, INT_HASH num_buckets) {
	
	INT_HASH value;

	unsigned long long int a = round(W / PHI);

	value = (a * SIN % W) / (W / num_buckets);

	return value;
}
