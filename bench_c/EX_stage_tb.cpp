#include "systemc.h"
#include "../rtl_c/EX_stage.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal<bool>          rst;
   sc_signal< sc_uint<57> > pipeline_reg_in;
   sc_signal< sc_uint<38> > pipeline_reg_out;
   sc_signal< sc_uint<3> >  ex_op_dest;

   //Variables
   sc_uint<57> pipeline_reg_in_temp;
   sc_uint<38> pipeline_reg_out_temp;


   //=========================================================================
   // DUT
   //=========================================================================
   EX_stage EX_stage_inst("EX_stage_inst");
   EX_stage_inst.clk(clk);
   EX_stage_inst.rst(rst);
   EX_stage_inst.pipeline_reg_in(pipeline_reg_in);
   EX_stage_inst.pipeline_reg_out(pipeline_reg_out);
   EX_stage_inst.ex_op_dest(ex_op_dest);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("EX_stage");
   wf->set_time_unit(1, SC_PS);
   EX_stage_inst.wf = wf;
   EX_stage_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================

   // SIMULATION START
   sc_start(0, SC_NS);
   // STEP 0
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   rst = true;
   pipeline_reg_in = 0;
   sc_start(10, SC_NS);

   rst = false;

   //Testing ADD operation
   pipeline_reg_in_temp.range(56,54)  = ALU_ADD;
   pipeline_reg_in_temp.range(53,38)  = 0x2;
   pipeline_reg_in_temp.range(37,22)  = 0x3;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x5) {
      cout << "@" << sc_time_stamp() << ": " << "ALU ADD OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU ADD ERROR" << endl;
   }

   //Testing SUB operation
   pipeline_reg_in_temp.range(56,54)  = ALU_SUB;
   pipeline_reg_in_temp.range(53,38)  = 0x7;
   pipeline_reg_in_temp.range(37,22)  = 0x4;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x3) {
      cout << "@" << sc_time_stamp() << ": " << "ALU SUB OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU SUB ERROR" << endl;
   }

   //Testing AND operation
   pipeline_reg_in_temp.range(56,54)  = ALU_AND;
   pipeline_reg_in_temp.range(53,38)  = 0x7;
   pipeline_reg_in_temp.range(37,22)  = 0x4;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x4) {
      cout << "@" << sc_time_stamp() << ": " << "ALU AND OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU AND ERROR" << endl;
   }

   //Testing OR operation
   pipeline_reg_in_temp.range(56,54)  = ALU_OR;
   pipeline_reg_in_temp.range(53,38)  = 0x3;
   pipeline_reg_in_temp.range(37,22)  = 0x4;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x7) {
      cout << "@" << sc_time_stamp() << ": " << "ALU OR OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU OR ERROR" << endl;
   }

   //Testing XOR operation
   pipeline_reg_in_temp.range(56,54)  = ALU_XOR;
   pipeline_reg_in_temp.range(53,38)  = 0x3;
   pipeline_reg_in_temp.range(37,22)  = 0x6;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x5) {
      cout << "@" << sc_time_stamp() << ": " << "ALU XOR OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU XOR ERROR" << endl;
   }

   //Testing SL operation
   pipeline_reg_in_temp.range(56,54)  = ALU_SL;
   pipeline_reg_in_temp.range(53,38)  = 0x1;
   pipeline_reg_in_temp.range(37,22)  = 0x6;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x40) {
      cout << "@" << sc_time_stamp() << ": " << "ALU SL OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU SL ERROR" << endl;
   }

   //Testing SR operation
   pipeline_reg_in_temp.range(56,54)  = ALU_SR;
   pipeline_reg_in_temp.range(53,38)  = 0x40;
   pipeline_reg_in_temp.range(37,22)  = 0x6;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x1) {
      cout << "@" << sc_time_stamp() << ": " << "ALU SR OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU SR ERROR" << endl;
   }

   //Testing SR operation
   pipeline_reg_in_temp.range(56,54)  = ALU_SR;
   pipeline_reg_in_temp.range(53,38)  = 0x40;
   pipeline_reg_in_temp.range(37,22)  = 0x6;
   pipeline_reg_in = pipeline_reg_in_temp;
   sc_start(3, SC_NS);
   if (pipeline_reg_out.read().range(37,22) == 0x1) {
      cout << "@" << sc_time_stamp() << ": " << "ALU SRU OK" << endl;
   } else {
      cout << pipeline_reg_out.read().range(37,22) << endl;
      cout << "@" << sc_time_stamp() << ": " << "ALU SRU ERROR" << endl;
   }

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
