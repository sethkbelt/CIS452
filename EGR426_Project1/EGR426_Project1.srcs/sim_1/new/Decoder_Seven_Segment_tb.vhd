----------------------------------------------------------------------------------
 -- Title: Deocder_Seven_Segment_tb.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Deocder for 7 segment test bench
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Decoder_Seven_Segment_tb is
end Decoder_Seven_Segment_tb;

architecture Behavioral of Decoder_Seven_Segment_tb is

----------------------------------------------------------------------------------
component Counter_Two_Bit
port (
    bcd: in std_logic_vector(3 downto 0);
    seg_out: out std_logic_vector(7 downto 0));
end component;
----------------------------------------------------------------------------------


signal bcd : std_logic_vector(3 downto 0) := "0000"; -- initialize inputs
signal seg_out:  std_logic_vector(7 downto 0);

component Decoder_Seven_Segment is
    port(
        bcd: in std_logic_vector(3 downto 0);
        seg_out: out std_logic_vector(7 downto 0));
end component;
  
begin
    T1: Decoder_Seven_Segment port map (bcd => bcd, seg_out => seg_out); -- connect ports
 
  process         -- clock process for clk
    begin
      wait for 100 ns;    -- hold reset STATE for 100 ns
      loop
        bcd <= bcd + 1;
        wait for 10ns;
      end loop;
    end process;
    
end Behavioral;