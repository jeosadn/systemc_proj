#include <systemc.h>
#include "../../rtl_c/WB_stage.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================

   // Inputs
   sc_signal< sc_uint<37> > pipeline_reg_in;

   //Outputs
   sc_signal <bool> reg_write_en;
   sc_signal < sc_uint<3> > reg_write_dest;
   sc_signal < sc_uint<16> > reg_write_data;
   sc_signal < sc_uint<3> > wb_op_dest;

   //=========================================================================
   // DUT
   //=========================================================================
   WB_stage WB_stage_inst("WB_stage_inst");
      WB_stage_inst.pipeline_reg_in(pipeline_reg_in);
      WB_stage_inst.reg_write_en(reg_write_en);
      WB_stage_inst.reg_write_dest(reg_write_dest);
      WB_stage_inst.reg_write_data(reg_write_data);
      WB_stage_inst.wb_op_dest(wb_op_dest);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("WB_stage");
   wf->set_time_unit(1, SC_PS);
   WB_stage_inst.wf = wf;
   WB_stage_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   double value_to_test = rand();

   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   reg_write_en    = false;
   pipeline_reg_in = 0;
   reg_write_dest  = 0;
   reg_write_data  = 0;
   wb_op_dest      = 0;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Write a value of " <<
   value_to_test << " and verify the results" << endl;

   pipeline_reg_in.write(value_to_test);
   sc_start(1, SC_NS);

   if(reg_write_en.read() == pipeline_reg_in.read().range(4, 4))
    cout << "reg_write_en OK" << endl;
   else
    cout << "reg_write_en ERROR" << endl;

   if(reg_write_dest.read() == pipeline_reg_in.read().range(3, 1))
    cout << "reg_write_dest OK" << endl;
   else
    cout << "reg_write_dest ERROR" << endl;

    if(pipeline_reg_in.read().range(0, 0)){
      if(reg_write_data.read() == pipeline_reg_in.read().range(20, 5))
        cout << "reg_write_data OK" << endl;
      else
        cout << "reg_write_data ERROR" << endl;
    }else{
      if(reg_write_data.read() == pipeline_reg_in.read().range(36, 21))
        cout << "reg_write_data OK" << endl;
      else
        cout << "reg_write_data ERROR" << endl;
    }

    if(wb_op_dest.read() == pipeline_reg_in.read().range(3, 1))
     cout << "wb_op_dest OK" << endl;
    else
     cout << "wb_op_dest ERROR" << endl;

   cout << "@" << sc_time_stamp() << ": " << "End of simulation" << endl;

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
