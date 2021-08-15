#include <stdlib.h>
#include <string.h>
static float fastInvSqrt(float x) {
	int i;
	float x2;
	if(sizeof(x) != sizeof(i))abort();
	if(sizeof(i) != 4) abort();
	memcpy(&i, &x, 4);
	i = 0x5F1FFFF9 - (i >> 1);
	memcpy(&x2, &i, 4);
	x2 *= 0.703952253f * (2.38924456f - x * x2 * x2);
	return x2;
}
