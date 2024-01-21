----------------------------------------------------------------------------------
 -- Title: Decoder_Seven_Segment.vhd
 -- Author: Seth Konynenbelt
 -- Created: January 20, 2023
 -- Description: Seven Segment Decoder
 ----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Decoder_Seven_Segment is
  port(
      bcd: in std_logic_vector(3 downto 0);
      seg_out: out std_logic_vector(7 downto 0));
end Decoder_Seven_Segment;

architecture Behavioral of Decoder_Seven_Segment is

begin
  process(bcd)
    begin
      case bcd is
        -- corresponds to basys-3 segment control bits
        when "0000" => seg_out <= "00000001";
        when "0001" => seg_out <= "01001111";
        when "0010" => seg_out <= "00010010";
        when "0011" => seg_out <= "00000110";
        when "0100" => seg_out <= "01001100";
        when "0101" => seg_out <= "00100100";
        when "0110" => seg_out <= "00001111";
        when "0111" => seg_out <= "00000000";
        when "1000" => seg_out <= "00000100";
        when "1001" => seg_out <= "01001111";
        
        when "1010" => seg_out <= "10101010"; -- just random for now
        when "1011" => seg_out <= "10101010";
        when "1100" => seg_out <= "10101010";
        when "1101" => seg_out <= "10101010";
        when "1110" => seg_out <= "10101010";
        when "1111" => seg_out <= "00000000";
        when others => seg_out <= "XXXXXXXX";

      end case;
    end process;

end Behavioral;
