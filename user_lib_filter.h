
#ifndef USER_LIB_FILTER_H
#define USER_LIB_FILTER_H
#include "stdint.h"

typedef struct
{
    float Ts;    // 执行周期
    float fc;    // 截止频率
    float alpha; // 滤波器系数
    float previousValue;
    // 要先积累一定的数据才能开始滤波
    // 这里虽然只用记录上一次的数据
    int32_t previousValueState;
    float valueFliterd;
} FirstOrderLowpssFilter_STR;

extern void USER_LIB_FILTER_void_firstOrderLowpssFilter_init(FirstOrderLowpssFilter_STR *str, float fc, float Ts);
extern float USER_LIB_FILTER_float_firstOrderLowpssFilter_init(float value, FirstOrderLowpssFilter_STR *str);

#endif
