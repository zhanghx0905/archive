`timescale 1ns / 1ps
module sram_io(
           input wire clk,
           input wire rst,
           input wire oen,
           input wire wen,
           input wire[31:0] data_in,
           output reg[31:0] data_out,
           output wire done,

           inout wire[31:0] base_ram_data_wire,

           output reg base_ram_ce_n,
           output reg base_ram_oe_n,
           output reg base_ram_we_n
       );

reg data_z;
assign base_ram_data_wire = data_z ? 32'bz : data_in;

localparam STATE_IDLE    = 3'b000;
localparam STATE_READ_0  = 3'b001;
localparam STATE_READ_1  = 3'b010;
localparam STATE_WRITE_0 = 3'b100;
localparam STATE_WRITE_1 = 3'b101;
localparam STATE_DONE    = 3'b111;

reg[2:0] state;
assign done = state == STATE_DONE;

always @(posedge clk or posedge rst) begin
    if (rst) begin
        state <= STATE_IDLE;
        base_ram_ce_n <= 1'b1;
        base_ram_we_n <= 1'b1;
        base_ram_oe_n <= 1'b1;
        data_z <= 1'b1;
    end
    else begin
        case (state)
            STATE_IDLE: begin
                if (~oen) begin
                    data_z <= 1'b1;
                    state <= STATE_READ_0;
                end
                else if (~wen) begin
                    data_z <= 1'b0;
                    state <= STATE_WRITE_0;
                end
            end
            STATE_READ_0: begin
                base_ram_ce_n <= 1'b0;
                base_ram_oe_n <= 1'b0;
                state <= STATE_READ_1;
            end
            STATE_READ_1: begin
                base_ram_ce_n <= 1'b1;
                base_ram_oe_n <= 1'b1;
                data_out <= base_ram_data_wire;
                state <= STATE_DONE;
            end
            STATE_WRITE_0: begin
                base_ram_ce_n <= 1'b0;
                base_ram_we_n <= 1'b0;
                state <= STATE_WRITE_1;
            end
            STATE_WRITE_1: begin
                base_ram_ce_n <= 1'b1;
                base_ram_we_n <= 1'b1;
                state <= STATE_DONE;
            end
            STATE_DONE: begin
                data_z <= 1'b1;
                if (oen & wen) begin
                    state <= STATE_IDLE;
                end
            end
        endcase
    end
end
endmodule
