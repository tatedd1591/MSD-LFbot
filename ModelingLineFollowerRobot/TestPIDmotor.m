%% ------------------------------------------------------------------- %%
%%  Mo phong Robot do line - Do an thiet ke He thong Co dien tu ME4071 %%
%%                     Truong Dai hoc Bach Khoa                        %%
%%                           Khoa Co khi                               %%
%%                           ---***---                                 %%
%%               KIEM TRA BO DIEU KHIEN PID DONG CO                    %%
%|               GVHD: Nguyen Tan Tien - Duong Van Tu                  |%
%|                                                                     |%
%|               SVTH: Huynh Nguyen Phong - 1914622                    |%
%|               Gmail: phong.huynhnguyen2401@hcmut.edu.vn             |%
%% ------------------------------------------------------------------- %%
%% Click Run to RUN %%
clc
clear all
close all
DatatestPIDmotor();
global PWMLe PWMRi 
PWMLe = 0;
PWMRi = 0;

t(1)        = 0;
dtpid       = 0.01;
vR          = 400;
giamchandc  = 1;
omegandc    = 4/(giamchandc*0.1);
%% PID dong co trai
pre_nLe = 0;
eLe     = 0;
int_eLe = 0;
de_dtLe = 0;
pre_eRi = 0;

kpLe    = (2*giamchandc*omegandc-32.65)/76.66;
kiLe    = omegandc^2/76.66;
kdLe 	= 0; 
%% PID dong co phai
pre_nRi = 0;
eRi     = 0;
int_eRi = 0;
de_dtRi = 0;

kpRi    = (2*giamchandc*omegandc-32.05)/73.2;
kiRi    = omegandc^2/73.2;
kdRi    = 0;
%% Mo phong
% ref=100;%500/(85/2)*(30/pi);
for j=2:360
    if      j <= 180
        ref = 80;
    elseif  j > 180
        ref = 40;
    end 
%% PID dong co trai
    pre_eLe = eLe;
    eLe     = ref - pre_nLe;
    int_eLe = int_eLe + (eLe)*dtpid;
    de_dtLe = (eLe-pre_eLe)/dtpid;
    PWMLe   = (kpLe*eLe + kiLe*int_eLe + kdLe*de_dtLe);
    if PWMLe > 90
        PWMLe  = 90;
    end
    [tt,y]  = ode45(@motorLe_tf,[0 dtpid],pre_nLe);
    pre_nLe = y(length(y),1);
    wLe(j)  = pre_nLe;
%% PID dong co phai
    pre_eRi = eRi;
    eRi     = ref - pre_nRi;
    int_eRi = int_eRi + (eRi)*dtpid;
    de_dtRi = (eRi-pre_eRi)/dtpid;
    PWMRi   = (kpRi*eRi + kiRi*int_eRi + kdRi*de_dtRi);
    if PWMRi > 90
        PWMRi  = 90;
    end   
    [tt,y]  = ode45(@motorRi_tf,[0 dtpid],pre_nRi);
    pre_nRi = y(length(y),1);
    wRi(j)  = pre_nRi;
    t(j)    = dtpid*j;
    a(j)    = PWMLe;
    b(j)    = PWMRi;

end 
subplot(1,2,1)
plot(t,wLe,'black--','Linewidth',1.5);
hold on;
plot(t,OUTPID2,'r');
legend('WLe','Thucte Wle');
xlabel('time(s)');
ylabel('RPM');
xlim([0,3.65]);
ylim([0,100]);
title('Duong dap ung ly thuyet va thuc te cua dong co 2');

subplot(1,2,2)
plot(t,wRi,'black--','Linewidth',1.5);
hold on;
plot(t,OUTPID1,'b');
legend('WRi ','Thucte WRi');
xlabel('time(s)');
ylabel('RPM');
xlim([0,3.65]);
ylim([0,100]);
title('Duong dap ung ly thuyet va thuc te cua dong co 1');