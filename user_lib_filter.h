#ifndef USER_LIB_FILTER_H
#define USER_LIB_FILTER_H
#include "stdint.h"

// 限幅滤波====================================================================================================================
typedef struct
{
    float amplitude;     // 限幅
    uint32_t cnt;        // 连续多次读到异常值，则认为异常值就是正常值
    uint32_t cnt_max;    // 限幅次数阈值
    float previousValue; // 上一次的值
    float valueFliterd;  // 滤波后的值
} amplitudeLimitFilter_STR;
extern amplitudeLimitFilter_STR test_amplitudeLimitFilter_str; // 测试用

extern void USER_LIB_FILTER_void_amplitudeLimitFilter_init(amplitudeLimitFilter_STR *str, float amp, uint32_t cnt_max);
/**
 * @note 认为异常值就是正常值的条件：连续cnt_max次读到异常值，注意①连续②异常值可以不相等
 * @note 滤波后的值既返回了，也记录在了结构体变量中
 */
extern float USER_LIB_FILTER_void_amplitudeLimitFilter_calcu(float value, amplitudeLimitFilter_STR *str);
// 限幅滤波====================================================================================================================

// 一阶低通滤波器====================================================================================================================
typedef struct
{
    float Ts;            // 执行周期（采样时间）
    float fc;            // 截止频率
    float alpha;         // 滤波器系数
    float previousValue; // 上一次的值
    float valueFliterd;  // 滤波后的值
} FirstOrderLowpssFilter_STR;
extern FirstOrderLowpssFilter_STR test_firstOrderLowpssFilter_str;
extern void USER_LIB_FILTER_void_firstOrderLowpssFilter_init(FirstOrderLowpssFilter_STR *str, float fc, float Ts);
extern float USER_LIB_FILTER_float_firstOrderLowpssFilter_calcu(float value, FirstOrderLowpssFilter_STR *str);
// 一阶低通滤波器====================================================================================================================

// 陷波滤波器====================================================================================================================
/**
 * 陷波滤波器（Notch Filter）的离散化设计
 * https://blog.csdn.net/u013581448/article/details/116743786
 */
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
