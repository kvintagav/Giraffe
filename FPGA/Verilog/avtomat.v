/*
	Name:avtomat control
	Author: KAZAKOV Andrey.
	Date: 01.04.2014
*/
`include "define.v"

module avtomat(
input wire RESET,
input wire CLK,

output START_SPI_TRANS,
input wire STOP_SPI_TRANS,


input wire START_TRANSLATION,
output STOP_TRANSLATION,
output SWITCH,

output [`MOUNT_BIT-1:0] MOUNT_BYTE,

input wire STOP_BUFER,
output START_BUFER,
output READY_TRANS,
output [3:0]STATE
);

reg [7:0]r_wait=8'b0;
reg [3:0]state;
assign STATE=state;
parameter s_idle 			=0;
parameter s_start_byfer	=1;
parameter s_stop_byfer	=2;
parameter s_ready_trans	=3;
parameter s_wait_trans	=4; 
parameter s_start_spi	=5;
parameter s_wait_spi		=6;
parameter s_stop_spi		=7;

reg ready_trans=0;
reg start_spi_trans=0;
reg start_bufer=0;
reg switch=1;
reg stop_translation;

assign READY_TRANS=ready_trans;
assign START_SPI_TRANS=start_spi_trans;
assign STOP_TRANSLATION=stop_translation;
assign START_BUFER=start_bufer;
assign SWITCH=switch;
always @(posedge RESET or negedge CLK)
begin
	if (RESET)
	begin
		state<=s_idle;
		r_wait<=8'b0;
		switch=1;
		start_bufer=0;
	end
	else
	begin
		case(state)
		 s_idle:begin
			 state<=s_start_byfer;
			 start_bufer=1;
   	 end
		 s_start_byfer:begin
			 state<=s_stop_byfer;
			 start_bufer=0;
		 end
		 s_stop_byfer:
		 begin
			 if (STOP_BUFER==1)
			 begin
				state<=s_ready_trans;
				ready_trans=1;
				r_wait<=30;
			 end	
  		 end
		 s_ready_trans:  begin
			
			if (r_wait==0) begin
				state<=s_wait_trans;;
				ready_trans=0;
			end
			else r_wait<=r_wait-1'b1;
		 end
		 s_wait_trans:begin
		 	 if (START_TRANSLATION==1)begin
				state<=s_start_spi; 
				start_spi_trans=1;
			 end	
		 end
		 s_start_spi:begin 
			//state<=s_wait_spi;
			switch=0;
			start_spi_trans=0;
			if (STOP_SPI_TRANS==1)begin
				state<=s_stop_spi;
				switch=1;
				r_wait<=`DELAY_STOP;
				stop_translation=1'b1;
				
			end
		 end
	/*	 s_wait_spi:
		 begin
			if (STOP_SPI_TRANS==1)begin
				state<=s_stop_spi;
				switch=1;
				r_wait<=`DELAY_STOP;
				stop_translation=1'b1;
				
			end	
		 end
		*/s_stop_spi: begin
			if (r_wait==0) begin
				state<=s_idle;
				stop_translation=1'b0;
			end	
			else r_wait<=r_wait-1'b1;
		 
		 end
		 
		default: state<=s_idle;
		endcase
	
	end
	

end
endmodule

