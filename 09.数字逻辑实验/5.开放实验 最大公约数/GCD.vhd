library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity gcd is
port(
    clk, rst: in std_logic;
    X:  in unsigned(3 downto 0);
    Z:  out unsigned(3 downto 0)
);
end gcd;

architecture behv of gcd is
signal mark: std_logic := '0';
signal gcd_prev: unsigned(3 downto 0):= "0000";
begin
    process(clk, rst)
    variable tmp_X, tmp_Y: unsigned(3 downto 0);
    begin
    if (rst = '1') then
        mark <= '0';
        gcd_prev <= "0000";
    elsif (clk'event and clk = '1') then
        if (mark = '0') then
            mark <= '1';
            gcd_prev <= X;
        else
            tmp_X := X;
            tmp_Y := gcd_prev;       
            for i in 0 to 15 loop
                if (tmp_X /= tmp_Y) then        
                    if (tmp_X < tmp_Y) then
                        tmp_Y := tmp_Y - tmp_X;
                    else
                        tmp_X := tmp_X - tmp_Y;
                    end if;
                else
                    gcd_prev <= tmp_X;
                end if;
            end loop;   
        end if;
    end if;
    end process;

    process(gcd_prev)
    begin
        Z <= gcd_prev;
    end process;
end;