library ieee;
use ieee.std_logic_1164.all;

entity comparador_4bits_tb is
end entity;

architecture sim of comparador_4bits_tb is
  signal A, B : std_logic_vector(3 downto 0) := (others => '0');
  signal Maior, Igual : std_logic;
begin

  -- Instancia o comparador
  DUT: entity work.comparador_4bits
    port map (
      A => A,
      B => B,
      Maior => Maior,
      Igual => Igual
    );

  -- Processo de estímulo
  stim: process
  begin
    wait for 1 ns;  -- Evita comparação com 'U' no tempo 0

    A <= "0001"; B <= "0000"; wait for 10 ns;  -- Maior = 1, Igual = 0
    A <= "0010"; B <= "0010"; wait for 10 ns;  -- Maior = 0, Igual = 1
    A <= "0011"; B <= "0100"; wait for 10 ns;  -- Maior = 0, Igual = 0

    wait;  -- Termina o processo
  end process;

end architecture;
