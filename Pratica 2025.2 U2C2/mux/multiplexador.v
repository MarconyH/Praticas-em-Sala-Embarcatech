module mulltiplexador (e1, e2, s0, x);
input e1, e2, s0;
output x;

assign x = ~s0 & e1 | s0 & e2;
endmodule