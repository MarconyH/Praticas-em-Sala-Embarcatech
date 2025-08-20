`include "paridade_par.v"
`timescale 1ns/100ps

module paridade_par_tb;
reg a0, b0, c0;
wire s0;

paridade_par uut (.a(a0), .b(b0), .c(c0), .s(s0));

initial begin
    $dumpfile("paridade_par.vcd");
    $dumpvars(0, paridade_par_tb);
    a0 = 0; b0 = 0; c0 = 0; 
    #10; a0 = 0; b0 = 0; c0 = 1;
    #10; a0 = 0; b0 = 1; c0 = 0;
    #10; a0 = 0; b0 = 1; c0 = 1;
    #10; a0 = 1; b0 = 0; c0 = 0;
    #10; a0 = 1; b0 = 0; c0 = 1;
    #10; a0 = 1; b0 = 1; c0 = 0;
    #10; a0 = 1; b0 = 1; c0 = 1;
    #10; $finish;
end
endmodule