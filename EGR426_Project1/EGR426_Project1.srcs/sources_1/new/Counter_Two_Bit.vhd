----------------------------------------------------------------------------------
 -- Title: Counter_Two_bit.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Two bit counter
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Counter_Two_Bit is
    port (
      clk_in: in std_logic; 
      reset: in std_logic;
      count_out: out std_logic_vector(1 downto 0));
end Counter_Two_Bit;

architecture Behavioral of Counter_Two_Bit is
  
  SIGNAL count: STD_LOGIC_VECTOR (1 downto 0);

  begin
      process(clk_in, reset)
        begin
        if (reset = '1') then -- synchronous
            count <= "00";
        elsif (rising_edge(clk_in)) then
            count <= count + 1; -- roll over after 11
        end if;
      end process;

  count_out <= count;
  
end Behavioral;
