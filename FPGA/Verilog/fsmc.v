`include "define.v"

module fsmc(
input wire CLK,
input wire CLE,
input wire ALE,
input wire NRE,
input wire NCE,
input wire NWE,

output NWAIT,

output RESET_FSMC,
inout   [`FSMC_WIDTH-1:0] BIDIR,

output START_DEVICES,
output [`FSMC_WIDTH-1:0]CONTROL_REG,
input wire[`FSMC_WIDTH-1:0] IN_DATA,

output [`DATA_READ_RAZR-1:0]BYTE_CNT,

input wire STOP_BUFER
);

wire [7 : 0] A;
assign A = BIDIR[7:0];
wire [`FSMC_WIDTH-1 : 0 ] DIn;
assign DIn = BIDIR[`FSMC_WIDTH-1:0];

wire [`FSMC_WIDTH-1:0]bus_data_com;

reg 								 reset_fsmc=1'b0;
reg 								 start_devices=1'b0;
reg							    read=1'b0;
reg [7 : 0] 					 addr;
reg [7:0] 						 command;
reg [`FSMC_WIDTH-1:0]		 in_byte;
reg [`FSMC_WIDTH-1:0]		 bus_out=8'bzzzz_zzzz;
reg [`FSMC_WIDTH-1:0]		 control_reg;
reg [`FSMC_WIDTH-1:0]		 status;
reg [`DATA_READ_RAZR-1:0]	 byte_cnt=16'b0;
reg [`FSMC_WIDTH-1:0]		 bus_out_bufer;	


assign CONTROL_REG=control_reg;
assign BYTE_CNT=byte_cnt;
assign BIDIR= (read==1'b1) ? bus_data_com : 8'bzzzz_zzzz;
assign START_DEVICES=start_devices;
assign RESET_FSMC= reset_fsmc;
assign NWAIT=1'b0;
assign bus_data_com=(command==`CMD_READ_BUFER) ? bus_out_bufer : bus_out;

parameter IDLE= 		0;
parameter RESET= 		1;
parameter WAIT_ADDR=	2;
parameter WAIT_BYTE=	3;
parameter READ_BYTE=	4;
parameter WRITE_BYTE=	5;


initial
begin

	read           =1'b0;
	command			=8'b0000_0000;
	addr				=8'b0000_0000;
	in_byte			=8'b0000_0000;
	status			=8'b0000_0000;
	bus_out			=8'b0000_0000;

end
/*status devices*/
always @(posedge STOP_BUFER or posedge CLK)
begin
	if (STOP_BUFER)
	begin
		status[`BUFER_READY]<=1'b1;
	end
	else 
	begin
		if (start_devices==1) status[`DEVICES_WORK]<=1'b1;
		if ((byte_cnt<`LENTH_BUFER )&&(command==`CMD_READ_BUFER)) status[`BUFER_READ]<=1'b1;
		else status[`BUFER_READ]<=1'b0;
		
		if ((read==1)&&(byte_cnt==1)) status[`BUFER_READY]<=1'b0;

	end
end

always@ (negedge NRE or posedge CLE)
begin
	if (CLE)
	begin
		 byte_cnt<=16'b0;
	end
	else
	begin

		bus_out_bufer<=IN_DATA;
		byte_cnt<=byte_cnt+1'b1;
	//	if (byte_cnt<=`LENTH_BUFER) byte_cnt<=byte_cnt+1'b1;
	//	else byte_cnt<=16'b0;
	end
end
always @(posedge NCE or negedge NRE)
begin
	if (NCE)
	begin
		read <= 1'b0;
	end
	else
	begin
		read <= 1'b1;

	end
end	

/*Latches 8 bit address on rising edge of WE#
Latches data on rising edge of WE#
*/


always @ (posedge NWE or posedge NCE)
begin
	if (NCE)
	begin
		reset_fsmc<=1'b0;
		start_devices<=1'b0;


	end
	else
	begin

		if ( ALE && ~NCE && ~CLE && NRE)
		begin	
		end
		if ( ~ALE  && ~NCE && ~CLE && NRE) //write byte
		begin

			if (command==`CMD_WRITE_CONTROL_REG)
			begin

				control_reg<= DIn;
			end
		end	
		if ( ~ALE && ~NCE && CLE && NRE)
		begin	
			command <= A[7:0];
			case(A[7:0])
				`CMD_RESET:reset_fsmc<=1'b1;
				`CMD_READ_CONTROL_REG:bus_out<=control_reg;
				`FPGA_CMD_START:start_devices<=1'b1;

				`CMD_STATUS:bus_out<=status;
				`FPGA_CMD_DETECT:bus_out<=8'b10100101; //0xA5 - fpga detect sms
				default:begin

					//bus_out<=status;
					/*Illegal command*/
				end
			endcase	
		end	
	end
end

endmodule