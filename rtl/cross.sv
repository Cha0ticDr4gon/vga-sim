/*
Author: Oscar Balan <oab34@drexel.edu>
Date: 2024-10-09
Description:
	Pipelined cross-product component module with DSP48E1 architecture in mind.
	Can be modified or swapped out if synthesis produces sub-par DSP usage.
*/

//10 + 7 + 1 = 18, which is bitwidth of smaller multiplicand input for DSP48E1
//Signed fixed-point numbers
//11 bits integer (signed)
//7 bits decimal
//[1+10][7]
module cross_product #(
	parameter INTEGER   = 10,
	parameter DECIMAL   =  7,
	parameter PRECISION = 1 + INTEGER + DECIMAL
)(
	input                  clk    ,
	input                  in_vld ,
	input  [PRECISION-1:0] in_ax  ,
	input  [PRECISION-1:0] in_ay  ,
	input  [PRECISION-1:0] in_bx  ,
	input  [PRECISION-1:0] in_by  ,
	output [PRECISION-1:0] out_z  ,
	output                 out_vld
);

localparam DELAY = 3;
reg [DELAY-1:0] reg_vld_prop = 0;

//Pipeline inputs
reg signed [PRECISION-1:0] reg_ax;
reg signed [PRECISION-1:0] reg_ay;
reg signed [PRECISION-1:0] reg_bx;
reg signed [PRECISION-1:0] reg_by;

//Multiplication results
reg signed [PRECISION*2-1:0] reg_axby;
reg signed [PRECISION*2-1:0] reg_aybx;

//Subtraction results
reg signed [PRECISION*2-1:0] reg_axby_aybx;

//Multiplication stage
wire signed [PRECISION*2-1:0] axby;
wire signed [PRECISION*2-1:0] aybx;

//Subtraction stage
wire signed [PRECISION*2-1:0] axby_aybx;

always @(posedge clk) begin
	//Propogate valid signal for data
	reg_vld_prop[0] <= in_vld;
	reg_vld_prop[DELAY-1:1] <= reg_vld_prop[DELAY-2:0];

	//Input pipeline
	reg_ax <= in_ax;
	reg_ay <= in_ay;
	reg_bx <= in_bx;
	reg_by <= in_by;

	//Multiplication part
	reg_axby <= axby;
	reg_aybx <= aybx;

	//Subtraction part
	reg_axby_aybx <= axby_aybx;
end

//Multiply inputs
assign axby = reg_ax * reg_by;
assign aybx = reg_ay * reg_bx;

//Subtract intermediate results
assign axby_aybx = reg_axby - reg_aybx;

//Output propogation
assign out_z = reg_axby_aybx[PRECISION+DECIMAL-1:DECIMAL];
assign out_vld = reg_vld_prop[DELAY-1];

endmodule
