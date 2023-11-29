clc; clear;
%% 基于二阶广义积分器的单相锁相环

%% 初始化
u(1) = 0;
u(2) = 0;
u(3) = 0;

osg_u(1) = 0;
osg_u(2) = 0;
osg_u(3) = 0;

osg_qu(1) = 0;
osg_qu(2) = 0;
osg_qu(3) = 0;

u_Q(1) = 0;
u_Q(2) = 0;

u_D(1) = 0;
u_D(2) = 0;

ylf(1) = 0;
ylf(2) = 0;

fo = 0;

theta = 0;

sine = 0;
cosine = 0;

%% 配置
fn = 50;
delta_t = 1/10e3;
lpf_coeff.b0 = 222.2862;
lpf_coeff.b1 = -222.034;

%%计算中间变量

wn = fn * 2 * pi;
osg_k = 0.5;
osg_x = 2 * osg_k * wn * delta_t;
osg_y = (wn * delta_t) ^ 2;
osg_b0 = (osg_x) / (osg_x + osg_y + 4);
osg_b2 = -osg_b0;
osg_a1 = (2 * (4 - osg_y)) / (osg_x + osg_y + 4);
osg_a2 = (osg_x - osg_y - 4) / (osg_x + osg_y + 4);
osg_qb0 = osg_k * osg_y / (osg_x + osg_y + 4);
osg_qb1 = osg_qb0 * 2;
osg_qb2 = osg_qb0;



t=0:delta_t:0.5;


%% 锁相过程
osg_u(1) = (osg_b0 * u(1)) + (osg_b2 * u(3)) + (osg_a1 * osg_u(2)) + (osg_a2 * osg_u(3));
osg_u(3) = osg_u(2);
osg_u(2) = osg_u(1);

osg_qu(1) = (osg_qb0 * u(1)) + (osg_qb1 * u(2)) + (osg_qb2 * u(3)) + (osg_a1 * osg_qu(2)) + (osg_a2 * osg_qu(3));
osg_qu(3) = osg_qu(2);
osg_qu(2) = osg_qu(1);

u(3) = u(2);
u(2) = u(1);

u_Q(1) = (cosine * osg_u(1)) + (sine * osg_qu(1));
u_D(1) = (cosine * osg_qu(1)) - (sine * osg_u(1));

ylf(1) = ylf(2) + (lpf_coeff.b0 * u_Q(1)) + (lpf_coeff.b1 * u_Q(2));
ylf(2) = ylf(1);

u_Q(2) = u_Q(1);

fo = fn + ylf(1);

theta = theta + (fo * delta_t) * 2 * pi;

if (theta > 2 * pi)
    theta = theta - 2 * pi;
end
