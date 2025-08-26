library ieee;
use ieee.std_logic_1164.all;

entity xor_not_gate is
  port (
    A      : in  std_logic;
    B      : in  std_logic;
    Y      : out std_logic;
    X      : out std_logic
  );
end entity;

architecture comportamento of xor_not_gate is
  begin
    Y <= A xor B;
    X <= not A;
    
end architecture;
