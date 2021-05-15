#include "systemc.h"
#include "../../rtl_c/MEM_stage.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal< sc_uint<38> > pipeline_reg_in;
   sc_signal< sc_uint<37> > pipeline_reg_out;
   sc_signal< sc_uint<3> >  mem_op_dest;
   sc_signal<bool>          rst;

   sc_uint<38> pipeline_reg_in_temp;

   //=========================================================================
   // DUT
   //=========================================================================
   MEM_stage MEM_stage_inst("MEM_stage_inst");
      MEM_stage_inst.clk(clk);
      MEM_stage_inst.rst(rst);
      MEM_stage_inst.pipeline_reg_in(pipeline_reg_in);
      MEM_stage_inst.pipeline_reg_out(pipeline_reg_out);
      MEM_stage_inst.mem_op_dest(mem_op_dest);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("MEM_stage");
   wf->set_time_unit(1, SC_PS);
   MEM_stage_inst.wf = wf;
   MEM_stage_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   rst = true;
   pipeline_reg_in = 0;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Pull down reset" << endl;
   rst = false;
   sc_start(1, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Write to address 0x13 the value 0xCAFE" << endl;
   pipeline_reg_in_temp.range(37,22) = 0x13;
   pipeline_reg_in_temp.range(20,5)  = 0xCAFE;
   pipeline_reg_in_temp.range(21,21) = 1;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(1, SC_NS);
   pipeline_reg_in = 0;
   sc_start(1, SC_NS);
   if (MEM_stage_inst.dmem.memory[0x13] == sc_uint<16> (0xCAFE)) {
      cout << "@" << sc_time_stamp() << ": " << "Write OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Write ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Write to address 0x16 the value 0xABBA" << endl;
   pipeline_reg_in_temp.range(37,22) = 0x16;
   pipeline_reg_in_temp.range(20,5)  = 0xABBA;
   pipeline_reg_in_temp.range(21,21) = 1;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(1, SC_NS);
   pipeline_reg_in = 0;
   sc_start(1, SC_NS);
   if (MEM_stage_inst.dmem.memory[0x16] == sc_uint<16> (0xABBA)) {
      cout << "@" << sc_time_stamp() << ": " << "Write OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Write ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Backdoor write to address 0x20 the value 0xDECA" << endl;
   MEM_stage_inst.dmem.memory[0x20] = 0xDECA;
   cout << "@" << sc_time_stamp() << ": " << "Read address 0x20" << endl;
   pipeline_reg_in_temp.range(37,22) = 0x20;
   pipeline_reg_in_temp.range(20,5)  = 0x0;
   pipeline_reg_in_temp.range(21,21) = 0;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(1, SC_NS);
   pipeline_reg_in = 0;
   sc_start(1, SC_NS);
   if (pipeline_reg_out.read().range(20,5) == 0xDECA) {
      cout << "@" << sc_time_stamp() << ": " << "Read OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Read ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Backdoor write to address 0x40 the value 0xFACE" << endl;
   MEM_stage_inst.dmem.memory[0x40] = 0xFACE;
   cout << "@" << sc_time_stamp() << ": " << "Read address 0x20" << endl;
   pipeline_reg_in_temp.range(37,22) = 0x40;
   pipeline_reg_in_temp.range(20,5)  = 0x0;
   pipeline_reg_in_temp.range(21,21) = 0;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(1, SC_NS);
   pipeline_reg_in = 0;
   sc_start(1, SC_NS);
   if (pipeline_reg_out.read().range(20,5) == 0xFACE) {
      cout << "@" << sc_time_stamp() << ": " << "Read OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Read ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "End of simulation" << endl;

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
