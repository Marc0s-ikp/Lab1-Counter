// mandatory includes
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {

    int i;   // counts the number of clock cycles to simulate 
    int clk; // clock signal
    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    // Turn on signal tracing and tell verilog to dump the waveform to a file
    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp ->open("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation
    for (i=0; i<300; i++){

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump(2*i+clk); //unit is in time step
            top->clk = !top->clk; //toggle clock
            top->eval(); //evaluate model
        }
        top->rst = (i<2) | (i==15); //assert reset for 2 cycles at start and at cycle 15
        top->en = (i>=4);
        if (Verilated::gotFinish())  exit(0);
    }
    tfp ->close();
    exit(0);
}