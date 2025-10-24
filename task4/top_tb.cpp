// mandatory includes
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;   // counts the number of clock cycles to simulate 
    int clk; // clock signal
    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop* top = new Vtop;
    // init trace dump
    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");
    vbdSetMode(1);  // Set to ONE-SHOT mode

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    top->v = 0;

    // run simulation
    for (i=0; i<300; i++){
        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }

        // Vbuddy output: display BCD value
        vbdHex(4, (top->bcd >> 8) & 0xF);  // hundreds digit
        vbdHex(3, (top->bcd >> 4) & 0xF);  // tens digit
        vbdHex(1, top->bcd & 0xF);         // ones digit
        vbdPlot(int(top->bcd), 0, 255);    // plot BCD value
        vbdCycle(i+1);

        // Assertion of control signals
        top->rst = (i<2) | (i==15);        // reset at start and cycle 15
        
        // Check if button is pressed
        if (vbdFlag()) {  // ONE-SHOT mode will auto-clear the flag
            if (vbdValue() > 0) {  // If rotary value > 0, load that value
                top->v = vbdValue();
                top->en = 0;       // disable counting
            } else {  // If rotary value = 0, enable counting
                top->en = 1;       // enable counting for one cycle
            }
        } else {
            top->en = 0;           // disable counting when no button press
        }

        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();     // close Vbuddy
    tfp->close();   // close dump file
    exit(0);
}