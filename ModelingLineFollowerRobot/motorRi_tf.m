%% ------------------------------------------------------------------- %%
%%  Mo phong Robot do line - Do an thiet ke He thong Co dien tu ME4071 %%
%%                     Truong Dai hoc Bach Khoa                        %%
%%                           Khoa Co khi                               %%
%%                           ---***---                                 %%
%%               HAM TRUYEN DONG CO PHAI                               %%
%|               GVHD: Nguyen Tan Tien - Duong Van Tu                  |%
%|                                                                     |%
%|               SVTH: Huynh Nguyen Phong - 1914622                    |%
%|               Gmail: phong.huynhnguyen2401@hcmut.edu.vn             |%
%% ------------------------------------------------------------------- %%
%% Do not use to RUN %%
function dwdt = motorRi_tf(t,w)
global K a b PWMRi
K = 73.2;
a = 32.05;
b = 1;

dwdt = (-a*w(1) + K*PWMRi)/b;

end
   
   