#include "stdint.h"
#include "user_lib_filter.h"

// 避免和其它文件冲突，用static修饰
static const float PI = 3.14159265f;

inline void USER_LIB_FILTER_void_firstOrderLowpssFilter_init(FirstOrderLowpssFilter_STR *str, float fc, float Ts)
{
    str->fc = fc;
    str->Ts = Ts;
    float temp = 0;
    temp = 2.0f * PI * fc * Ts;
    str->alpha = temp / (1.0f + temp);
    str->previousValue = 0.0f;
    str->previousValueState = 0;
    str->valueFliterd = 0.0f;
}

inline float USER_LIB_FILTER_float_firstOrderLowpssFilter_calcu(float value, FirstOrderLowpssFilter_STR *str)
{
    static const int32_t PREPARED = 1;
    static const int32_t NOT_PREPARED = 0;

    if (str->previousValueState == NOT_PREPARED)
    {
        str->valueFliterd = value;
        str->previousValue = value;
        str->previousValueState = PREPARED;
    }
    else
    {
        str->valueFliterd = str->alpha * value + (1.0f - str->alpha) * str->previousValue;
        str->previousValue = str->valueFliterd;
    }
    return str->valueFliterd;
}
