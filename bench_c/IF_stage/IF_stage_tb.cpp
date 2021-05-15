#include "systemc.h"
#include "../../rtl_c/IF_stage.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal<bool>                  rst;
   sc_signal<bool>                  instruction_fetch_en;
   sc_signal<bool>                  branch_taken;
   sc_signal< sc_int<6> >          branch_offset_imm;	
   sc_signal< sc_uint<PC_WIDTH> >   pc;  
   sc_signal< sc_uint<16> >         instruction;

   //=========================================================================
   // DUT
   //=========================================================================
   IF_stage IF_stage_inst("IF_stage_inst");
   IF_stage_inst.clk(clk);
   IF_stage_inst.rst(rst);
   IF_stage_inst.instruction_fetch_en(instruction_fetch_en);
   IF_stage_inst.branch_taken(branch_taken);
   IF_stage_inst.branch_offset_imm(branch_offset_imm);
   IF_stage_inst.pc(pc);
   IF_stage_inst.instruction(instruction);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("IF_stage");
   wf->set_time_unit(1, SC_PS);
   IF_stage_inst.wf = wf;
   IF_stage_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   rst = true;
   instruction_fetch_en = false;
   branch_taken = false;
   branch_offset_imm = 0;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Pull down reset" << endl;
   rst = false;
   sc_start(1, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Instruction fetch enable" << endl;
   instruction_fetch_en = true;
   
   for(int i=0; i<10; i++){
      sc_start(1, SC_NS);
      cout << "@" << sc_time_stamp() << ": " << "Program counter: " << pc << " Instruction: " << instruction << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Doing reset" << endl;
   rst = true;
   sc_start(1, SC_NS);
   rst = false;

   for(int i=0; i<3; i++){
      sc_start(1, SC_NS);
      cout << "@" << sc_time_stamp() << ": " << "Program counter: " << pc << " Instruction: " << instruction << endl;
   }
   branch_taken = true;
   branch_offset_imm = 3;
   sc_start(1, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Branch taken: " << branch_offset_imm << endl;
   cout << "@" << sc_time_stamp() << ": " << "Program counter: " << pc << " Instruction: " << instruction << endl;

   branch_taken = false;
   branch_offset_imm = -4;
   for(int i=0; i<2; i++){
      sc_start(1, SC_NS);
      cout << "@" << sc_time_stamp() << ": " << "Program counter: " << pc << " Instruction: " << instruction << endl;
   }

   branch_taken = true;
   branch_offset_imm = -4;
   sc_start(1, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Branch taken: " << branch_offset_imm << endl;
   cout << "@" << sc_time_stamp() << ": " << "Program counter: " << pc << " Instruction: " << instruction << endl;
 
   cout << "@" << sc_time_stamp() << ": " << "End of simulation" << endl;

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
