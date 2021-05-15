#include <systemc.h>
#include "../../rtl_c/data_mem.cpp"

int sc_main(int argc, char* argv[]) {
   //=========================================================================
   // Signals
   //=========================================================================
   sc_clock clk("clk", 1, SC_NS, 0.5);
   sc_signal< sc_uint<16> > mem_access_addr;
   sc_signal< sc_uint<16> > mem_write_data;
   sc_signal<bool>          mem_write_en;
   sc_signal< sc_uint<16> > mem_read_data;

   //=========================================================================
   // DUT
   //=========================================================================
   data_mem dmem("dmem");
      dmem.clk(clk);
      dmem.mem_access_addr(mem_access_addr);
      dmem.mem_write_data(mem_write_data);
      dmem.mem_write_en(mem_write_en);
      dmem.mem_read_data(mem_read_data);

   //=========================================================================
   // VCD
   //=========================================================================
   sc_trace_file *wf = sc_create_vcd_trace_file("data_mem");
   wf->set_time_unit(1, SC_PS);
   dmem.wf = wf;
   dmem.trace_me();

   //=========================================================================
   // Stimulus
   //=========================================================================
   cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
   sc_start(0, SC_NS);
   cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
   mem_access_addr = 0;
   mem_write_data = 0;
   mem_write_en = false;
   sc_start(10, SC_NS);

   cout << "@" << sc_time_stamp() << ": " << "Write to address 0x13 the value 0xCAFE" << endl;
   mem_access_addr = 0x13;
   mem_write_data = 0xCAFE;
   mem_write_en = true;
   sc_start(1, SC_NS);
   if (dmem.memory[0x13] == sc_uint<16> (0xCAFE)) {
      cout << "@" << sc_time_stamp() << ": " << "Write OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Write ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Write to address 0x16 the value 0xABBA" << endl;
   mem_access_addr = 0x16;
   mem_write_data = 0xABBA;
   mem_write_en = true;
   sc_start(1, SC_NS);
   if (dmem.memory[0x16] == sc_uint<16> (0xABBA)) {
      cout << "@" << sc_time_stamp() << ": " << "Write OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Write ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Backdoor write to address 0x20 the value 0xDECA" << endl;
   dmem.memory[0x20] = 0xDECA;
   cout << "@" << sc_time_stamp() << ": " << "Read address 0x20" << endl;
   mem_access_addr = 0x20;
   mem_write_data = 0x0;
   mem_write_en = false;
   sc_start(1, SC_NS);
   if (mem_read_data.read() == 0xDECA) {
      cout << "@" << sc_time_stamp() << ": " << "Read OK" << endl;
   } else {
      cout << "@" << sc_time_stamp() << ": " << "Read ERROR" << endl;
   }

   cout << "@" << sc_time_stamp() << ": " << "Backdoor write to address 0x40 the value 0xFACE" << endl;
   dmem.memory[0x40] = 0xFACE;
   cout << "@" << sc_time_stamp() << ": " << "Read address 0x40" << endl;
   mem_access_addr = 0x40;
   mem_write_data = 0x0;
   mem_write_en = false;
   sc_start(1, SC_NS);
   if (mem_read_data.read() == 0xFACE) {
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
