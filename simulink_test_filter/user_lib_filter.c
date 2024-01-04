#include "stdint.h"
#include "user_lib_filter.h"

// 文件内部公用变量和函数====================================================================================================================
// 仅内部使用，用static修饰
static const float PI = 3.14159265f;
static const int32_t PREPARED = 1;
static const int32_t NOT_PREPARED = 0;

static inline float USER_LIB_FILTER_float_abs(float x)
{
    float temp = 0;
    if (x < 0.0f)
    {
        temp = -x;
    }
    else
    {
        temp = x;
    }
    return temp;
}
// 文件内部公用变量和函数====================================================================================================================

// 限幅滤波====================================================================================================================
amplitudeLimitFilter_STR test_amplitudeLimitFilter_str = {0};
inline void USER_LIB_FILTER_void_amplitudeLimitFilter_init(amplitudeLimitFilter_STR *str, float amp, uint32_t cnt_max)
{
    str->amplitude = amp;
    str->cnt = 0;
    str->cnt_max = cnt_max;
    str->previousValue = 0.0f;
    str->valueFliterd = 0.0f;
}
inline float USER_LIB_FILTER_void_amplitudeLimitFilter_calcu(float value, amplitudeLimitFilter_STR *str)
{
    if (USER_LIB_FILTER_float_abs(value - str->previousValue) > str->amplitude)
    {
        str->cnt++;
        str->valueFliterd = str->previousValue;
        if (str->cnt >= str->cnt_max)
        { // 连续多次读到异常值，则认为异常值就是正常值
            str->cnt = 0;
            str->valueFliterd = value;
        }
    }
    else
    {
        str->cnt = 0;
        str->valueFliterd = value;
    }
    str->previousValue = str->valueFliterd;
    return str->valueFliterd;
}
// 限幅滤波====================================================================================================================

// 一阶低通滤波器====================================================================================================================
FirstOrderLowpssFilter_STR test_firstOrderLowpssFilter_str = {0};
/**
 * @description: 一阶低通滤波器，结构体初始化
 * @param {FirstOrderLowpssFilter_STR} *str 结构体
 * @param {float} fc 截止频率
 * @param {float} Ts 采样时间
 * @return {*} 无返回值
 */
inline void USER_LIB_FILTER_void_firstOrderLowpssFilter_init(FirstOrderLowpssFilter_STR *str, float fc, float Ts)
{
    str->fc = fc;
    str->Ts = Ts;
    float temp = 0;
    temp = 2.0f * PI * fc * Ts;
    str->alpha = temp / (1.0f + temp);
    str->previousValue = 0.0f;
    str->valueFliterd = 0.0f;
}
/**
 * @description: 一阶低通滤波器
 * @param {float} value 需要滤波的数
 * @param {FirstOrderLowpssFilter_STR} *str 滤波器对应的结构体
 * @return {float} 滤波后的数
 */
inline float USER_LIB_FILTER_float_firstOrderLowpssFilter_calcu(float value, FirstOrderLowpssFilter_STR *str)
{
    str->valueFliterd = str->alpha * value + (1.0f - str->alpha) * str->previousValue;
    str->previousValue = str->valueFliterd;
    return str->valueFliterd;
}
// 一阶低通滤波器====================================================================================================================

// 陷波滤波器====================================================================================================================
/**
 * @description:
 * 参考链接
 * 常见三种陷波滤波器（Notch Filter）的离散化设计
 * https://blog.csdn.net/qczhao_10/article/details/128073104
 * 陷波滤波器（Notch Filter）的离散化设计
 * https://blog.csdn.net/u013581448/article/details/116743786
 * Software Phase Locked Loop Design Using C2000™ Microcontrollers for Single Phase Grid Connected Inverter
 * https://www.ti.com/lit/an/sprabt3a/sprabt3a.pdf
 */
notchFilter_STR test_notchFilter_str = {0};

inline void USER_LIB_FILTER_void_notchFilter_init(notchFilter_STR *str, float Q, float fc, float Ts)
{
    str->valueFliterd = 0.0f;
    str->u[0] = 0.0f;
    str->u[1] = 0.0f;
    str->u[2] = 0.0f;
    str->y[0] = 0.0f;
    str->y[1] = 0.0f;
    str->y[2] = 0.0f;

    str->Ts = Ts;
    str->fc = fc;
    str->wc = 2 * 3.1415926535897932384626 * fc;
    str->Q = Q;

    str->b0 = 4.0f + str->wc * str->wc * str->Ts * str->Ts;
    str->b1 = 2.0f * str->wc * str->wc * str->Ts * str->Ts - 8;
    str->b2 = str->b0;

    str->a0 = 4.0f + str->wc * str->wc * str->Ts * str->Ts + 2 * (str->wc / Q) * str->Ts;
    str->a1 = str->b1;
    str->a2 = 4.0f + str->wc * str->wc * str->Ts * str->Ts - 2 * (str->wc / Q) * str->Ts;

    str->a1 = str->a1 / str->a0;
    str->a2 = str->a2 / str->a0;
    str->b0 = str->b0 / str->a0;
    str->b1 = str->b1 / str->a0;
    str->b2 = str->b2 / str->a0;
    str->a0 = 1.0f;
}
inline float USER_LIB_FILTER_float_notchFilter_calcu(float value, notchFilter_STR *str)
{

    str->u[2] = str->u[1];
    str->u[1] = str->u[0];
    str->u[0] = value;

    str->y[0] = (+str->b0 * str->u[0]) +
                (+str->b1 * str->u[1]) +
                (+str->b2 * str->u[2]) +
                (-str->a1 * str->y[1]) +
                (-str->a2 * str->y[2]);

    str->y[2] = str->y[1];
    str->y[1] = str->y[0];
    str->valueFliterd = str->y[0];
    return str->valueFliterd;
}
// 陷波滤波器====================================================================================================================

/*
 *
 *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 */
