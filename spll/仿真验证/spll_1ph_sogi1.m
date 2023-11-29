clear;clc;clf;
%SOGI生成正交波形,park变换到dq轴，实现相位跟踪
%输入va

k = 1.414;
f0 = 35; %电网频率
w0 = 2 * pi * f0;
deltaT = 1/10e3;

Vmax = 6; %电网电压幅值
phase = 3 * pi / 2;

t = 0:deltaT:0.9;
t = t';

wout = zeros(size(t));
va = zeros(size(t));
v = zeros(size(t));
qv = zeros(size(t));
vd = zeros(size(t));
vq = zeros(size(t));
theta = zeros(size(t));
vf = zeros(size(t));
angle = zeros(size(t));
Vout = zeros(size(t));

err = zeros(size(t));
fo = zeros(size(t));

wout(1) = 0;
va(1) = Vmax * sin(w0 * t(1) + phase);
v(1) = 0;
qv(1) = 0;
vd(1) = 0;
vq(1) = 0;
theta(1) = 0;

kp = 14.9235;
ki = 0.05;
err(1) = 0; %误差
vf(1) = 0; %PI输出值

angle(1) = 0; %输出的电压角度
Vout(1) = 0; %输出的电压值

fo(1) = 0;

for i = 3:length(t)
    %输入信号
    if i == 6e3
        phase = pi / 2;
    end

    if i == 3e3
        f0 = 50;
        w0 = 2 * pi * f0;
    end

    va(i) = Vmax * sin(w0 * t(i) + phase);

    %sogi正交信号生成
    v(i) = -deltaT * wout(i - 1) * qv(i - 1) + deltaT * k * wout(i - 1) * va(i) + (1 - deltaT * k * wout(i - 1)) * v(i - 1);
    qv(i) = deltaT * wout(i - 1) * v(i - 1) + qv(i - 1);

    %park变换
    vq(i) = (-1) * sin(theta(i - 1)) * v(i) + cos(theta(i - 1)) * qv(i);
    vd(i) = cos(theta(i - 1)) * v(i) + sin(theta(i - 1)) * qv(i);

    %pi环路
    err(i) = vq(i) - 0;
    vf(i) = vf(i - 1) + kp * (err(i) - err(i - 1)) + ki * err(i);

    if vf(i) > 50 * pi
        vf(i) = 50 * pi;
    end

    if vf(i) <- 20 * pi
        vf(i) = -20 * pi;
    end

    wout(i) = vf(i) + w0;

    %输出

    theta(i) = theta(i - 1) + deltaT * wout(i);

    angle(i) = mod(theta(i) + pi / 2, 2 * pi);
    Vout(i) = Vmax * cos(theta(i));

end

subplot(4, 1, 1);
plot(t, va, 'g', t, v, 'b', t, qv, 'r');
subplot(4, 1, 2);
plot(t, va, 'g', t, angle, 'r');
subplot(4, 1, 3);
plot(t, va, 'g', t, Vout, 'r');
subplot(4, 1, 4);
plot(t, vf / 10, 'b', t, vq, 'r', t, vd, 'g', t, fo / 10, 'm');
