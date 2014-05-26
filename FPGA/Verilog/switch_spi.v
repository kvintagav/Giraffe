/*
	Module switch spi 
	if SWITCH = 1 with WIZNET work STM32
	else with WIZNET work FPGA
*/
module switch_spi(

input wire 	FPGA_CLK,
output 		WIZ_CLK,
input wire 	FPGA_MOSI,
output 		WIZ_MOSI,
output		MK_MISO,
output 		FPGA_MISO,
input wire 	WIZ_MISO,
input wire SWITCH,
input wire 	MK_CLK,
input wire 	MK_MOSI
);


assign WIZ_CLK=(SWITCH==1) ? MK_CLK : FPGA_CLK;
assign WIZ_MOSI=(SWITCH==1) ? MK_MOSI : FPGA_MOSI;
assign MK_MISO=(SWITCH==1) ? WIZ_MISO : 1'b0;
assign FPGA_MISO=(SWITCH==0) ? WIZ_MISO : 1'b0;



endmodule
