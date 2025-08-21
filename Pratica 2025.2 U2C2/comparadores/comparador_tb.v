// =============================================
// Testbench para o módulo comparador4
// =============================================

`include "comparador.v"
`timescale 1ns/1ps

module comparador_tb;

    reg [3:0] A, B;
    wire AmaiorB, AmenorB, AigualB;

    // Instanciação do comparador
    comparador uut (
        .A(A),
        .B(B),
        .AmaiorB(AmaiorB),
        .AmenorB(AmenorB),
        .AigualB(AigualB)
    );

    initial begin
        $dumpfile("comparador.vcd");
        $dumpvars(0, comparador_tb);

        // Teste 1: A == B
        A = 4'b0101; B = 4'b0101; #10;

        // Teste 2: A > B
        A = 4'b1001; B = 4'b0110; #10;

        // Teste 3: A < B
        A = 4'b0011; B = 4'b1110; #10;

        // Teste 4: A == B (novamente)
        A = 4'b1111; B = 4'b1111; #10;

        // Teste 5: A < B
        A = 4'b0000; B = 4'b0001; #10;

        $finish;
    end

endmodule