----------------------------------------------------------------------------------
 -- Title: Decoder_Two_To_Four.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Two to four decoder
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Decoder_Two_To_Four is
  port(
    sel: in std_logic_vector(1 downto 0);
    anode: out std_logic_vector(3 downto 0));
end Decoder_Two_To_Four;

architecture Behavioral of Decoder_Two_To_Four is

begin

  -- select what anode is enabled
  process(sel)
    begin
      case sel is 
        when "00" => anode <= "0001";
        when "01" => anode <= "0010";
        when "10" => anode <= "0100";
        when "11" => anode <= "1000";
        when others => anode <= "X";
      end case;
    end process;

end Behavioral;
        