#include "systemc.h"
#include "../../rtl_c/mips_16_core_top.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal<bool>          rst;

   sc_signal < sc_uint<8> > pc;

   //=========================================================================
   // DUT
   //=========================================================================
   mips_16_core_top mips_16_core_top_inst("mips_16_core_top_inst");
      mips_16_core_top_inst.clk(clk);
      mips_16_core_top_inst.rst(rst);
      mips_16_core_top_inst.pc(pc);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("mips_16_core_top");
   wf->set_time_unit(1, SC_PS);
   mips_16_core_top_inst.wf = wf;
   mips_16_core_top_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   rst = true;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Pull down reset" << endl;
   rst = false;
   //mips_16_core_top_inst.register_file_inst.reg_array[4] = 7;
   //mips_16_core_top_inst.register_file_inst.reg_array[5] = 3;
   sc_start(60, SC_NS);

   //cout << mips_16_core_top_inst.register_file_inst.reg_array[0] << endl;
   //cout << mips_16_core_top_inst.register_file_inst.reg_array[1] << endl;
   cout << "@" << sc_time_stamp() << ": " << "End of test" << endl;

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
