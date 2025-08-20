module paridade_impar (a, b, c, s);
input a, b, c;
output s;

assign s = a ^ b ^ c;
endmodule