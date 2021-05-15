#include "systemc.h"
#include "alu.cpp"

SC_MODULE(EX_stage) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             clk;
   sc_in<bool>           rst;
   sc_in< sc_uint<57> >  pipeline_reg_in;   //  [56:22],35bits: ex_alu_cmd[2:0], ex_alu_src1[15:0], ex_alu_src2[15:0]
                                            //  [21:5],17bits:  mem_write_en, mem_write_data[15:0]
                                            //  [4:0],5bits:    write_back_en, write_back_dest[2:0], write_back_result_mux,
   //Outputs
   sc_out< sc_uint<38> > pipeline_reg_out;  //  [37:22],16bits: ex_alu_result[15:0];
                                            //  [21:5],17bits:  mem_write_en, mem_write_data[15:0]
                                            //  [4:0],5bits:    write_back_en, write_back_dest[2:0], write_back_result_mux,
   sc_out< sc_uint<3> > ex_op_dest;

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_signal< sc_uint<3> > alu_cmd;
   sc_signal< sc_uint<16> > alu_src1;
   sc_signal< sc_uint<16> > alu_src2;
   sc_signal< sc_uint<16> > ex_alu_result;

   //Methods

   void assign_alu_cmd() {
      alu_cmd = pipeline_reg_in.read().range(56,54);
   }

   void assign_alu_srcs() {
      alu_src1 = pipeline_reg_in.read().range(53,38);
      alu_src2 = pipeline_reg_in.read().range(37,22);
   }

   void assign_pipeline_reg_out() {
      while (true) {
         if (rst.read()) {
            pipeline_reg_out.write(0);
         } else {
            sc_uint<38> temp;
            temp.range(37,22) = ex_alu_result;
            temp.range(21,0)   = pipeline_reg_in.read().range(21,0);
            pipeline_reg_out.write(temp);
         }
         wait();
      }
   }

   void assign_ex_op_dest() {
      ex_op_dest.write(pipeline_reg_in.read().range(3,1));
   }

   //Submodules
   alu alu_inst;

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(EX_stage)
   : clk("clk")
   , rst("rst")
   , pipeline_reg_in("pipeline_reg_in")
   , pipeline_reg_out("pipeline_reg_out")
   , ex_op_dest("ex_op_dest")
   , alu_src1("alu_src1")
   , alu_src2("alu_src2")
   , alu_cmd("alu_cmd")
   , ex_alu_result("ex_alu_result")
   , alu_inst("alu_inst")
   {

      SC_METHOD(assign_alu_cmd);
         sensitive << pipeline_reg_in;

      SC_METHOD(assign_alu_srcs);
         sensitive << pipeline_reg_in;

      SC_CTHREAD(assign_pipeline_reg_out, clk.pos());

      SC_METHOD(assign_ex_op_dest);
         sensitive << pipeline_reg_in;

      alu_inst.clk(clk);
      alu_inst.a(alu_src1);
      alu_inst.b(alu_src2);
      alu_inst.cmd(alu_cmd);
      alu_inst.r(ex_alu_result);
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, rst, rst.name());
      sc_trace(wf, pipeline_reg_in, pipeline_reg_in.name());
      sc_trace(wf, pipeline_reg_out, pipeline_reg_out.name());
      sc_trace(wf, ex_op_dest, ex_op_dest.name());
      sc_trace(wf, alu_src1, alu_src1.name());
      sc_trace(wf, alu_src2, alu_src2.name());
      sc_trace(wf, alu_cmd, alu_cmd.name());
      sc_trace(wf, ex_alu_result, ex_alu_result.name());

      alu_inst.wf = wf;
      alu_inst.trace_me();
   }
};
