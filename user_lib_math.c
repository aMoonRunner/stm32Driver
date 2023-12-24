#include "user_lib_math.h"
#include "stdint.h"
#include "math.h"

float USER_LIB_MATH_float_abs(float a)
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
// 浮点数正弦函数

static const int32_t SINTABLE_LENGTH = 1000;
static const float PI = 3.14159265f;
static const float PI_inverse = 0.318909886f;
static const float two_PI = 6.2831853f;
static const float two_PI_inverse = 0.15915494f;
static float SINTABLE[SINTABLE_LENGTH] = {0};

void USER_LIB_MATH_void_sinTable_init()
{
    int32_t i = 0;
    for (i = 0; i < SINTABLE_LENGTH; i++)
    {
        SINTABLE[i] = sin(two_PI * ((float)i / (float)SINTABLE_LENGTH));
    }
}

/*查表法正弦函数*/
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
    return SINTABLE[(int32_t)(SINTABLE_LENGTH * (x * two_PI_inverse))];
}
//==================================================================