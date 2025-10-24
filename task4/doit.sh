#!/bin/bash

~/Documents/iac/lab0-devtools/tools/attach_usb.sh

#cleanup
rm -rf obj_dir
rm -f counter.vcd

#compile and run
verilator -Wall --cc --trace counter.sv bin2bcd.sv top.sv --exe top_tb.cpp

make -j -C obj_dir/ -f Vtop.mk Vtop

obj_dir/Vtop