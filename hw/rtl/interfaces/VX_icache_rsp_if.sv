`ifndef VX_ICACHE_CORE_RSP_IF
`define VX_ICACHE_CORE_RSP_IF

`include "../cache/VX_cache_define.vh"

interface VX_icache_rsp_if #(
    parameter WORD_SIZE = 1,
    parameter TAG_WIDTH = 1
) ();

    logic                   valid;
    logic [`WORD_WIDTH-1:0] data;
    logic [TAG_WIDTH-1:0]   tag;
    logic                   ready;

    modport master (
        output valid,
        output data,
        output tag,
        input  ready
    );

    modport slave (
        input  valid,
        input  data,
        input  tag,
        output ready
    );

endinterface

`endif
