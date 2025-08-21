// =============================================
// Módulo: Comparador 4 bits
// =============================================
// Entradas:
//   – A3, A2, A1, A0: Palavra A de 4 bits
//   – B3, B2, B1, B0: Palavra B de 4 bits
// Saídas:
     //   – AmaiorB: nível alto (1) se A > B
     //   – AmenorB: nível alto (1) se A < B
     //   – AigualB: nível alto (1) se A = B
  //============================================
module comparador (
    input  [3:0] A,
    input  [3:0] B,
    output AmaiorB,
    output AmenorB,
    output AigualB
);

    assign AmaiorB = (A > B) ? 1'b1 : 1'b0;
    assign AmenorB = (A < B) ? 1'b1 : 1'b0;
    assign AigualB = (A == B) ? 1'b1 : 1'b0;

endmodule