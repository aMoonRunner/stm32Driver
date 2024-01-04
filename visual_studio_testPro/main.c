#include "stdio.h"
#include "user_lib_math.h"

void main()
{
	float a = -5.3f;
	float b = 0.0f;
	b = USER_LIB_MATH_float_abs(a);
	printf("abs(b)=%3.2f\n",b);
}