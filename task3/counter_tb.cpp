// mandatory includes
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

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

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");
    vbdSetMode(1);  // Set to ONE-SHOT mode

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    top->v = 0;

    // run simulation
    for (i=0; i<300; i++){

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump(2*i+clk); //unit is in time step
            top->clk = !top->clk; //toggle clock
            top->eval(); //evaluate model
        }

        // Vbuddy output
        vbdHex(4, (init(top->count) >> 16) & 0xF); // upper 4 bits
        vbdHex(3, (init(top->count) >> 8) & 0xF);
        vbdHex(2, (init(top->count) >> 4) & 0xF);
        vbdHex(1, (init(top->count) >> 0) & 0xF); // lower 4 bits
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);
        // end of Vbuddy output

        top->rst = (i<2) | (i==15); //assert reset for 2 cycles at start and at cycle 15
        
        // Check if button is pressed
        if (vbdFlag()) {  // ONE-SHOT mode will auto-clear the flag
            if (vbdValue() > 0) {  // If rotary value > 0, treat as load mode
                top->ld = 1;   // Enable loading
                top->v = vbdValue(); // Get value from Vbuddy rotary encoder
            } else {  // If rotary value = 0, increment counter
                top->ld = 0;   // Disable loading
                top->v = 1;    // Signal to increment
            }
        } else {
            top->ld = 0;   // Disable loading
            top->v = 0;    // No increment
        }
        if (Verilated::gotFinish())  exit(0);
    }
    vbdClose();
    tfp ->close();
    exit(0);
}