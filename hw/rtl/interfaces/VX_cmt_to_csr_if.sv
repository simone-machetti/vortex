`ifndef VX_CMT_TO_CSR_IF
`define VX_CMT_TO_CSR_IF

`include "VX_define.vh"

interface VX_cmt_to_csr_if ();

    logic                                valid;
`ifdef EXT_F_ENABLE
    logic [$clog2(6*`NUM_THREADS+1)-1:0] commit_size;
`else
    logic [$clog2(5*`NUM_THREADS+1)-1:0] commit_size;
`endif
    modport master (
        output valid,
        output commit_size
    );

    modport slave (
        input valid,
        input commit_size
    );

endinterface

`endif
