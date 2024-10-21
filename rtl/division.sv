module division #(
	parameter INTEGER = 10,
	parameter DECIMAL =  7,
	parameter PRECISION = 1 + INTEGER + DECIMAL
)(
	input                  clk    ,
	input                  vld    ,
	input  [PRECISION-1:0] in_n   ,
	input  [PRECISION-1:0] in_d   ,
	output                 out_vld,
	output [PRECISION-1:0] out_r
);

reg [PRECISION-1:0] reg_n;
reg [PRECISION-1:0] reg_d;

wire                 stage_1_vld;
wire [PRECISION-1:0] stage_1_r;

wire                 stage_2_vld;
wire [PRECISION-1:0] stage_2_r;

wire                 stage_3_vld;
wire [PRECISION-1:0] stage_3_r;

//Three stages to reach good precision using NR division
nr_stage #(
	.INTEGER(INTEGER),
	.DECIMAL(DECIMAL)
) nr_stage_inst_1 (
	.clk    (clk   ),
	.in_vld (in_vld),
	.in_x   (      ),
	.in_d   (in_d  ),
	.out_vld(stage_1_vld),
	.out_r  (stage_1_r)
);

nr_stage #(
	.INTEGER(INTEGER),
	.DECIMAL(DECIMAL)
) nr_stage_inst_2 (
	.clk    (clk        ),
	.in_vld (stage_1_vld),
	.in_x   (stage_1_r  ),
	.in_d   (in_d       ),
	.out_vld(stage_2_vld),
	.out_r  (stage_2_r  )
);

nr_stage #(
	.INTEGER(INTEGER),
	.DECIMAL(DECIMAL)
) nr_stage_inst_3 (
	.clk    (clk        ),
	.in_vld (stage_2_vld),
	.in_x   (stage_2_r  ),
	.in_d   (in_d       ),
	.out_vld(stage_3_vld),
	.out_r  (stage_3_r  )
);

always @(posedge clk) begin
	reg_n <= in_n;
	reg_d <= in_d;
end

endmodule
