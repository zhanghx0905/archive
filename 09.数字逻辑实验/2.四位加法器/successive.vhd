-- 逐次进位加法器
architecture successive of full_adder_4 is
component full_adder_1
port(
a, b, cin: in std_logic;
s, cout: out std_logic
);
end component;
signal c: std_logic_vector(2 downto 0);
begin
	adder0 : full_adder_1 port map(a(0), b(0), cin, s(0), c(0));
	adder1 : full_adder_1 port map(a(1), b(1), c(0), s(1), c(1));
	adder2 : full_adder_1 port map(a(2), b(2), c(1), s(2), c(2));
	adder3 : full_adder_1 port map(a(3), b(3), c(2), s(3), cout);
end;