`timescale 1ns / 1ps

module tb_vga #(
	parameter WIDTH = 800,
	parameter HEIGHT = 600,
	parameter CLK_PERIOD = 1
);

reg clk;

always #CLK_PERIOD clk = ~clk;

reg  switch = 0;
wire led;

wire [3:0] vga_red;
wire [3:0] vga_green;
wire [3:0] vga_blue;
wire       v_sync;
wire       h_sync;

top top_inst (
	.clk      (clk      ),
	.switch   (switch   ),
	.led      (led      ),
	.vga_red  (vga_red  ),
	.vga_green(vga_green),
	.vga_blue (vga_blue ),
	.v_sync   (v_sync   ),
	.h_sync   (h_sync   )
);

import "DPI-C" function void write_pixel(
	input int x,
	input int y,
	input int r,
	input int g,
	input int b
);

/*
import "DPI-C" function void start_glfw();
import "DPI-C" function void wait_glfw();
*/

import "DPI-C" function void run();

initial begin
		$display("VGA Simulation");
		$display("==============");
		$display("Writing pixels");
		for(int y = 0; y < 600; y++) begin
			for(int x = 0; x < 800; x++) begin
				write_pixel(x, y, x[7:0], 8'h00, 8'h00);
			end
		end
		$display("Running display");
		run();
		$display("Simulation finished");
		$finish;
end
endmodule
