`include "multiplexador.v"
`timescale 1ns/100ps

module multiplexador_tb;
reg e1, e2, e3, e4, s0, s1;
wire x;

multiplexador uut (.e1(e1), .e2(e2), .e3(e3), .e4(e4), .s0(s0), .s1(s1), .x(x));

initial begin
    $dumpfile("multiplexador.vcd");
    $dumpvars(0, multiplexador_tb);
    s1 = 0; s0 = 0; e1 = 0; e2 = 0; e3 = 0; e4 = 0; #10;
    for (integer i = 0; i < 64; i++) begin
        {s1, s0, e1, e2, e3, e4} = i+1;
        #10;
    end

end
endmodule