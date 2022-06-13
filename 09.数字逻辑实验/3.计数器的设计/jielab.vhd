library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity counter4 is
	port(
		clk, rst, pause: in std_logic;
		mode: in std_logic;
		d0, d1: buffer std_logic_vector(3 downto 0)
	);
end counter4;

architecture count of counter4 is
	signal cnt: integer := 0;
begin
	process(clk, rst)
	begin
		if (rst = '1') then
			d0 <= "0000";
			d1 <= "0000";
			cnt <= 0;
		elsif (clk'event and clk = '1' and pause = '0') then
			if (mode = '1') then -- 秒表计数
				if (cnt < 1000000) then
					cnt <= cnt + 1;
				else
					cnt <= 0;
				end if;
			end if;
			if (mode = '0' or cnt = 0) then -- 手动计数
				if (d0 = "1001") then
					d0 <= "0000";
					if (d1 = "0101") then
						d1 <= "0000";
					else
						d1 <= d1 + 1;
					end if;
				else
					d0 <= d0 + 1;
				end if;
			end if;
		end if;
	end process;
end;

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.all;
USE IEEE.STD_LOGIC_ARITH.all;
USE IEEE.STD_LOGIC_UNSIGNED.all;

entity digit7 is
    port(
		 digit: in std_logic_vector(3 downto 0);
       light_natural_7: out std_logic_vector(6 downto 0)
    );
end digit7;

architecture lighting of digit7 is
begin
	process(digit)
	begin
		case (digit) is
			when "0000" => light_natural_7<="1111110";
         when "0001" => light_natural_7<="0110000";
         when "0010" => light_natural_7<="1101101";
         when "0011" => light_natural_7<="1111001";
         when "0100" => light_natural_7<="0110011";
         when "0101" => light_natural_7<="1011011";
         when "0110" => light_natural_7<="1011111";
         when "0111" => light_natural_7<="1110000";
         when "1000" => light_natural_7<="1111111";
         when "1001" => light_natural_7<="1111011";
         when "1010" => light_natural_7<="1110111";
         when "1011" => light_natural_7<="0011111";
         when "1100" => light_natural_7<="1001110";
         when "1101" => light_natural_7<="0111101";
         when "1110" => light_natural_7<="1001111";
         when "1111" => light_natural_7<="1000111";
         when others => light_natural_7<="0000000";
		end case;
	end process;
end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity counter is
	port(
		clk, rst, pause: in std_logic;
		mode: in std_logic;
		l0, l1: out std_logic_vector(6 downto 0)
	);
end counter;

architecture count of counter is
	signal d0: std_logic_vector(3 downto 0) := "0000";
   signal d1: std_logic_vector(3 downto 0) := "0000";
	component counter4
		port(
			clk, rst, mode, pause: in std_logic;
			d0, d1: buffer std_logic_vector(3 downto 0)
		);
	end component;
	component digit7
		port(
			digit: in std_logic_vector(3 downto 0);
			light_natural_7: out std_logic_vector(6 downto 0)
		);	
	end component;
begin
	FF: counter4 port map(clk=>clk, rst=>rst, d0=>d0, d1=>d1, mode=>mode, pause=>pause);
	light0: digit7 port map(digit=>d0, light_natural_7=>l0);
	light1: digit7 port map(digit=>d1, light_natural_7=>l1);
end;