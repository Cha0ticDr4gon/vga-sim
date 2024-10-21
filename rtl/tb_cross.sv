`timescale 1ns / 1ps

module tb_cross #(
	parameter CLK_PERIOD = 1,
	parameter INTEGER = 10,
	parameter DECIMAL =  7,
	parameter PRECISION = 1 + INTEGER + DECIMAL
);

reg clk = 0;
always #CLK_PERIOD clk = ~clk;

reg  [PRECISION-1:0] ax;
reg  [PRECISION-1:0] ay;
reg  [PRECISION-1:0] bx;
reg  [PRECISION-1:0] by;
wire [PRECISION-1:0] z;

reg  in_vld;
wire out_vld;

cross_product #(
	.INTEGER(INTEGER),
	.DECIMAL(DECIMAL)
) cross_product_inst (
	.clk    (clk    ),
	.in_vld (in_vld ),
	.in_ax  (ax     ),
	.in_ay  (ay     ),
	.in_bx  (bx     ),
	.in_by  (by     ),
	.out_vld(out_vld),
	.out_z  (z      )
);

initial begin
	$display("Cross product test");
	//Initial values for everything
	in_vld <= 0;

	//These should be perpendicular to each other in the XY plane
	//Point 1
	ax <= int'($floor(2.5 * (2**DECIMAL)));
	ay <= int'($floor(4.1 * (2**DECIMAL)));
	//Point 2
	bx <= int'($floor(-4 * (2**DECIMAL)));
	by <= int'($floor(2.1 * (2**DECIMAL)));

	@(posedge clk);

	$display("ax = %B.%B", ax[PRECISION-1:DECIMAL], ax[DECIMAL-1:0]);
	$display("ay = %B.%B", ay[PRECISION-1:DECIMAL], ay[DECIMAL-1:0]);
	$display("bx = %B.%B", bx[PRECISION-1:DECIMAL], bx[DECIMAL-1:0]);
	$display("by = %B.%B", by[PRECISION-1:DECIMAL], by[DECIMAL-1:0]);

	//Set valid for one clock cycle
	@(posedge clk);
	in_vld <= 1;
	@(posedge clk);
	in_vld <= 0;

	@(posedge out_vld);
	$display("result = %B.%B", z[PRECISION-1:DECIMAL], z[DECIMAL-1:0]);
	$finish;
end

endmodule
