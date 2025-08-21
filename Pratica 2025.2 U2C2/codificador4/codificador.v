module codificador (e1, e2, e3, s0, s1);
input e1, e2, e3;
output s0, s1;

wire xor123;
assign xor123 = e1 ^ e2 ^ e3 & ~(e1 & e2 & e3);

assign s0 = (e2 | e3) & xor123;
assign s1 = (e1 | e3) & xor123;
endmodule