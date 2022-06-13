-- 本代码用于Modelsim-Alter仿真
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.all;
USE IEEE.STD_LOGIC_ARITH.all;
USE IEEE.STD_LOGIC_UNSIGNED.all;

entity digitalLife is
    port(
        light_even_4: out std_logic_vector(3 downto 0); --带译码的偶数列
        light_odd_4: out std_logic_vector(3 downto 0); --带译码的奇数列
        light_natural_7: out std_logic_vector(6 downto 0); --不带译码的自然数列
        clk, rst: in std_logic --clk对应clk接口；rst作为复位器，对应rst接口
    );
end digitalLife;

architecture lighting of digitalLife is
    signal signal_natural_7: std_logic_vector(3 downto 0):="0000";
    signal signal_odd_4: std_logic_vector(3 downto 0):="0001";
    signal signal_even_4: std_logic_vector(3 downto 0):="0000";
    signal cnt: integer:=0; --计数器
begin
    process(clk, rst) --clk/rst变化时触发，只改变signal
    begin
        if(clk'event and clk='1') then --clk被按下，计数器加1
                cnt <= 0;
                if (signal_even_4 ="1000") then --偶数列 8->0
                    signal_even_4 <= "0000";
                else
                    signal_even_4 <= signal_even_4 + 2;
                end if;
                if (signal_odd_4="1001") then -- 奇数列 9->1
                    signal_odd_4 <= "0001";
                else
                    signal_odd_4 <= signal_odd_4 + 2;
                end if;
                if (signal_natural_7="1001") then --只显示自然数列0-9
                    signal_natural_7 <= "0000";
                else
                    signal_natural_7 <= signal_natural_7 + '1';
                end if;
			end if;
        if (rst='1') then --复原计数器
            signal_natural_7 <= "0000";
            signal_odd_4 <= "0001";
            signal_even_4 <= "0000";
        end if;     
    end process;

    process(signal_even_4) --改变偶数列的输出
    begin
        light_even_4 <= signal_even_4;
        end process;

    process(signal_odd_4) --改变奇数列的输出
    begin
        light_odd_4 <= signal_odd_4;
    end process;

    process(signal_natural_7) --改变自然数列的输出
    begin
        case( signal_natural_7 ) is
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
end lighting;