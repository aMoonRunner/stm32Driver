#include "user_lib_math.h"
#include "stdint.h"
#include "math.h"
//==================================================================
// 浮点数绝对值函数
inline float USER_LIB_MATH_float_abs(float a)
{
	float temp = 0;
	if (a >= 0)
	{
		temp = a;
	}
	else
	{
		temp = -a;
	}
	return temp;
}
//==================================================================

//==================================================================
// 浮点数正弦函数、余弦函数
#define USER_LIB_MATH_SINTABLE_LENGTH ((int32_t)1000)
#define USER_LIB_MATH_COSTABLE_LENGTH ((int32_t)1000)

static const float PI = 3.14159265f;
static const float PI_inverse = 0.318909886f;
static const float two_PI = 6.2831853f;
static const float two_PI_inverse = 0.15915494f;
static float SINTABLE[USER_LIB_MATH_SINTABLE_LENGTH] = {0};
static float COSTABLE[USER_LIB_MATH_COSTABLE_LENGTH] = {0};

static inline void USER_LIB_MATH_void_sinTable_init()
{
	int32_t i = 0;
	for (i = 0; i < USER_LIB_MATH_SINTABLE_LENGTH; i++)
	{
		SINTABLE[i] = sinf(two_PI * ((float)i / (float)USER_LIB_MATH_SINTABLE_LENGTH));
	}
}
static inline void USER_LIB_MATH_void_cosTable_init()
{
	int32_t i = 0;
	for (i = 0; i < USER_LIB_MATH_COSTABLE_LENGTH; i++)
	{
		COSTABLE[i] = cosf(two_PI * ((float)i / (float)USER_LIB_MATH_COSTABLE_LENGTH));
	}
}
// 查表法正弦函数
float USER_LIB_MATH_float_getsinValue(float x)
{
	while (x > two_PI)
	{
		x = x - two_PI;
	}

	while (x < 0.0f)
	{
		x = x + two_PI;
	}
	return SINTABLE[(int32_t)(USER_LIB_MATH_SINTABLE_LENGTH * (x * two_PI_inverse))];
}
// 查表法余弦函数
inline float USER_LIB_MATH_float_getcosValue(float x)
{
	while (x > two_PI)
	{
		x = x - two_PI;
	}

	while (x < 0.0f)
	{
		x = x + two_PI;
	}
	return COSTABLE[(int32_t)(USER_LIB_MATH_COSTABLE_LENGTH * (x * two_PI_inverse))];
}
//==================================================================

//==================================================================
void USER_LIB_MATH_void_init()
{
	USER_LIB_MATH_void_sinTable_init();
}
//==================================================================