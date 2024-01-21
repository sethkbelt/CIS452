----------------------------------------------------------------------------------
 -- Title: Clock_Divider_tb.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Clock divider test bench
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Clock_Divider_tb is
end Clock_Divider_tb;

architecture Behavioral of Clock_Divider_tb is

----------------------------------------------------------------------------------
component Clock_Divider_1Khz
  port (
    clk,reset: in std_logic;
    clk_div: out std_logic
        );
end component;
----------------------------------------------------------------------------------

  signal clk: std_logic := '0'; -- initialize inputs
  signal reset: std_logic := '0';
  signal count_out: std_logic := '0';
  
begin                         -- module => testbench
  T1: Clock_Divider_1Khz port map (clk => clk, reset => reset, clk_div => count_out); -- connect ports
  
  process         -- clock process for clk
    begin
      wait for 100 ns;    -- hold reset STATE for 100 ns
      reset <= '1';
      wait for 10 ns;
      reset <= '0';
      loop
        clk <= '0';
        wait for 10ns;
        clk <= '1';
        wait for 10ns;
      end loop;
    end process;
    
end Behavioral;
