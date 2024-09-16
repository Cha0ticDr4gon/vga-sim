module vga #(
	parameter DEFAULT_RED   = 4'hF,
	parameter DEFAULT_GREEN = 4'h0,
	parameter DEFAULT_BLUE  = 4'h0
) (
	input        clk       ,
	input        rst       ,
	output [3:0] red       ,
	output [3:0] green     ,
	output [3:0] blue      ,
	output       h_sync    ,
	output       v_sync    ,
	output       display_on
);

//Total width  = 800 pixels
//Total height = 525 pixels
localparam SCREEN_WIDTH = 640;
localparam LEFT_BORDER  = 48;
localparam RIGHT_BORDER = 16;
localparam H_RETRACE    = 96;
localparam TOTAL_WIDTH  = LEFT_BORDER + SCREEN_WIDTH + RIGHT_BORDER + H_RETRACE;

localparam SCREEN_HEIGHT = 480;
localparam TOP_BORDER    = 33;
localparam BOTTOM_BORDER = 10;
localparam V_RETRACE     = 2;
localparam TOTAL_HEIGHT  = TOP_BORDER + SCREEN_HEIGHT + BOTTOM_BORDER + V_RETRACE;

//Drive outputs
reg [3:0] red_reg;
reg [3:0] green_reg;
reg [3:0] blue_reg;
reg h_sync_reg;
reg v_sync_reg;
reg display_on_reg;

assign red   = red_reg;
assign green = green_reg;
assign blue  = blue_reg;
//h_sync and v_sync active low
assign h_sync = ~h_sync_reg;
assign v_sync = ~v_sync_reg;
assign display_on = display_on_reg;

//VGA clock 640x480@60Hz is 25.175MHz
//+/-0.5% accuracy lets 25MHz clock work
//Input clock expected to be 100MHz, so 25MHz achieved by clk mod 4
reg [1:0] vga_clk_counter;
wire vga_clk;

//Asserts once every 4 clk cycles to achieve 25MHz
assign vga_clk = vga_clk_counter == 2'b11;

//Regions where h-sync and v-sync signals should assert
localparam H_SYNC_START = SCREEN_WIDTH + RIGHT_BORDER;
localparam H_SYNC_END   = H_SYNC_START + H_RETRACE;
localparam V_SYNC_START = SCREEN_HEIGHT + TOP_BORDER;
localparam V_SYNC_END   = V_SYNC_START + V_RETRACE;

reg [9:0] h_counter;
reg [9:0] v_counter;

initial begin
	//Ouptut signals
    red_reg <= DEFAULT_RED;
    green_reg <= DEFAULT_GREEN;
    blue_reg <= DEFAULT_BLUE;
	h_sync_reg <= 0;
	v_sync_reg <= 0;
	display_on_reg <= 0;
	//Counters
    vga_clk_counter <= 0;
	h_counter <= 0;
	v_counter <= 0;
end

always @(posedge clk) begin
    if(rst) begin
		//Ouptut signals
		red_reg <= DEFAULT_RED;
		green_reg <= DEFAULT_GREEN;
		blue_reg <= DEFAULT_BLUE;
		h_sync_reg <= 0;
		v_sync_reg <= 0;
		display_on_reg <= 0;
		//Counters
		vga_clk_counter <= 0;
		h_counter <= 0;
		v_counter <= 0;
    end
    else begin
        vga_clk_counter <= vga_clk_counter + 1;
        if(vga_clk) begin
			//Horizontal and vertical counters for display area
			if(h_counter < TOTAL_WIDTH - 1) begin
				h_counter <= h_counter + 1;
			end
			else begin
				h_counter <= 0;
				if(v_counter < TOTAL_HEIGHT - 1) begin
					v_counter <= v_counter + 1;
				end
				else begin
					v_counter <= 0;
				end
			end
			//Horizontal and vertical sync activation
			h_sync_reg <= (h_counter >= H_SYNC_START) && (h_counter < H_SYNC_END);
			v_sync_reg <= (v_counter >= V_SYNC_START) && (v_counter < V_SYNC_END);
			//Display on, starts at zero for each
			display_on_reg <= (h_counter < SCREEN_WIDTH) && (v_counter < SCREEN_HEIGHT);
        end
    end
end

endmodule
