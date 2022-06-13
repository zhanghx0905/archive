library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;

entity digitalLife_tb is
end;

architecture bench of digitalLife_tb is

  component digitalLife
      port(
          light_even_4: out std_logic_vector(3 downto 0);
          light_odd_4: out std_logic_vector(3 downto 0);
          light_natural_7: out std_logic_vector(6 downto 0);
          clk, rst: in std_logic
      );
  end component;

  signal light_even_4: std_logic_vector(3 downto 0);
  signal light_odd_4: std_logic_vector(3 downto 0);
  signal light_natural_7: std_logic_vector(6 downto 0);
  signal clk, rst: std_logic ;

  constant clock_period: time := 10 ns;
  signal stop_the_clock: boolean;

begin

  uut: digitalLife port map ( light_even_4    => light_even_4,
                              light_odd_4     => light_odd_4,
                              light_natural_7 => light_natural_7,
                              clk             => clk,
                              rst             => rst );

  stimulus: process
  begin
  
    -- Put initialisation code here

	for i in 0 to 2 loop
		clk <= '1';
		wait for clock_period;
		clk <= '0';
		wait for clock_period;
	end loop;
	rst <= '1';
	wait for clock_period;
	rst <= '0';
	wait for clock_period;
	for i in 0 to 4 loop
		clk <= '1';
		wait for clock_period;
		clk <= '0';
		wait for clock_period;
	end loop;
    -- Put test bench stimulus code here

    stop_the_clock <= true;
    wait;
  end process;

  clocking: process
  begin
    while not stop_the_clock loop
      clk <= '0', '1' after clock_period / 2;
      wait for clock_period;
    end loop;
    wait;
  end process;

end;