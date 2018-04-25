library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is port
	(
		SMCLK: in  STD_LOGIC;
		RESET: in  STD_LOGIC;
		ROW:   out STD_LOGIC_VECTOR (7 downto 0);
		LED:   out STD_LOGIC_VECTOR (7 downto 0)
	);
end ledc8x8;

architecture struc of ledc8x8 is

	signal enable: STD_LOGIC;
	signal rozsviet: STD_LOGIC_VECTOR (7 downto 0);
	signal riadok: STD_LOGIC_VECTOR (7 downto 0);
	signal change: STD_LOGIC_VECTOR(20 downto 0);
	signal variable1: STD_LOGIC := '0';
	signal variable2: STD_LOGIC_VECTOR(13 downto 0);
	begin
		process(RESET,SMCLK)
			begin
				if(RESET = '1') then
					riadok <= "10000000";
				elsif (SMCLK'event) and (SMCLK = '1') then

					if(variable2(13 downto 0) = "1111111111111") then
						variable1 <= '1';
					end if;

						if(enable = '1') then				
							riadok <= riadok(0) & riadok(7 downto 1);
							enable <='0';
						end if;

				if(change(7 downto 0) = "1111111") then
					enable <= '1';
					variable2 <= variable2 +1;
				end if;
				change <= change + 1;

				end if;
		end process;

	process (riadok) begin

		if(variable1 = '0') then
			case riadok is         
				when "10000000" => rozsviet <= "11111111";
				when "01000000" => rozsviet <= "11111111";
				when "00100000" => rozsviet <= "11111111";
				when "00010000" => rozsviet <= "11101110";
				when "00001000" => rozsviet <= "11101110";
				when "00000100" => rozsviet <= "11101010";
				when "00000010" => rozsviet <= "11100100";
				when "00000001" => rozsviet <= "11101110";	
				when others => rozsviet <= "11111111";
			end case;

		else   
			case riadok is         
				when "10000000" => rozsviet <= "00011111";
				when "01000000" => rozsviet <= "11101111";
				when "00100000" => rozsviet <= "11101111";
				when "00010000" => rozsviet <= "11101110";
				when "00001000" => rozsviet <= "00001110"; 
				when "00000100" => rozsviet <= "11101010"; 
				when "00000010" => rozsviet <= "11100100"; 
				when "00000001" => rozsviet <= "11101110"; 	
				when others => rozsviet <= "11111111";
			end case;

	      end if;

	end process;

		ROW <= riadok;
		LED <= rozsviet;

	end struc;