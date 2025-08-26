library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity comparador_4bits is
  port (
    A, B  : in std_logic_vector(3 downto 0);
    Maior : out std_logic;
    Igual : out std_logic
  );
end comparador_4bits;

architecture rtl of comparador_4bits is
begin
  Maior <= '1' when unsigned(A) > unsigned(B) else '0';
  Igual <= '1' when A = B else '0';
end rtl;
