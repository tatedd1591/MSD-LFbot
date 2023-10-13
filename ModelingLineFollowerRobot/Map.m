%% ------------------------------------------------------------------- %%
%%  Mo phong Robot do line - Do an thiet ke He thong Co dien tu ME4071 %%
%%                     Truong Dai hoc Bach Khoa                        %%
%%                           Khoa Co khi                               %%
%%                           ---***---                                 %%
%%               VE SA BAN BAM LINE                                    %%
%|               GVHD: Nguyen Tan Tien - Duong Van Tu                  |%
%|                                                                     |%
%|               SVTH: Huynh Nguyen Phong - 1914622                    |%
%|               Gmail: phong.huynhnguyen2401@hcmut.edu.vn             |%
%% ------------------------------------------------------------------- %%
%% Do not use to RUN %%
function [xR yR phR wR] = Map(vR,dt,vatcan)
%% Toa do cac diem
rR1 = 500;
rR2 = 800;
x1  = 0             ; y1  = 0;
x2  = -2500         ; y2  = 0;
x3  = -2500         ; y3  = 1000;
x4  = x3 + 168.629  ; y4  = y3;
%% Nhanh trai
x5s = x4  + 565.685 ; y5s = y4  + 234.315;
x5  = x5s + 31.371  ; y5  = y5s + 31.371;
x6  = x5  + 565.685 ; y6  = y5  + 234.314;
x7  = 0             ; y7  = y6;
%% Nhanh phai
x8s = x4  + 565.685 ; y8s = y4  - 234.315;
x8  = x8s + 31.371  ; y8  = y8s - 31.371;
x9  = x8  + 565.685 ; y9  = y8  - 234.314;
x10 = 0             ; y10 = y9;
%% Vat can
xcan(1) = 0;
ycan(1) = 0;
phcan(1)= 0;
%% Thiet lap cac doan line
x = [x1 x2 x3 x4 x5s x5 x6 x7 x8s x8 x9 x10];
y = [y1 y2 y3 y4 y5s y5 y6 y7 y8s y8 y9 y10];
plot(x,y,'.')
hold on
axis equal;
axis([-3200 200 -200 1700])

%% 1 --> 2
xR1(1)  = x1;
yR1(1)  = y1;
phR1(1) = -pi;
wR1(1)  = 0;
L12     = 2500;
k12     = ceil(L12/(vR*dt));
for i = 2:k12
    xR1(i)  = xR1(i-1) - vR*dt;
    yR1(i)  = yR1(i-1);
    phR1(i) = -pi;
    wR1(i)  = 0;
end
%% 2 --> 3
xR2(1)  = x2;
yR2(1)  = y2;
phR2(1) = -pi;
wR2(1)  = -vR/rR1;
L23     = pi*rR1;
k23     = ceil(L23/(vR*dt));
for i = 2:k23
    phR2(i) = phR2(i-1) + wR2(i-1)*dt;
    xR2(i)  = x2 + rR1*cos(phR2(i) + pi/2);
    yR2(i)  = y2 + 500 + rR1*sin(phR2(i) + pi/2);
    wR2(i)  = -vR/rR1;
end
%% 3 --> 4
xR3(1)  = x3;
yR3(1)  = y3;
phR3(1) = 0;
wR3(1)  = 0;
L34     = abs(x4-x3);
k34     = ceil(L34/(vR*dt));
for i = 2:k34
    xR3(i)  = xR3(i-1) + vR*dt;
    yR3(i)  = yR3(i-1);
    phR3(i) = 0;
    wR3(i)  = 0;
end
%% 4 --> 5s --> 5
xR4(1)  = x4;
yR4(1)  = y4;
phR4(1) = 0;
wR4(1)  = vR/rR2;
L45s    = 2*pi*rR2/8;
L55s    =  sqrt((x5-x5s)^2 + (y5-y5s)^2);
k45s    = ceil(L45s/(vR*dt));
k55s    = ceil(L55s/(vR*dt));
for i = 2:k45s + k55s
    if i <= k45s
        phR4(i) = phR4(i-1) + wR4(1)*dt;
        xR4(i)  = x4 + rR2*cos(phR4(i)-pi/2);
        yR4(i)  = y4 + 800 + rR2*sin(phR4(i)-pi/2);
        wR4(i)  = vR/rR2;
    else
        phR4(i) = pi/4;
        xR4(i)  = xR4(i-1) + vR*dt*cos(phR4(i));
        yR4(i)  = yR4(i-1) + vR*dt*sin(phR4(i));
        wR4(i)  = 0;
    end
end
%% 5 --> 6
xR5(1)  = x5;
yR5(1)  = y5;
phR5(1) = pi/4;
wR5(1)  = vR/rR2;
L56     = 2*pi*rR2/8;
k56     = ceil(L56/(vR*dt));
for i = 2:k56
    phR5(i) = phR5(i-1) - wR5(1)*dt;
    xR5(i)  = x6 + rR2*cos(phR5(i)+pi/2);
    yR5(i)  = y6 - 800 + rR2*sin(phR5(i)+pi/2);
    wR5(i)  = vR/rR2;
end
%% 6 --> 7
xR6(1)  = x6;
yR6(1)  = y6;
phR6(1) = 0;
wR6(1)  = 0;
L67     = abs(x6-x7);
k67     = ceil(L67/(vR*dt));
for i = 2:k67
    xR6(i)  = xR6(i-1) + vR*dt;
    yR6(i)  = yR6(i-1);
    phR6(i) = 0;
    wR6(i)  = 0;
end
%% 4 --> 8s --> 8
xR7(1)  = x4;
yR7(1)  = y4;
phR7(1) = 0;
wR7(1)  = vR/rR2;
L48s    = 2*pi*rR2/8;
L88s    = sqrt((x8-x8s)^2 + (y8-y8s)^2);
k48s    = ceil(L48s/(vR*dt));
k88s    = ceil(L88s/(vR*dt));
for i = 2:k48s + k88s
    if i <= k48s
        phR7(i) = phR7(i-1) - wR7(1)*dt;
        xR7(i)  = x4 + rR2*cos(phR7(i)+pi/2);
        yR7(i)  = y4 - 800 + rR2*sin(phR7(i)+pi/2);
        wR7(i)  = vR/rR2;
    else
        phR7(i) = -pi/4;
        xR7(i)  = xR7(i-1) + vR*dt*cos(phR7(i));
        yR7(i)  = yR7(i-1) + vR*dt*sin(phR7(i));
        wR7(i)  = 0;
    end
end
%% 8 --> 9
xR8(1)  = x8;
yR8(1)  = y8;
phR8(1) = -pi/4;
wR8(1)  = vR/rR2;
L89     = 2*pi*rR2/8;
k89     = ceil(L89/(vR*dt));
for i = 2:k89
    phR8(i) = phR8(i-1) + wR8(1)*dt;
    xR8(i)  = x9 + rR2*cos(phR8(i)-pi/2);
    yR8(i)  = y9 + 800 + rR2*sin(phR8(i)-pi/2);
    wR8(i)  = vR/rR2;
end
%% 9 --> 10
xR9(1)  = x9;
yR9(1)  = y9;
phR9(1) = 0;
wR9(1)  = 0;
L910    = abs(x9-x10);
k910    = ceil(L910/(vR*dt));
for i = 2:k910
    xR9(i)  = xR9(i-1)+vR*dt;
    yR9(i)  = yR9(i-1);
    phR9(i) = 0;
    wR9(i)  = 0;
end
%% Vat can
for i = 1:36
    if      vatcan == 1
        phcan(i) = i*10;
        xcan(i)  = x6 + 25*cos(phcan(i));
        ycan(i)  = y6 + 25*sin(phcan(i));
    elseif  vatcan == 2
        phcan(i) =i;
        xcan(i)  = x9 + 25*cos(phcan(i));
        ycan(i)  = y9 + 25*sin(phcan(i));
    end
end
%% Ve map - xuat du lieu map tim giao diem
x  = [xR1, xR2, xR3];
y  = [yR1, yR2, yR3];
plot(x,y,'c','LineWidth',2.6);
x1 = [xR4, xR5, xR6];
y1 = [yR4, yR5, yR6];
plot(x1,y1,'c','LineWidth',2.6);
x2 = [xR7, xR8, xR9];
y2 = [yR7, yR8, yR9];
plot(x2,y2,'c','LineWidth',2.6);
plot(xcan,ycan,'black','LineWidth',2.6);
if      vatcan == 2 
    xR  = [xR1 , xR2 , xR3 , xR4 , xR5 , xR6 ];
    yR  = [yR1 , yR2 , yR3 , yR4 , yR5 , yR6 ];
    phR = [phR1, phR2, phR3, phR4, phR5, phR6];
    wR  = [wR1 , wR2 , wR3 , wR4 , wR5 , wR6 ];
elseif  vatcan == 1
    xR  = [xR1 , xR2 , xR3 , xR7 , xR8 , xR9 ];
    yR  = [yR1 , yR2 , yR3 , yR7 , yR8 , yR9 ];
    phR = [phR1, phR2, phR3, phR7, phR8, phR9];
    wR  = [wR1 , wR2 , wR3 , wR7 , wR8 , wR9 ];
end
end