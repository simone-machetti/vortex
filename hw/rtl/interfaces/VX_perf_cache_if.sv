`ifndef VX_PERF_CACHE_IF
`define VX_PERF_CACHE_IF

`include "VX_define.vh"

interface VX_perf_cache_if ();

    logic [`PERF_CTR_BITS-1:0] reads;
    logic [`PERF_CTR_BITS-1:0] writes;
    logic [`PERF_CTR_BITS-1:0] read_misses;
    logic [`PERF_CTR_BITS-1:0] write_misses;
    logic [`PERF_CTR_BITS-1:0] bank_stalls;
    logic [`PERF_CTR_BITS-1:0] mshr_stalls;
    logic [`PERF_CTR_BITS-1:0] mem_stalls;
    logic [`PERF_CTR_BITS-1:0] crsp_stalls;

    modport master (
        output reads,
        output writes,
        output read_misses,
        output write_misses,
        output bank_stalls,
        output mshr_stalls,
        output mem_stalls,
        output crsp_stalls
    );

    modport slave (
        input reads,
        input writes,
        input read_misses,
        input write_misses,
        input bank_stalls,
        input mshr_stalls,
        input mem_stalls,
        input crsp_stalls
    );

endinterface

`endif
