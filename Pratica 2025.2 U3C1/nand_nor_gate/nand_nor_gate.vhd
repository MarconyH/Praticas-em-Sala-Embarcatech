library ieee;
use ieee.std_logic_1164.all;

entity nand_nor_gate is
  port (
    A      : in  std_logic;
    B      : in  std_logic;
    Y      : out std_logic;
    X      : out std_logic
  );
end entity;

architecture comportamento of nand_nor_gate is
  begin
    Y <= not (A or B);
    X <= not (A and B);
    
end architecture;
