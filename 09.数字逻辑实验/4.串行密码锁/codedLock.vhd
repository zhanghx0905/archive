library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity codedLock is
	port(
		rst, clk: in std_logic;
		code: in std_logic_vector(3 downto 0);
		mode: in std_logic; -- 0: set pwd; 1: verify pwd
		unlocked, err, alarm: out std_logic
	);
	type IntArray4 is array (3 downto 0) of integer;
end codedLock;

architecture lock of codedLock is
	signal pwd: IntArray4;
	signal state: integer := 0;
	signal cnt: integer := 0; -- 用户密码输入错误次数/管理员密码正确位数
	signal alarm_signal: std_logic := '0';
	signal err_signal: std_logic := '0';
begin
	process(clk, rst)
	begin
		if (rst = '1') then
			unlocked <= '0';
			err_signal <= '0';
			state <= 0;
			if (alarm_signal = '1') then
				cnt <= 0;
			end if;
		elsif (clk'event and clk = '1') then -- clk上升沿读密码
			if (alarm_signal = '1') then -- 锁定状态
				if (CONV_INTEGER(code) = 1) then -- 管理员密码 1111
					if (cnt < 3) then 
						cnt <= cnt + 1;
					else -- 管理员验证成功，清除alarm
						cnt <= 0;
						alarm_signal <= '0';
						state <= 0;
					end if;
				else
					cnt <= 0;
				end if;
			elsif (mode = '0') then -- set pwd
				case state is
					when 0 => 
						pwd(0) <= CONV_INTEGER(code); state <= 1;
					when 1 => 
						pwd(1) <= CONV_INTEGER(code); state <= 2;
					when 2 => 
						pwd(2) <= CONV_INTEGER(code); state <= 3;
					when 3 => 
						pwd(3) <= CONV_INTEGER(code); state <= 7; unlocked <= '1';
					when others => NULL;
				end case;
			elsif (mode = '1') then -- varify pwd
				case state is
					when 0 => -- varify 1st bit
						if (CONV_INTEGER(code) = pwd(0)) then
							state <= 4;
							err_signal <= '0';
						else
							err_signal <= '1';
							if (cnt < 2) then
								cnt <= cnt + 1;
							else -- fails 3 times, set err = 1
								alarm_signal <= '1';
								cnt <= 0;
							end if;
						end if;
					when 4 => -- varify 2nd bit
						if (CONV_INTEGER(code) = pwd(1)) then
							state <= 5;
							err_signal <= '0';
						else
							err_signal <= '1';
							state <= 0;
							if (cnt < 2) then
								cnt <= cnt + 1;
							else -- fails 3 times, set err = 1
								alarm_signal <= '1';
								cnt <= 0;
							end if;
						end if;
					when 5 => -- varify 3rd bit
						if (CONV_INTEGER(code) = pwd(2)) then
							state <= 6;
							err_signal <= '0';
						else
							err_signal <= '1';
							state <= 0;
							if (cnt < 2) then
								cnt <= cnt + 1;
							else -- fails 3 times, set err = 1
								alarm_signal <= '1';
								cnt <= 0;
							end if;
						end if;
					when 6 => -- varify 4th bit
						if (CONV_INTEGER(code) = pwd(3)) then
							state <= 7;
							err_signal <= '0';
							unlocked <= '1';
						else
							err_signal <= '1';
							state <= 0;
							if (cnt < 2) then
								cnt <= cnt + 1;
							else -- fails 3 times, set err = 1
								alarm_signal <= '1';
								cnt <= 0;
							end if;
						end if;
					when others => NULL;
				end case;
			end if;
		end if;
	end process;

	process(alarm_signal)
	begin
		alarm <= alarm_signal;
	end process;
	
	process(err_signal)
	begin
		err <= err_signal;
	end process;
end;