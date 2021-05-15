#include "systemc.h"
#include "mips_16_defs.h"

SC_MODULE(WB_stage) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   // [36:21],16bits: ex_alu_result[15:0]
   // [20:5],16bits:  mem_read_data[15:0]
   // [4:0],5bits:    write_back_en, write_back_dest[2:0], write_back_result_mux,
   sc_in< sc_uint<37> >   pipeline_reg_in;

   //Outputs
   sc_out <bool> reg_write_en;
   sc_out < sc_uint<3> > reg_write_dest;
   sc_out < sc_uint<16> > reg_write_data;
   sc_out < sc_uint<3> > wb_op_dest;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_uint<16> ex_alu_result;
   sc_uint<16> mem_read_data;
   sc_uint<1> write_back_en;
   sc_uint<3> write_back_dest;
   sc_uint<1> write_back_result_mux;

   //Methods
   void assign_reg_write_data() {
     ex_alu_result = pipeline_reg_in.read().range(36, 21);
     mem_read_data = pipeline_reg_in.read().range(20, 5);
     write_back_result_mux = pipeline_reg_in.read().range(0, 0);

     if(write_back_result_mux)
      reg_write_data.write(mem_read_data);
     else
      reg_write_data.write(ex_alu_result);
   }

   void assign_write_back_en() {
      write_back_en = pipeline_reg_in.read().range(4, 4);
      reg_write_en.write(write_back_en);
   }

   void assign_write_back_dest() {
     write_back_dest = pipeline_reg_in.read().range(3, 1);
     reg_write_dest.write(write_back_dest);
   }

   void assign_wb_op_dest() {
     wb_op_dest.write(pipeline_reg_in.read().range(3, 1));
   }

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(WB_stage)
   : pipeline_reg_in("pipeline_reg_in")
   , reg_write_en("reg_write_en")
   , reg_write_dest("reg_write_dest")
   , reg_write_data("reg_write_data")
   , wb_op_dest("wb_op_dest")
   {
      SC_METHOD(assign_reg_write_data);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_write_back_en);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_write_back_dest);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_wb_op_dest);
         sensitive << pipeline_reg_in;
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, pipeline_reg_in, pipeline_reg_in.name());
      sc_trace(wf, reg_write_en,    reg_write_en.name());
      sc_trace(wf, reg_write_dest,  reg_write_dest.name());
      sc_trace(wf, reg_write_data,  reg_write_data.name());
      sc_trace(wf, wb_op_dest,      wb_op_dest.name());
   }
};
