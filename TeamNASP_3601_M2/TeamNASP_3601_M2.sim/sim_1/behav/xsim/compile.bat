@echo off
REM ****************************************************************************
REM Vivado (TM) v2021.1 (64-bit)
REM
REM Filename    : compile.bat
REM Simulator   : Xilinx Vivado Simulator
REM Description : Script for compiling the simulation design source files
REM
REM Generated by Vivado on Wed Nov 01 20:13:35 +1100 2023
REM SW Build 3247384 on Thu Jun 10 19:36:33 MDT 2021
REM
REM IP Build 3246043 on Fri Jun 11 00:30:35 MDT 2021
REM
REM usage: compile.bat
REM
REM ****************************************************************************
REM compile Verilog/System Verilog design sources
echo "xvlog --relax -L axi_vip_v1_1_10 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_10 -L xilinx_vip -prj i2s_master_tb_vlog.prj"
call xvlog  --relax -L axi_vip_v1_1_10 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_10 -L xilinx_vip -prj i2s_master_tb_vlog.prj -log xvlog.log
call type xvlog.log > compile.log
REM compile VHDL design sources
echo "xvhdl --relax -prj i2s_master_tb_vhdl.prj"
call xvhdl  --relax -prj i2s_master_tb_vhdl.prj -log xvhdl.log
call type xvhdl.log >> compile.log
if "%errorlevel%"=="1" goto END
if "%errorlevel%"=="0" goto SUCCESS
:END
exit 1
:SUCCESS
exit 0
