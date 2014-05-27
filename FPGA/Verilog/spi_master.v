`include "define.v"


module spi_master(

	input wire RESET,
	input wire CLK,

	output SCKOUT,
	input wire MISO,
	output MOSI,

	input wire START_SPI_TRANS,
	output STOP_SPI_TRANS,

	input wire [`SPI_WIDHT-1:0] DATA_IN,
	input wire [`MOUNT_BIT-1:0] MOUNT_BYTE,
	output [`DATA_READ_RAZR-1:0]DATA_READ


);

reg [7:0]state;
reg [9:0]cnt_wait;
reg stop_trans;
reg [`SPI_WIDHT-1:0] DATA_SPI_OUT;
reg [`SPI_WIDHT-1:0] DATA_SPI_IN;



parameter s_idle=0;
parameter s_start=1;
parameter s_work=2;
parameter s_stop=3;
parameter s_wait=4;

reg [5:0] cnt_bit;  //n?ao?ee aeoia a spi-iaeaoa
reg [`DATA_READ_RAZR-1:0] CNT_DATA_READ;
assign MOSI= (state == s_work) ? DATA_SPI_OUT[`SPI_WIDHT-1] : 1'bz;

assign  SCKOUT=(state==s_work) ? CLK : 1'b0;
assign STOP_SPI_TRANS=stop_spi_trans;

reg stop_spi_trans;
assign DATA_READ = CNT_DATA_READ;

always @(negedge CLK or posedge RESET)
begin
	if (RESET)
	begin
		DATA_SPI_OUT<=8'b0;
		state<=s_idle;
		cnt_bit<=6'b0;
		CNT_DATA_READ<=16'b0;
		stop_trans=0;
		cnt_wait<=10'b0;
	end
	else 
	begin
		case(state)
			s_idle:begin
				if (START_SPI_TRANS==1)
				begin
					state<=s_start;
					DATA_SPI_OUT<=DATA_IN;

				end				
			end
			s_start:begin
				state<=s_work;

			end
			s_work:begin
				if (cnt_bit<`SPI_WIDHT-1)
				begin
					cnt_bit<=cnt_bit+1'b1;
					DATA_SPI_OUT={DATA_SPI_OUT[`SPI_WIDHT-2:0],1'b0};
				end
				else 
				begin
					cnt_bit<=6'b0;
					if (stop_trans==0)
					begin
						//state<=s_start;
						DATA_SPI_OUT<=DATA_IN;
						state<=s_wait;
						cnt_wait<=`DELAY_SPI;
					end
					else
					begin
						state<=s_stop;
						stop_spi_trans=1;
						stop_trans=0;
					end
				end	

				if (cnt_bit==5) 
				begin
					if (CNT_DATA_READ<`LENTH_BUFER-1)
						CNT_DATA_READ<=CNT_DATA_READ+1'b1;
					else 
						stop_trans=1;
				end
			end
			s_wait:begin
				if (cnt_wait==0) state<=s_work;
				else cnt_wait<=cnt_wait-1'b1;
			end
			s_stop:begin
				state<=s_idle;
				stop_spi_trans=0;
				cnt_bit<=6'b0;
				CNT_DATA_READ<=16'b0;
				cnt_wait<=10'b0;

			end

		endcase
	end

end

endmodule