
`define DELAY_SPI  				0
`define DELAY_STOP  				200

`define SPI_WIDHT  				8
`define MOUNT_BIT  				16
`define DATA_READ_RAZR  		16

`define LENTH_BUFER 				200

`define CONTROL_REG_MAX       16

//FSMC parameter

`define FSMC_WIDTH  				8
`define RESET_WAIT				25

/*command FSMC*/
`define CMD_RESET					255  
`define CMD_STATUS				112 
`define FPGA_CMD_START  		3
`define FPGA_CMD_DETECT 		4


`define CMD_READ_BUFER			1
`define CMD_WRITE_BUFER			128

`define CMD_ERASE_BUFER			96

`define CMD_READ_CONTROL_REG	2
`define CMD_WRITE_CONTROL_REG	129

/*STATUS_BITS*/
`define DEVICES_WORK 0   	// 1 -work , 0 - stop or delay math/filter level
`define BUFER_READY 	1		// bufer is ready
`define BUFER_READ  	2		//bufer read

