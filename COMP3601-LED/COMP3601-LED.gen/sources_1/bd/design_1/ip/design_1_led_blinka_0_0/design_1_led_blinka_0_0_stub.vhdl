-- Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2021.1 (win64) Build 3247384 Thu Jun 10 19:36:33 MDT 2021
-- Date        : Wed Sep 20 13:13:54 2023
-- Host        : AronLaptop running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top design_1_led_blinka_0_0 -prefix
--               design_1_led_blinka_0_0_ design_1_led_blinka_0_0_stub.vhdl
-- Design      : design_1_led_blinka_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xck26-sfvc784-2LV-c
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity design_1_led_blinka_0_0 is
  Port ( 
    clk_10MHZ : in STD_LOGIC;
    led_out : out STD_LOGIC
  );

end design_1_led_blinka_0_0;

architecture stub of design_1_led_blinka_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_10MHZ,led_out";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "led_blinka,Vivado 2021.1";
begin
end;
