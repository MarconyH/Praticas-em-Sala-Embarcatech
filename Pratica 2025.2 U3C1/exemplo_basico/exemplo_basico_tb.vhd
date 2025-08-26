library ieee;
use ieee.std_logic_1164.all;

entity exemplo_basico_tb is 
end entity;

architecture sim of exemplo_basico_tb is
  signal clk       : std_logic := '0';
  signal saida_var : std_logic;
  signal saida_sig : std_logic;
begin

  -- Instancia a unidade testada (DUT)
  DUT: entity work.exemplo_basico
    port map (
      clk       => clk,
      saida_var => saida_var,
      saida_sig => saida_sig
    );

  -- Geração de clock
  clk_process: process
  begin
    while now < 100 ns loop
      clk <= '0';
      wait for 10 ns;
      clk <= '1';
      wait for 10 ns;
    end loop;
    wait;
  end process;

end architecture;
