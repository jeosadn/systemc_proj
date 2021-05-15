#include "systemc.h"
#include "mips_16_defs.h"

SC_MODULE(register_file) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             clk;
   sc_in<bool>           rst;
   sc_in<bool>           reg_write_en;
   sc_in< sc_uint<3> >   reg_write_dest;
   sc_in< sc_uint<16> >  reg_write_data;
   sc_in< sc_uint<3> >   reg_read_addr_1;
   sc_in< sc_uint<3> >   reg_read_addr_2;

   //Outputs
   sc_out < sc_uint<16> > reg_read_data_1;
   sc_out < sc_uint<16> > reg_read_data_2;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_signal< sc_uint<16> > reg_array[8];

   //Methods
   void assign_reg_array() {
      while (true) {
         wait();
         if (rst) {
            reg_array[0] = 0;
            reg_array[1] = 0;
            reg_array[2] = 0;
            reg_array[3] = 0;
            reg_array[4] = 0;
            reg_array[5] = 0;
            reg_array[6] = 0;
            reg_array[7] = 0;
         } else if(reg_write_en) {
            reg_array[reg_write_dest.read()] = reg_write_data.read();
         }
      }
   }

   void assign_reg_read_data_1() {
      if(reg_read_addr_1.read() == 0){
        reg_read_data_1.write(0);
      }else{
        reg_read_data_1.write(reg_array[reg_read_addr_1.read()]);
      }
   }

   void assign_reg_read_data_2() {
      if(reg_read_addr_2.read() == 0){
        reg_read_data_2.write(0);
      }else{
        reg_read_data_2.write(reg_array[reg_read_addr_2.read()]);
      }
   }

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(register_file)
   : clk("clk")
   , rst("rst")
   , reg_write_en("reg_write_en")
   , reg_write_dest("reg_write_dest")
   , reg_write_data("reg_write_data")
   , reg_read_addr_1("reg_read_addr_1")
   , reg_read_addr_2("reg_read_addr_2")
   , reg_read_data_1("reg_read_data_1")
   , reg_read_data_2("reg_read_data_2")
   {
      int j = 0;

      SC_CTHREAD(assign_reg_array, clk.pos());

      SC_METHOD(assign_reg_read_data_1);
         sensitive << reg_read_addr_1;
         for (j = 0; j < 8; j++)
            sensitive << reg_array[j];

      SC_METHOD(assign_reg_read_data_2);
         sensitive << reg_read_addr_2;
         for (j = 0; j < 8; j++)
            sensitive << reg_array[j];
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk,            clk.name());
      sc_trace(wf, rst,            rst.name());
      sc_trace(wf, reg_write_en,   reg_write_en.name());
      sc_trace(wf, reg_write_dest, reg_write_dest.name());
      sc_trace(wf, reg_write_data, reg_write_data.name());
      sc_trace(wf, reg_read_addr_1, reg_read_addr_1.name());
      sc_trace(wf, reg_read_addr_2, reg_read_addr_2.name());
      sc_trace(wf, reg_read_data_1, reg_read_data_1.name());
      sc_trace(wf, reg_read_data_2, reg_read_data_2.name());
   }
};
