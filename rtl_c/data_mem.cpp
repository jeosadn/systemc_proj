#include "systemc.h"

SC_MODULE(data_mem) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             clk;
   sc_in< sc_uint<16> >  mem_access_addr;
   sc_in< sc_uint<16> >  mem_write_data;
   sc_in<bool>           mem_write_en;

   //Outputs
   sc_out< sc_uint<16> > mem_read_data;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_signal< sc_uint<16> > memory[256];

   //Methods
   void write_op() {
      while (true) {
         if (mem_write_en.read()) {
            memory[mem_access_addr.read()] = mem_write_data.read();
         }
         wait();
      }
   }

   void read_op() {
      mem_read_data.write(memory[mem_access_addr.read()]);
   }

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(data_mem)
   : clk("clk")
   , mem_access_addr("mem_access_addr")
   , mem_write_data("mem_write_data")
   , mem_write_en("mem_write_en")
   , mem_read_data("mem_read_data")
   {
      int j = 0;

      SC_CTHREAD(write_op, clk.pos());

      SC_METHOD(read_op);
         sensitive << mem_access_addr;
         for (j = 0; j < 256; j++)
            sensitive << memory[j];
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, mem_access_addr, mem_access_addr.name());
      sc_trace(wf, mem_write_data, mem_write_data.name());
      sc_trace(wf, mem_write_en, mem_write_en.name());
      sc_trace(wf, mem_read_data, mem_read_data.name());
   }
};
