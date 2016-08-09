#include "VDutCore.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <vector>
#include <stdio.h>

namespace DutCore {
        struct Memory {
                std::vector<uint32_t> instruction_memory;
                std::vector<uint32_t> data_memory;
        };

        void simulate(VDutCore* core, Memory m, const size_t N, VerilatedVcdC* tfp) {
                core->clk = 0;
                for (size_t i = 0; i < N; i++) {
                        for (int clk = 0; clk < 2; clk++) {
                                tfp->dump(2*i+clk);
                                core->clk = !core->clk;
                        }
                        uint32_t iaddr = core->imem_in_req_addr;
                        core->eval();
                }
        }
}


int main(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        VDutCore* core = new VDutCore("Core");
        Verilated::traceEverOn(true);
        VerilatedVcdC* tfp = new VerilatedVcdC;

        core->trace(tfp, 99);
        tfp->open("core.vcd");

        std::vector<uint32_t> instruction_memory;
        std::vector<uint32_t> data_memory;
        DutCore::Memory m = {
                instruction_memory,
                data_memory
        };

        simulate(core, m, 1000, tfp);
        tfp->close();
}
