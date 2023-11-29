/*===================================================
作者: jhshe
版本: 0.0
邮箱: jh_she@qq.com
创建时间: 2023/11/29 星期三
说明：基于二阶广义积分器的单相锁相环头文件
===================================================*/

#ifndef SPLL_1PH_SOGI_H
#define SPLL_1PH_SOGI_H

/*正交信号发生器相关系数结构体*/
typedef struct
{
    float osg_k;
    float osg_x;
    float osg_y;
    float osg_b0;
    float osg_b2;
    float osg_a1;
    float osg_a2;
    float osg_qb0;
    float osg_qb1;
    float osg_qb2;
} SPLL_1PH_SOGI_OSG_COEFF;

/*低通滤波器系数结构体*/
typedef struct
{
    float b1;
    float b0;
} SPLL_1PH_SOGI_LPF_COEFF;

/*基于二阶广义积分器的单相锁相环结构体*/
typedef struct
{
    float u[3];                        //!< AC input data buffer
    float osg_u[3];                    //!< Orthogonal signal generator data buffer
    float osg_qu[3];                   //!< Orthogonal signal generator quadrature data buffer
    float u_Q[2];                      //!< Q-axis component
    float u_D[2];                      //!< D-axis component
    float ylf[2];                      //!< Loop filter data storage
    float fo;                          //!< Output frequency of PLL(Hz)
    float fn;                          //!< Nominal frequency (Hz)
    float theta;                       //!< Angle output (0-2*pi)
    float cosine;                      //!< Cosine value of the PLL angle
    float sine;                        //!< Sine value of the PLL angle
    float delta_t;                     //!< Inverse of the ISR rate at which module is called
    SPLL_1PH_SOGI_OSG_COEFF osg_coeff; //!< Orthogonal signal generator coefficient
    SPLL_1PH_SOGI_LPF_COEFF lpf_coeff; //!< Loop filter coeffcient structure
} SPLL_1PH_SOGI;

extern void SPLL_1PH_SOGI_reset(SPLL_1PH_SOGI *spll_obj);
extern void SPLL_1PH_SOGI_config(SPLL_1PH_SOGI *spll_obj, float acFreq, float isrFrequency, float lpf_b0, float lpf_b1);
extern void SPLL_1PH_SOGI_run(SPLL_1PH_SOGI *spll_obj, float acValue);

extern SPLL_1PH_SOGI TTPLPFC_spll1;

#endif
