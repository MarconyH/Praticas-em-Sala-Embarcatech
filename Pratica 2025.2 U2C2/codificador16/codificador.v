module codificador (e1, e2, e3, e4, e5, e6, e7, e8, e9, s0, s1, s2, s3);
input e1, e2, e3, e4, e5, e6, e7, e8, e9;
output s0, s1, s2, s3;

wire xor123;
assign xor123 = e1 ^ e2 ^ e3 ^ e4 ^ e5 ^ e6 ^ e7 ^ e8 ^ e9 & ~(e1 & e2 & e3 & e4 & e5 & e6 & e7 & e8 & e9);

assign s0 = (e2 | e3) & xor123;
assign s1 = (e1 | e3) & xor123;
assign s2 = (e1 | e2) & xor123;
assign s3 = (e1 | e2 | e3) & xor123;
endmodule