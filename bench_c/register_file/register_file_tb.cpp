#include <systemc.h>
#include "../../rtl_c/register_file.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================

   // Inputs
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal<bool>           rst;
   sc_signal<bool>           reg_write_en;
   sc_signal< sc_uint<3> >   reg_write_dest;
   sc_signal< sc_uint<16> >  reg_write_data;
   sc_signal< sc_uint<3> >   reg_read_addr_1;
   sc_signal< sc_uint<3> >   reg_read_addr_2;

   //Outputs
   sc_signal < sc_uint<16> > reg_read_data_1;
   sc_signal < sc_uint<16> > reg_read_data_2;

   //=========================================================================
   // DUT
   //=========================================================================
   register_file register_file_inst("register_file_inst");
      register_file_inst.clk(clk);
      register_file_inst.rst(rst);
      register_file_inst.reg_write_en(reg_write_en);
      register_file_inst.reg_write_dest(reg_write_dest);
      register_file_inst.reg_write_data(reg_write_data);
      register_file_inst.reg_read_addr_1(reg_read_addr_1);
      register_file_inst.reg_read_addr_2(reg_read_addr_2);
      register_file_inst.reg_read_data_1(reg_read_data_1);
      register_file_inst.reg_read_data_2(reg_read_data_2);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("register_file");
   wf->set_time_unit(1, SC_PS);
   register_file_inst.wf = wf;
   register_file_inst.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   int value_to_test = rand() % 0XFFFF;
   int addr_to_test = rand() % 8;
   int expected_value = 0;

   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   reg_write_en    = false;
   reg_write_dest  = 0;
   reg_write_data  = 0;
   reg_read_addr_1 = 0;
   reg_read_addr_2 = 0;
   reg_read_data_1 = 0;
   reg_read_data_2 = 0;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Write to address " <<
   addr_to_test << " the value " << value_to_test << endl;
   reg_write_dest = addr_to_test;
   reg_write_data = value_to_test;
   reg_write_en   = true;

   // Verify that all the positions contain the expected value
   for (int i = 0; i < 8; i++) {
     reg_read_addr_1 = i;
     reg_read_addr_2 = i;
     sc_start(1, SC_NS);

     // Verify if the actual position contains the previously set value
     if(reg_write_dest.read() == i)
       expected_value = value_to_test;
     else
      expected_value = 0;

     if (register_file_inst.reg_read_data_1.read() == expected_value &&
         register_file_inst.reg_read_data_1.read() == expected_value)
        cout << "@" << sc_time_stamp() << ": " << "Read " << i << " OK" << endl;
     else
        cout << "@" << sc_time_stamp() << ": " << "Read " << i << " ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "End of simulation" << endl;

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
