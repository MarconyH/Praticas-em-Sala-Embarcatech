library ieee;
use ieee.std_logic_1164.all;

entity xor_not_gate_tb is 
end entity;

architecture sim of xor_not_gate_tb is
  signal A, B, Y, X: std_logic;
begin

  -- Instancia a unidade testada (DUT)
  DUT: entity work.xor_not_gate
    port map (
      A => A,
      B => B,
      Y => Y,
      X => X
    );

  -- Geração de clock
  stim: process
  begin
    A <='0'; B <= '0'; wait for 10 ns;
    A <='0'; B <= '1'; wait for 10 ns;
    A <='1'; B <= '0'; wait for 10 ns;
    A <='1'; B <= '1'; wait for 10 ns;
    wait;
  end process;

end architecture;
