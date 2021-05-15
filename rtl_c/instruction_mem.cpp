#include "systemc.h"

SC_MODULE (instruction_mem) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             	clk; //asynchronized!!
   sc_in< sc_uint<PC_WIDTH> >	pc;
 
   //Outputs
   sc_out< sc_uint<16> >	instruction;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_uint<16> rom[INSTR_MEM_ADDR_WIDTH];

   //Methods
   void get_instruction() {
      instruction.write(rom[pc.read()]);
   }


   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(instruction_mem)
   : clk("clk")
   , pc("pc")
   , instruction("instruction")
   {
      #include "../bench_c/rom.cpp"
      SC_METHOD(get_instruction);
      sensitive << pc;
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, pc, pc.name());
      sc_trace(wf, instruction, instruction.name());
   }

};
