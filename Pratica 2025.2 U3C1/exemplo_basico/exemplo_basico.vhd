library ieee;
use ieee.std_logic_1164.all;

entity exemplo_basico is
  port (
    clk      : in  std_logic;
    saida_var : out std_logic;
    saida_sig : out std_logic
  );
end entity;

architecture comportamento of exemplo_basico is
  signal s_contador : std_logic := '0';
begin

  process(clk)
    variable v_contador : std_logic := '0';
  begin
    if rising_edge(clk) then
      -- Atualiza a variável e o sinal
      v_contador := not v_contador;
      s_contador <= not s_contador;

      -- Atribui para as saídas
      saida_var <= v_contador;
      saida_sig <= s_contador;
    end if;
  end process;

end architecture;
