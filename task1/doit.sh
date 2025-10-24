#!/bin/bash

#cleanup
rm -rf obj_dir
rm -f counter.vcd

#compile and run
verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp

make -j -C obj_dir/ -f Vcounter.mk Vcounter

obj_dir/Vcounter