library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
-- 四位全加器
entity full_adder_4 is
    port(
        a, b: in std_logic_vector(3 downto 0);
        cin: in std_logic;
        s: out std_logic_vector(3 downto 0);
        cout: out std_logic
    );
end full_adder_4;

-- 用configuration控制构造方法
configuration plus of full_adder_4 is
	for lookahead
	end for;
end plus;