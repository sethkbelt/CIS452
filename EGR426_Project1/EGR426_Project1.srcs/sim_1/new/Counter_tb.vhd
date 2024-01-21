----------------------------------------------------------------------------------
 -- Title: Counter_tb.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Two bit counter test bench
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Counter_tb is
end Counter_tb;

architecture Behavioral of Counter_tb is

----------------------------------------------------------------------------------
component Counter_Two_Bit
  port (
      clk_in: in std_logic; -- instantiate counter entity
      reset: in std_logic;
      count_out: out std_logic_vector(1 downto 0)
        );
end component;
----------------------------------------------------------------------------------

  signal clk: std_logic := '0'; -- initialize inputs
  signal reset: std_logic := '0';
  signal count_out: std_logic_vector (1 downto 0);
  
begin                         -- module => testbench
  T1: Counter_Two_Bit port map (clk_in => clk, reset => reset, count_out => count_out); -- connect ports
  
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