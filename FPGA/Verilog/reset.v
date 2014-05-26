`include "define.v"

module reset(
input CLK_IN,
output reg RESET
);
reg [1:0] cnt=2'b00;



always @(posedge CLK_IN )
begin
	if (cnt==0) RESET<=1'b1;
	else if (cnt==1) RESET<=1'b0;
	

	if (cnt<2)cnt<=cnt+1'b1;
end




endmodule
