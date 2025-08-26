module multiplexador (e1, e2, e3, e4, s0, s1, x);
input e1, e2, e3, e4, s0, s1;
output x;

assign x = ~s0 & ~s1 & e1 | s0 & ~s1 & e2 | ~s0 & s1 & e3 | s0 & s1 & e4;
endmodule