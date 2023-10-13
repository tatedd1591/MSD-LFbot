%% ------------------------------------------------------------------- %%
%%  Mo phong Robot do line - Do an thiet ke He thong Co dien tu ME4071 %%
%%                     Truong Dai hoc Bach Khoa                        %%
%%                           Khoa Co khi                               %%
%%                           ---***---                                 %%
%%               MO PHONG ROBOT DO LINE                                %%
%|               GVHD: Nguyen Tan Tien - Duong Van Tu                  |%
%|                                                                     |%
%|               SVTH: Huynh Nguyen Phong - 1914622                    |%
%|               Gmail: phong.huynhnguyen2401@hcmut.edu.vn             |%
%% ------------------------------------------------------------------- %%
%% Click Run to RUN %%
clc
clear all
close all
%% Khai bao cac bien co ban
global PWMLe PWMRi vInput wInput d xM yM phM r vR

PWMLe=0;                        % %PWM dc 2.
PWMRi=0;                        % %PWM dc 1.
vR = 400;                       % Toc do tham chieu can ðat.

dt = 0.1;                       % Thoi gian lay mau PID bam line.
dtpid = 0.01;                   % Thoi gian lay mau PID dong co.
t=0;
                
vatcan=1;                       % Gia ðinh vi tri vat can.
errorcb = 2.09406;              % Do lech chuan sai so do cam bien gay ra.
% errorcb =0;          
%% Ve Map
[xR, yR, phR, wR] = Map(vR, dtpid,vatcan);
%% Thong so co ban cua robot
b = 195;                        % Khoang cach 2 banh xe.
d = 180;                        % Khoang cach tu cam bien den truc dong co.
r = 85/2;                       % Ban kinh banh xe.
%% Vi tri bat dau
xM(1) = d;
yM(1) = 0;
phM(1)=-pi;
pre_pos = [xM(1); yM(1); phM(1)];

xC(1) = xM(1)+d*cos(phM(1));
yC(1) = yM(1)+d*sin(phM(1));
phC(1)= phM(1);
%% Hinh dang robot
h1=plot(1,1, 'black');          % b1
h2=plot(1,1, 'black');          % b2
h3=plot(1,1, 'black');          % Truc noi 2 banh.
h4=plot(1,1, 'red');            % Thanh cam bien.
h5=plot(1,1, 'blue');           % Banh trai.
h6=plot(1,1, 'blue');           % Banh phai.

obit=animatedline('Color','black','LineWidth',1);
banhleft=animatedline('Color','r','LineWidth',0.5);
banhright=animatedline('Color','b','LineWidth',0.5);
%% Bo dieu khien PID bam line
pre_E2=0;
intE2=0;

%||| omegan <2*giamchan*vR/d    |||%
omegan=4.4;                     % He so giam chan PID bam line.
giamchan=1;                     % Tan so giao dong rieng PID bam line.
%||| hoi tiep: K=[k1;k2]        |||%
k1=-omegan^2/vR;          
k2=-(2*giamchan*omegan+d*k1)/vR;
%||| Bo so PID                  |||%
kpE2=-1.*k1/(1+k2*d);
kdE2=-1.*k2/(1+k2*d);
KiE2=0.1;
%% Bo dieu khien PID dong co
giamchandc=1;                   % He so giam chan PID dong co
omegandc=4/(giamchandc*0.1);    % Tan so giao dong rieng PID dong co
%||| Dong co 2 - trai           |||%
pre_nLe=0;
eLe=0;
int_eLe=0;
de_dtLe=0;

kpLe=(2*giamchandc*omegandc-32.65)/76.66;
kiLe=omegandc^2/76.66;
kdLe=0.0; 
%||| Dong co 1 - phai          |||%
pre_nRi=0;
eRi=0;
int_eRi=0;
de_dtRi=0;

kpRi=(2*giamchandc*omegandc-32.05)/73.2;
kiRi=omegandc^2/73.2;
kdRi=0.00;
i=1;
%% --------------------------------- %%
xs = linspace(xC(i) - 45*sin(phM(i)), xC(i) + 45*sin(phM(i)),2);
ys = linspace(yC(i) + 45*cos(phM(i)), yC(i) - 45*cos(phM(i)),2);
P = Giaodiem([xs;ys],[xR;yR]);
%% Mo phong bam line
% while (size(P,2) ~= 0)
while (t<30)
%% Cap nhat vi tri hinh dang robot di chuyen
delete(h1);
delete(h2);
delete(h3);
delete(h4);
delete(h5);
delete(h6);
xb1 = linspace(xC(i) - 0*sin(phM(i)-pi/2) - 45*sin(phM(i))  , xC(i) + d*sin(phM(i)-pi/2) - 45*sin(phM(i)) , 2);
yb1 = linspace(yC(i) + 0*cos(phM(i)-pi/2) + 45*cos(phM(i))  , yC(i) - d*cos(phM(i)-pi/2) + 45*cos(phM(i)) , 2);
xb2 = linspace(xC(i) - 0*sin(phM(i)-pi/2) + 45*sin(phM(i))  , xC(i) + d*sin(phM(i)-pi/2) + 45*sin(phM(i)) , 2);
yb2 = linspace(yC(i) + 0*cos(phM(i)-pi/2) - 45*cos(phM(i))  , yC(i) - d*cos(phM(i)-pi/2) - 45*cos(phM(i)) , 2);
xw  = linspace(xC(i) + d*sin(phM(i)-pi/2) - b/2*sin(phM(i)) , xC(i) + d*sin(phM(i)-pi/2) + b/2*sin(phM(i)), 2);
yw  = linspace(yC(i) - d*cos(phM(i)-pi/2) + b/2*cos(phM(i)) , yC(i) - d*cos(phM(i)-pi/2) - b/2*cos(phM(i)), 2);
xs  = linspace(xC(i) - 45*sin(phM(i))                       , xC(i) + 45*sin(phM(i)),2);
ys  = linspace(yC(i) + 45*cos(phM(i))                       , yC(i) - 45*cos(phM(i)),2);
xwleft  = linspace( xC(i) + d*sin(phM(i)-pi/2) - b/2*sin(phM(i)) - r*sin(phM(i)-pi/2), xC(i) + d*sin(phM(i)-pi/2) - b/2*sin(phM(i)) + r*sin(phM(i)-pi/2), 2);
ywleft  = linspace( yC(i) - d*cos(phM(i)-pi/2) + b/2*cos(phM(i)) + r*cos(phM(i)-pi/2), yC(i) - d*cos(phM(i)-pi/2) + b/2*cos(phM(i)) - r*cos(phM(i)-pi/2), 2);
xwright = linspace( xC(i) + d*sin(phM(i)-pi/2) + b/2*sin(phM(i)) - r*sin(phM(i)-pi/2), xC(i) + d*sin(phM(i)-pi/2) + b/2*sin(phM(i)) + r*sin(phM(i)-pi/2), 2);
ywright = linspace( yC(i) - d*cos(phM(i)-pi/2) - b/2*cos(phM(i)) + r*cos(phM(i)-pi/2), yC(i) - d*cos(phM(i)-pi/2) - b/2*cos(phM(i)) - r*cos(phM(i)-pi/2), 2);

h1=plot(xb1, yb1, 'black','LineWidth',2);       % Than b1.
h2=plot(xb2, yb2, 'black','LineWidth',2);       % Than b2.
h3=plot(xw, yw, 'black','Linewidth',2);         % Truc banh.
h4=plot(xs, ys, 'red','Linewidth',2);           % Thanh cam bien.
h5=plot(xwleft, ywleft, 'blue','Linewidth',2);  % Banh trai.
h6=plot(xwright, ywright, 'blue','Linewidth',2);% Banh phai.

addpoints(banhleft , xC(i) + d*sin(phM(i)-pi/2) - b/2*sin(phM(i)), yC(i) - d*cos(phM(i)-pi/2) + b/2*cos(phM(i)));
addpoints(banhright, xC(i) + d*sin(phM(i)-pi/2) + b/2*sin(phM(i)), yC(i) - d*cos(phM(i)-pi/2) - b/2*cos(phM(i)));
addpoints(obit, xC(i), yC(i));
drawnow;
%% Giao diem giua cam bien va tam line
P = Giaodiem([xs;ys],[xR;yR]);
if size(P,2) == 0
    break;
end
%% Tinh toan sai so
% e(1,i) =  (P(1,1) - xC(i))* cos(phM(i)) + (P(2,1) - yC(i))*sin(phM(i));                                     
e(2,i) = -(P(1,1) - xC(i))* sin(phM(i)) + (P(2,1) - yC(i))*cos(phM(i))+errorcb*randn;
% e(3,i) =   phR(i) - phM(i);
%% Tinh toan van toc goc w tu PID --> van toc dai v cua 2 banh 
eE2    = e(2,i);
intE2  = intE2+(eE2+pre_E2)*dt;
w(i)   = (kpE2*eE2 + kdE2*(eE2-pre_E2)/dt + KiE2*(intE2)); %PID
pre_E2 = e(2,i);
% Van toc 2 banh.
vLe(i) = vR - w(i)*b/2;
vRi(i) = vR + w(i)*b/2; 
% Toc do quay dong co Rad/s
wLe(i) = vLe(i)/r;
wRi(i) = vRi(i)/r;
%% PID dong co
jj=round(dt/dtpid);
if jj==0
    jj=1;
end
for j=1:(jj)
    if i>1
%% Dong co 2 - trai
    pre_eLe = eLe;
    eLe     = wLe(i)*30/pi - pre_nLe;
    int_eLe = int_eLe + (eLe)*dtpid;
    de_dtLe = (eLe-pre_eLe)/dtpid;
    PWMLe   = (kpLe*eLe + kiLe*int_eLe + kdLe*de_dtLe);
    % Gioi han PWM 90%.
    if PWMLe>90
        PWMLe=90;
    end
    [tt,y]  = ode45(@motorLe_tf, [0 dtpid], pre_nLe);
    pre_nLe = y(length(y),1);
    vLe(i)  = pre_nLe*pi/30*r;
%% Dong co 1 - phai
    pre_eRi = eRi;
    eRi     = wRi(i)*30/pi - pre_nRi;
    int_eRi = int_eRi + (eRi)*dtpid;
    de_dtRi = (eRi-pre_eRi)/dtpid;
    PWMRi   = (kpRi*eRi + kiRi*int_eRi + kdRi*de_dtRi);
    % Gioi han PWM 90%.
    if PWMRi>90
        PWMRi=90;
    end   
    [tt,y]  = ode45(@motorRi_tf, [0 dtpid], pre_nRi);
    pre_nRi = y(length(y),1);
    vRi(i)  = pre_nRi*pi/30*r;
    else 
        vLe(i)=0;
        vRi(i)=0;
    end
%% ---------------------------------------
    v(i)   = (vLe(i)+vRi(i))/2;
    vInput = v(i);
    wInput = (vRi(i)-vLe(i))/b;
%% Cap nhat vi tri robot qua phuong trinh dong hoc
    [tt,y]  = ode45(@kinematic_tf, [0 dtpid], pre_pos);
    xM(i+1) = y(length(y), 1);
    yM(i+1) = y(length(y), 2);
    phM(i+1)= y(length(y), 3);
    pre_pos = [xM(i+1); yM(i+1); phM(i+1)];
    xC(i+1) = xM(i+1) + d*cos(phM(i+1));
    yC(i+1) = yM(i+1) + d*sin(phM(i+1));
end
t(i)=(i)*dt;                    
i=i+1;                  
end

%% Ve do thi
%% Ve vet bam line - mo phong chuyen dong
xlabel('mm');

xlim([-3200,200]);
ylim([-200,2000]);

title('Mo phong bam line');
%% Do thi sai so bam line e2 va toc do goc w tu PID bam line
figure();

plot(t,e(2,:),'r','Linewidth',1);
hold on
plot(t,w,'b--','Linewidth',1);

legend('Sai so bam line', 'toc do goc w');
xlabel('time(s)');
ylabel('Sai so bam line e2 (mm)');
title('Do thi sai so bam line e2 ');
%% Van toc 2 banh
figure();

plot(t,vLe,'r','Linewidth',1);
hold on
plot(t,vRi,'b--','Linewidth',1)

legend('v left','v right ');
xlabel('time(s)');
ylabel('Van toc v (mm/s)');
title('van toc v cua 2 dong co');

ylim([-100 1000]);

%% Van toc trai
figure();

plot(t,vLe,'r','Linewidth',1);

legend('v left');
xlabel('time(s)');
ylabel('Van toc v (mm/s)');
title('van toc v cua dong co 2');

ylim([-100 1000]);
%% Van toc phai
figure();

plot(t,vRi,'b','Linewidth',1);

legend('v right ');
xlabel('time(s)');
ylabel('Van toc v (mm/s)');
title('Do thi van toc v cua dong co 1 ');

ylim([-100 1000]);
%% Ket thuc
%% Huynh Nguyen Phong %%
%% 1914622            %%