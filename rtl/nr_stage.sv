module nr_stage #(
	parameter INTEGER = 10,
	parameter DECIMAL =  7,
	parameter PRECISION = 1 + INTEGER + DECIMAL
)(
	input                  clk    ,
	input                  in_vld ,
	input  [PRECISION-1:0] in_d   ,
	input  [PRECISION-1:0] in_x   ,
	output                 out_vld,
	output [PRECISION-1:0] out_r
);

localparam DELAY = 4;
reg [DELAY-1:0] reg_vld_prop;

reg [PRECISION-1:0] reg_d;
reg [PRECISION-1:0] reg_x;
reg [PRECISION-1:0] reg_x_prop [1:0];
reg [PRECISION*2-1:0] reg_dx;
reg [PRECISION*2-1:0] reg_2_dx;

wire signed [PRECISION*2-1:0] dx;
wire signed [PRECISION-1:0] 2_dx;
wire signed [PRECISION*2-1:0] res;

always @(posedge clk) begin
	//Valid signal propogation
	reg_vld_prop[0] <= in_vld;
	reg_vld_prop[DELAY-1:1] <= reg_vld_prop[DELAY-2:0];

	//Pipeline inputs
	reg_d <= in_d;
	reg_x <= in_x;

	//Compute D * Xi
	reg_dx <= dx;
	reg_x_prop[0] <= reg_x;

	//Compute 2 - D * Xi
	reg_2_dx <= 2_dx;
	reg_x_prop[1] <= reg_x_prop[0];

	//Compute Xi * (2 - D * Xi)
	reg_res <= res;
end

//Intermediate operations
assign dx = reg_d * reg_x;
assign 2_dx = (2 << DECIMAL) - reg_dx[PRECISION+DECIMAL-1:DECIMAL];
assign res = reg_x_prop[1] * reg_2_dx[PRECISION+DECIMAL-1:DECIMAL];

//Ouptut
assign out_r = reg_res;
assign out_vld = reg_vld_prop[DELAY-1];

endmodule
