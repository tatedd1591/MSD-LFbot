%% ------------------------------------------------------------------- %%
%%  Mo phong Robot do line - Do an thiet ke He thong Co dien tu ME4071 %%
%%                     Truong Dai hoc Bach Khoa                        %%
%%                           Khoa Co khi                               %%
%%                           ---***---                                 %%
%%               PHUONG TRINH DONG HOC ROBOT DO LINE                   %%
%|               GVHD: Nguyen Tan Tien - Duong Van Tu                  |%
%|                                                                     |%
%|               SVTH: Huynh Nguyen Phong - 1914622                    |%
%|               Gmail: phong.huynhnguyen2401@hcmut.edu.vn             |%
%% ------------------------------------------------------------------- %%
%% Do not use to RUN %%
function dq = kinematic_tf(t,q)
    global  vInput wInput
    dq    = zeros(3,1);
    dq(1) = cos(q(3))*vInput; % xM
    dq(2) = sin(q(3))*vInput; % yM
    dq(3) = wInput;           % phiM
end