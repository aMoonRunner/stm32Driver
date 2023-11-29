/*===================================================
作者: jhshe
版本: 0.0
邮箱: jh_she@qq.com
创建时间: 2023/11/29 星期三
说明：基于二阶广义积分器的单相锁相环

参考《面向单相并网逆变器、采用 C2000 MCU 的软件 PLL 设计》应用报告
https://www.ti.com/lit/an/sprabt3a/sprabt3a.pdf?ts=1701240887359

使用说明：先SPLL_1PH_SOGI_reset初始化，再SPLL_1PH_SOGI_config配置系数，最后SPLL_1PH_SOGI_run调用即可
===================================================*/

/* Includes ------------------------------------------------------------------*/
#include "spll_1ph_sogi.h"
#include "app_sinTable.h"

/* 变量 ------------------------------------------------------------------*/
SPLL_1PH_SOGI TTPLPFC_spll1;

/* 函数 ------------------------------------------------------------------*/

/*自定义正弦函数*/
static inline float spll_1ph_sogi_sin(float x)
{
    return app_sineTable_getsinValue(x);
}
/*自定义余弦函数*/
static inline float spll_1ph_sogi_cos(float x)
{
    return app_sineTable_getcosValue(x);
}

void SPLL_1PH_SOGI_reset(SPLL_1PH_SOGI *spll_obj)
{
    spll_obj->u[0] = (float)(0.0);
    spll_obj->u[1] = (float)(0.0);
    spll_obj->u[2] = (float)(0.0);

    spll_obj->osg_u[0] = (float)(0.0);
    spll_obj->osg_u[1] = (float)(0.0);
    spll_obj->osg_u[2] = (float)(0.0);

    spll_obj->osg_qu[0] = (float)(0.0);
    spll_obj->osg_qu[1] = (float)(0.0);
    spll_obj->osg_qu[2] = (float)(0.0);

    spll_obj->u_Q[0] = (float)(0.0);
    spll_obj->u_Q[1] = (float)(0.0);

    spll_obj->u_D[0] = (float)(0.0);
    spll_obj->u_D[1] = (float)(0.0);

    spll_obj->ylf[0] = (float)(0.0);
    spll_obj->ylf[1] = (float)(0.0);

    spll_obj->fo = (float)(0.0);

    spll_obj->theta = (float)(0.0);

    spll_obj->sine = (float)(0.0);
    spll_obj->cosine = (float)(0.0);
}

void SPLL_1PH_SOGI_coeff_calc(SPLL_1PH_SOGI *spll_obj)
{
    float osgx, osgy, temp, wn;
    wn = spll_obj->fn * (float)2.0f * (float)3.14159265f;
    spll_obj->osg_coeff.osg_k = (float)(0.5);
    osgx = (float)(2.0f * 0.5f * wn * spll_obj->delta_t);
    spll_obj->osg_coeff.osg_x = (float)(osgx);
    osgy = (float)(wn * spll_obj->delta_t * wn * spll_obj->delta_t);
    spll_obj->osg_coeff.osg_y = (float)(osgy);
    temp = (float)1.0 / (osgx + osgy + 4.0f);
    spll_obj->osg_coeff.osg_b0 = ((float)osgx * temp);
    spll_obj->osg_coeff.osg_b2 = ((float)(-1.0f) * spll_obj->osg_coeff.osg_b0);
    spll_obj->osg_coeff.osg_a1 = ((float)(2.0 * (4.0f - osgy)) * temp);
    spll_obj->osg_coeff.osg_a2 = ((float)(osgx - osgy - 4) * temp);
    spll_obj->osg_coeff.osg_qb0 = ((float)(0.5f * osgy) * temp);
    spll_obj->osg_coeff.osg_qb1 = (spll_obj->osg_coeff.osg_qb0 * (float)(2.0));
    spll_obj->osg_coeff.osg_qb2 = spll_obj->osg_coeff.osg_qb0;
}

void SPLL_1PH_SOGI_config(SPLL_1PH_SOGI *spll_obj, float acFreq, float isrFrequency, float lpf_b0, float lpf_b1)
{
    spll_obj->fn = acFreq;
    spll_obj->delta_t = ((1.0f) / isrFrequency);

    SPLL_1PH_SOGI_coeff_calc(spll_obj);

    spll_obj->lpf_coeff.b0 = lpf_b0;
    spll_obj->lpf_coeff.b1 = lpf_b1;
}

void SPLL_1PH_SOGI_run(SPLL_1PH_SOGI *spll_obj, float acValue)
{
    // Update the spll_obj->u[0] with the grid value
    spll_obj->u[0] = acValue;

    //
    // Orthogonal Signal Generator
    //
    spll_obj->osg_u[0] = (spll_obj->osg_coeff.osg_b0 * spll_obj->u[0]) +
                         (spll_obj->osg_coeff.osg_b2 * spll_obj->u[2]) +
                         (spll_obj->osg_coeff.osg_a1 * spll_obj->osg_u[1]) +
                         (spll_obj->osg_coeff.osg_a2 * spll_obj->osg_u[2]);

    spll_obj->osg_u[2] = spll_obj->osg_u[1];
    spll_obj->osg_u[1] = spll_obj->osg_u[0];

    spll_obj->osg_qu[0] = (spll_obj->osg_coeff.osg_qb0 * spll_obj->u[0]) +
                          (spll_obj->osg_coeff.osg_qb1 * spll_obj->u[1]) +
                          (spll_obj->osg_coeff.osg_qb2 * spll_obj->u[2]) +
                          (spll_obj->osg_coeff.osg_a1 * spll_obj->osg_qu[1]) +
                          (spll_obj->osg_coeff.osg_a2 * spll_obj->osg_qu[2]);

    spll_obj->osg_qu[2] = spll_obj->osg_qu[1];
    spll_obj->osg_qu[1] = spll_obj->osg_qu[0];

    spll_obj->u[2] = spll_obj->u[1];
    spll_obj->u[1] = spll_obj->u[0];

    //
    // Park Transform from alpha beta to d-q axis
    //
    spll_obj->u_Q[0] = (spll_obj->cosine * spll_obj->osg_u[0]) + (spll_obj->sine * spll_obj->osg_qu[0]);
    spll_obj->u_D[0] = (spll_obj->cosine * spll_obj->osg_qu[0]) - (spll_obj->sine * spll_obj->osg_u[0]);

    //
    // Loop Filter
    //
    spll_obj->ylf[0] = spll_obj->ylf[1] + (spll_obj->lpf_coeff.b0 * spll_obj->u_Q[0]) + (spll_obj->lpf_coeff.b1 * spll_obj->u_Q[1]);
    spll_obj->ylf[1] = spll_obj->ylf[0];

    spll_obj->u_Q[1] = spll_obj->u_Q[0];

    //
    // VCO
    //
    spll_obj->fo = spll_obj->fn + spll_obj->ylf[0];

    spll_obj->theta = spll_obj->theta + (spll_obj->fo * spll_obj->delta_t) * (float)(2.0 * 3.14159265f);

    if (spll_obj->theta > (float)(2.0 * 3.14159265f))
    {
        spll_obj->theta = spll_obj->theta - (float)(2.0 * 3.14159265f);
    }

    spll_obj->sine = spll_1ph_sogi_sin(spll_obj->theta);
    spll_obj->cosine = spll_1ph_sogi_cos(spll_obj->theta);
}
