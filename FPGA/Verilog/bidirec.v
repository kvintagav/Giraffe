`include "define.v"

module bidirec (oe, clk, inp, outp, bidir);

// Port Declaration

input   oe;
input   clk;
input   [`FSMC_WIDTH-1:0] inp;
output  [`FSMC_WIDTH-1:0] outp;
inout   [`FSMC_WIDTH-1:0] bidir;

reg     [`FSMC_WIDTH-1:0] a;
reg     [`FSMC_WIDTH-1:0] b;

assign bidir = oe ? a : 8'bZ ;
assign outp  = b;

// Always Construct

always @ ( posedge clk)
begin
    b <= bidir;
    a <= inp;
end

endmodule
