module tb_vga #(
	parameter WIDTH = 800,
	parameter HEIGHT = 600
);
	initial begin
		$display("VGA Simulation");
		$finish;
	end
endmodule
