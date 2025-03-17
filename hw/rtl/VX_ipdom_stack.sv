`include "VX_platform.vh"

module VX_ipdom_stack #(
    parameter WIDTH = 1,
    parameter DEPTH = 1
) (
    input  logic               clk,
    input  logic               reset,
    input  logic               pair,
    input  logic [WIDTH - 1:0] q1,
    input  logic [WIDTH - 1:0] q2,
    output logic [WIDTH - 1:0] d,
    input  logic               push,
    input  logic               pop,
    output logic               index,
    output logic               empty,
    output logic               full
);
    localparam ADDRW = $clog2(DEPTH);

    logic curr_is_part [DEPTH-1:0];
    logic next_is_part [DEPTH-1:0];

    logic [ADDRW-1:0] curr_rd_ptr;
    logic [ADDRW-1:0] next_rd_ptr;
    logic [ADDRW-1:0] curr_wr_ptr;
    logic [ADDRW-1:0] next_wr_ptr;

    logic [WIDTH-1:0] d1;
    logic [WIDTH-1:0] d2;

    always_ff @(posedge clk) begin
        if (reset) begin
            curr_rd_ptr <= '0;
            curr_wr_ptr <= '0;
        end
        else begin
            curr_rd_ptr <= next_rd_ptr;
            curr_wr_ptr <= next_wr_ptr;
        end
    end

    always_comb begin
        if (push) begin
            next_rd_ptr = curr_wr_ptr;
            next_wr_ptr = curr_wr_ptr + {ADDRW{1}};
        end else if (pop) begin
            next_rd_ptr = curr_rd_ptr - {ADDRW{curr_is_part[curr_rd_ptr]}};
            next_wr_ptr = curr_wr_ptr - {ADDRW{curr_is_part[curr_rd_ptr]}};
        end
        else begin
            next_rd_ptr = curr_rd_ptr;
            next_wr_ptr = curr_wr_ptr;
        end
    end

    double_port_mem_wrapper #(
        .DATAW   (WIDTH * 2),
        .SIZE    (DEPTH),
        .OUT_REG (0)
    ) store_i (
        .clk_i   (clk),
        .rst_ni  (~reset),
        .wren_i  (push),
        .waddr_i (curr_wr_ptr),
        .wdata_i ({q2, q1}),
        .raddr_i (curr_rd_ptr),
        .rdata_o ({d2, d1})
    );

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < DEPTH; i++) begin
                curr_is_part[i] <= 1'b0;
            end
        end
        else begin
            curr_is_part <= next_is_part;
        end
    end

    always_comb begin
        next_is_part = curr_is_part;
        if (push) begin
            next_is_part[curr_wr_ptr] = ~pair;
        end
        if (pop) begin
            next_is_part[curr_rd_ptr] = 1;
        end
    end

    assign index = curr_is_part[curr_rd_ptr];
    assign d     = index ? d1 : d2;
    assign empty = (ADDRW'(0) == curr_wr_ptr);
    assign full  = (ADDRW'(DEPTH-1) == curr_wr_ptr);

endmodule
