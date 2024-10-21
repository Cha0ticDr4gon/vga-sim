module render #(
	parameter WIDTH     = 800,
	parameter HEIGHT    = 600,
	parameter INTEGER   =  10,
	parameter DECIMAL   =   7,
	parameter PRECISION = 1 + INTEGER + DECIMAL
)(
	input                  clk    ,
	input                  in_vld ,
	input  [PRECISION-1:0] in_x1  ,
	input  [PRECISION-1:0] in_y1  ,
	input  [PRECISION-1:0] in_x2  ,
	input  [PRECISION-1:0] in_y2  ,
	input  [PRECISION-1:0] in_x3  ,
	input  [PRECISION-1:0] in_y3  ,
	output                 out_vld,
	output                 out_red,
	output                 out_green,
	output                 out_blue
);

//Pipeline inputs
reg [PRECISION-1:0] reg_x1;
reg [PRECISION-1:0] reg_y1;
reg [PRECISION-1:0] reg_x2;
reg [PRECISION-1:0] reg_y2;
reg [PRECISION-1:0] reg_x3;
reg [PRECISION-1:0] reg_y3;

//Pixel counter (for now)
reg [PRECISION-1:0] px = 0;
reg [PRECISION-1:0] py = 0;

//Rays for triangle centered around (x1, y1)
wire [PRECISION-1:0] rx1;
wire [PRECISION-1:0] ry1;
wire [PRECISION-1:0] rx2;
wire [PRECISION-1:0] ry2;

//Ray to pixel centered around (x1, y1)
wire [PRECISION-1:0] prx;
wire [PRECISION-1:0] pry;

//Cross product results
wire signed [PRECISION-1:0] c1;
wire signed [PRECISION-1:0] c2;

//Modules for doing cross product
cross_product #(
	.INTEGER(INTEGER),
	.DECIMAL(DECIMAL)
) cross_product_inst (
	.clk(clk),
);

assign rx1 = x2 - x1;
assign ry1 = y2 - y1;
assign rx2 = x3 - x1;
assign ry2 = y3 - y1;

assign prx = px - x1;
assign pry = py - y1;

assign c1 = ((pry * rx1) >> DECIMAL) - ((prx * ry1) >> DECIMAL);
assign c2 = ((pry * rx2) >> DECIMAL) - ((prx * ry2) >> DECIMAL);

always @(posedge clk) begin
	if(rst) begin
		px <= 0;
		py <= 0;
	end
	else begin
		//Increment pixels
		if(px >= WIDTH) begin
			px <= 0;
			if(py >= HEIGHT) py <= 0;
			else py <= py + 1;
		end
		else begin
			px <= px + 1;
		end
	end
end

endmodule
