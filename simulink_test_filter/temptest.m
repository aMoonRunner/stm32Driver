clc;clear;
% zeta1=0.1;
% zeta2=0.00001;
% Ts=1/10e3;
% fn=100;
% wn=2*3.14159265*fn;
% a0=1;
% a1=2*zeta1*wn*Ts-2;
% a2=-2*zeta1*wn*Ts+wn*Ts*wn*Ts+1;
% b0=1;
% b1=2*zeta2*wn*Ts-2;
% b2=-2*zeta2*wn*Ts+wn*Ts*wn*Ts+1;

s=tf('s');
w0=2*pi*100;
Q=1.9;
g_s=(s^2+w0^2)/(s^2+(w0/Q)*s+w0^2);
format long;
g_z = c2d(g_s,1e-4,'tustin');
g_z.Numerator
g_z.Denominator



