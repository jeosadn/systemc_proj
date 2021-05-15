#include "systemc.h"
#include "mips_16_defs.h"
#include "instruction_mem.cpp"

SC_MODULE(IF_stage) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk            clk;
   sc_in<bool>          rst;
   sc_in<bool>          instruction_fetch_en;
   sc_in<bool>          branch_taken;
   sc_in< sc_int<6> >   branch_offset_imm;      

   //Outputs
   sc_out< sc_uint<PC_WIDTH> >  pc;  
   sc_out< sc_uint<16> >        instruction;

   //=========================================================================
   // Internal
   //=========================================================================
   //Methods
   void increment_pc() {
      sc_uint<PC_WIDTH> temp_pc = 0;
      while (true) {
         if (rst.read()) {
            temp_pc = 0;
         } else {
            if (instruction_fetch_en.read()) {
               if (branch_taken.read()) {
                  temp_pc = temp_pc + branch_offset_imm.read();
               } else {
                  temp_pc = temp_pc + 1;
               }
            }
         }
         pc.write(temp_pc);
         wait();
      }
   }

   //Submodules
   instruction_mem imem;

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(IF_stage)
   : clk("clk")
   , rst("rst")
   , instruction_fetch_en("instruction_fetch_en")
   , branch_taken("branch_taken")
   , branch_offset_imm("branch_offset_imm")
   , pc("pc")
   , instruction("instruction")
   , imem("imem")
   {
      SC_CTHREAD(increment_pc, clk.pos());
         sensitive << rst.pos();

      imem.clk(clk);
      imem.pc(pc);
      imem.instruction(instruction);
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, rst, rst.name());
      sc_trace(wf, instruction_fetch_en, instruction_fetch_en.name());
      sc_trace(wf, branch_taken, branch_taken.name());
      sc_trace(wf, branch_offset_imm, branch_offset_imm.name());
      sc_trace(wf, pc, pc.name());
      sc_trace(wf, instruction, instruction.name());

      imem.wf = wf;
      imem.trace_me();
   }
};
