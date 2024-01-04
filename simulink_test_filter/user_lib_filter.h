
#ifndef USER_LIB_FILTER_H
#define USER_LIB_FILTER_H
#include "stdint.h"

// 限幅滤波====================================================================================================================
typedef struct
{
    float amplitude; // 限幅
    uint32_t cnt;    // 连续多次读到异常值，则认为此异常值就是正常值
    uint32_t cnt_max;
    float previousValue;
    float valueFliterd;
} amplitudeLimitFilter_STR;
extern amplitudeLimitFilter_STR test_amplitudeLimitFilter_str;
extern void USER_LIB_FILTER_void_amplitudeLimitFilter_init(amplitudeLimitFilter_STR *str, float amp, uint32_t cnt);
extern float USER_LIB_FILTER_void_amplitudeLimitFilter_calcu(float value, amplitudeLimitFilter_STR *str);
// 限幅滤波====================================================================================================================

// 一阶低通滤波器====================================================================================================================
typedef struct
{
    float Ts;    // 执行周期
    float fc;    // 截止频率
    float alpha; // 滤波器系数
    float previousValue;
    float valueFliterd;
} FirstOrderLowpssFilter_STR;
extern FirstOrderLowpssFilter_STR test_firstOrderLowpssFilter_str;
extern void USER_LIB_FILTER_void_firstOrderLowpssFilter_init(FirstOrderLowpssFilter_STR *str, float fc, float Ts);
extern float USER_LIB_FILTER_float_firstOrderLowpssFilter_calcu(float value, FirstOrderLowpssFilter_STR *str);
// 一阶低通滤波器====================================================================================================================

// 陷波滤波器====================================================================================================================
typedef struct
{
    float valueFliterd;
    float u[3];
    float y[3];
    float Ts;
    float fc;
    float wc;
    float Q;
    float b0;
    float b1;
    float b2;
    float a0;
    float a1;
    float a2;
} notchFilter_STR;
extern notchFilter_STR test_notchFilter_str;
extern void USER_LIB_FILTER_void_notchFilter_init(notchFilter_STR *str, float Q, float fc, float Ts);
extern float USER_LIB_FILTER_float_notchFilter_calcu(float value, notchFilter_STR *str);
// 陷波滤波器====================================================================================================================

#endif
