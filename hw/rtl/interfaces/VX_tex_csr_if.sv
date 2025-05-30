`ifndef VX_TEX_CSR_IF
`define VX_TEX_CSR_IF

`include "VX_define.vh"

interface VX_tex_csr_if ();

    logic                      write_enable;
    logic [`CSR_ADDR_BITS-1:0] write_addr;
    logic [31:0]               write_data;
    logic [`UUID_BITS-1:0]     write_uuid;

    modport master (
        output write_enable,
        output write_addr,
        output write_data,
        output write_uuid
    );

    modport slave (
        input write_enable,
        input write_addr,
        input write_data,
        input write_uuid
    );

endinterface

`endif
