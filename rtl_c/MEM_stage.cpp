#include "systemc.h"
#include "data_mem.cpp"

SC_MODULE(MEM_stage) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             clk;
   sc_in<bool>           rst;
   sc_in< sc_uint<38> >  pipeline_reg_in; //From EX_stage

   //Outputs
   sc_out< sc_uint<37> > pipeline_reg_out; //To WB_stage
   sc_out < sc_uint<3> > mem_op_dest;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_signal< sc_uint<16> > ex_alu_result;
   sc_signal<bool>          mem_write_en;
   sc_signal< sc_uint<16> > mem_write_data;
   sc_signal< sc_uint<16> > mem_read_data;

   //Methods
   void assign_ex_alu_result() {
      ex_alu_result = pipeline_reg_in.read().range(37,22);
   }

   void assign_mem_write_en() {
      mem_write_en = pipeline_reg_in.read().range(21,21);
   }

   void assign_mem_write_data() {
      mem_write_data = pipeline_reg_in.read().range(20,5);
   }

   void assign_pipeline_reg_out() {
      while (true) {
         if (rst) {
            pipeline_reg_out.write(0);
         } else {
            sc_uint<37> temp;
            temp.range(36,21) = ex_alu_result;
            temp.range(20,5)  = mem_read_data;
            temp.range(4,0)   = pipeline_reg_in.read().range(4,0);
            pipeline_reg_out.write(temp);
         }
         wait();
      }
   }

   void assign_mem_op_dest() {
      mem_op_dest.write(pipeline_reg_in.read().range(3,1));
   }

   //Submodules
   data_mem dmem;

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(MEM_stage)
   : clk("clk")
   , rst("rst")
   , pipeline_reg_in("pipeline_reg_in")
   , pipeline_reg_out("pipeline_reg_out")
   , mem_op_dest("mem_op_dest")
   , ex_alu_result("ex_alu_result")
   , mem_write_en("mem_write_en")
   , mem_write_data("mem_write_data")
   , mem_read_data("mem_read_data")
   , dmem("dmem")
   {
      SC_METHOD(assign_ex_alu_result);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_mem_write_en);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_mem_write_data);
         sensitive << pipeline_reg_in;

      SC_CTHREAD(assign_pipeline_reg_out, clk.pos());

      SC_METHOD(assign_mem_op_dest);
         sensitive << pipeline_reg_in;

      dmem.clk(clk);
      dmem.mem_access_addr(ex_alu_result);
      dmem.mem_write_data(mem_write_data);
      dmem.mem_write_en(mem_write_en);
      dmem.mem_read_data(mem_read_data);
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, pipeline_reg_in, pipeline_reg_in.name());
      sc_trace(wf, pipeline_reg_out, pipeline_reg_out.name());
      sc_trace(wf, mem_op_dest, mem_op_dest.name());
      sc_trace(wf, rst, rst.name());
      sc_trace(wf, ex_alu_result, ex_alu_result.name());
      sc_trace(wf, mem_write_en, mem_write_en.name());
      sc_trace(wf, mem_write_data, mem_write_data.name());
      sc_trace(wf, mem_read_data, mem_read_data.name());

      dmem.wf = wf;
      dmem.trace_me();
   }
};
