module top (
    input        clk      ,
    input        switch   ,
    output       led      ,
    output [3:0] vga_red  ,
    output [3:0] vga_green,
    output [3:0] vga_blue ,
    output       v_sync   ,
    output       h_sync
);

wire [3:0] mux_red   ;
wire [3:0] mux_green ;
wire [3:0] mux_blue  ;
wire       mux_h_sync;
wire       mux_v_sync;

wire [3:0] my_red   ;
wire [3:0] my_green ;
wire [3:0] my_blue  ;
wire       my_h_sync;
wire       my_v_sync;

wire       known_h_sync;
wire       known_v_sync;
wire       display_on;
wire       video_on;

assign vga_red   = switch ? (video_on ? 4'h0 : 4'h0) : (display_on ? my_red   : 4'h0);
assign vga_green = switch ? (video_on ? 4'hF : 4'h0) : (display_on ? my_green : 4'h0);
assign vga_blue  = switch ? (video_on ? 4'h0 : 4'h0) : (display_on ? my_blue  : 4'h0);
assign v_sync    = switch ? known_v_sync             : my_v_sync;
assign h_sync    = switch ? known_h_sync             : my_h_sync;

reg led_reg;
assign led = led_reg;

vga vga_inst (
     .clk       (clk       )
    ,.rst       (1'b0      )
    ,.red       (my_red    )
    ,.green     (my_green  )
    ,.blue      (my_blue   )
    ,.h_sync    (my_h_sync )
    ,.v_sync	(my_v_sync )
    ,.display_on(display_on)
);

vga_known_good vga_known_good_inst (
     .clk     (clk         )
    ,.reset   (1'b0        )
    ,.vsync   (known_v_sync)
    ,.hsync   (known_h_sync)
    ,.video_on(video_on    )
);

always @(posedge clk) begin
    led_reg <= switch;
end

endmodule
