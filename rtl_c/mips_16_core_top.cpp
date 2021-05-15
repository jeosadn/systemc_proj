#include "systemc.h"
#include "EX_stage.cpp"
#include "ID_stage.cpp"
#include "IF_stage.cpp"
#include "MEM_stage.cpp"
#include "WB_stage.cpp"
#include "hazard_detection_unit.cpp"
#include "register_file.cpp"

SC_MODULE(mips_16_core_top) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk             clk;
   sc_in<bool>           rst;

   //Outputs
   sc_out < sc_uint<8> > pc;

   //=========================================================================
   // Internal
   //=========================================================================
   //Signals
   sc_signal<bool>          pipeline_stall_n;
   sc_signal< sc_int<6> >   branch_offset_imm;
   sc_signal<bool>          branch_taken;
   sc_signal< sc_uint<16> > instruction;
   sc_signal< sc_uint<57> > ID_pipeline_reg_out;
   sc_signal< sc_uint<38> > EX_pipeline_reg_out;
   sc_signal< sc_uint<37> > MEM_pipeline_reg_out;

   sc_signal< sc_uint<3> >  reg_read_addr_1;
   sc_signal< sc_uint<3> >  reg_read_addr_2;
   sc_signal< sc_uint<16> > reg_read_data_1;
   sc_signal< sc_uint<16> > reg_read_data_2;
   sc_signal< sc_uint<3> >  decoding_op_src1;
   sc_signal< sc_uint<3> >  decoding_op_src2;
   sc_signal< sc_uint<3> >  ex_op_dest;
   sc_signal< sc_uint<3> >  mem_op_dest;
   sc_signal< sc_uint<3> >  wb_op_dest;
   sc_signal<bool>          reg_write_en;
   sc_signal< sc_uint<3> >  reg_write_dest;
   sc_signal< sc_uint<16> > reg_write_data;

   //Submodules
   IF_stage              IF_stage_inst;
   ID_stage              ID_stage_inst;
   EX_stage              EX_stage_inst;
   MEM_stage             MEM_stage_inst;
   WB_stage              WB_stage_inst;
   register_file         register_file_inst;
   hazard_detection_unit hazard_detection_unit_inst;

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(mips_16_core_top)
   : clk("clk")
   , rst("rst")
   , pc("pc")
   , pipeline_stall_n("pipeline_stall_n")
   , branch_offset_imm("branch_offset_imm")
   , branch_taken("branch_taken")
   , instruction("instruction")
   , ID_pipeline_reg_out("ID_pipeline_reg_out")
   , EX_pipeline_reg_out("EX_pipeline_reg_out")
   , MEM_pipeline_reg_out("MEM_pipeline_reg_out")
   , reg_read_addr_1("reg_read_addr_1")
   , reg_read_addr_2("reg_read_addr_2")
   , reg_read_data_1("reg_read_data_1")
   , reg_read_data_2("reg_read_data_2")
   , decoding_op_src1("decoding_op_src1")
   , decoding_op_src2("decoding_op_src2")
   , ex_op_dest("ex_op_dest")
   , mem_op_dest("mem_op_dest")
   , wb_op_dest("wb_op_dest")
   , reg_write_en("reg_write_en")
   , reg_write_dest("reg_write_dest")
   , reg_write_data("reg_write_data")
   , IF_stage_inst("IF_stage_inst")
   , ID_stage_inst("ID_stage_inst")
   , EX_stage_inst("EX_stage_inst")
   , MEM_stage_inst("MEM_stage_inst")
   , WB_stage_inst("WB_stage_inst")
   , register_file_inst("register_file_inst")
   , hazard_detection_unit_inst("hazard_detection_unit_inst")
   {
      IF_stage_inst.clk(clk);
      IF_stage_inst.rst(rst);
      IF_stage_inst.instruction_fetch_en(pipeline_stall_n);
      IF_stage_inst.branch_offset_imm(branch_offset_imm);
      IF_stage_inst.branch_taken(branch_taken);
      IF_stage_inst.pc(pc);
      IF_stage_inst.instruction(instruction);

      ID_stage_inst.clk(clk);
      ID_stage_inst.rst(rst);
      ID_stage_inst.instruction_decode_en(pipeline_stall_n);
      ID_stage_inst.pipeline_reg_out(ID_pipeline_reg_out);
      ID_stage_inst.instruction(instruction);
      ID_stage_inst.branch_offset_imm(branch_offset_imm);
      ID_stage_inst.branch_taken(branch_taken);
      ID_stage_inst.reg_read_addr_1(reg_read_addr_1);
      ID_stage_inst.reg_read_addr_2(reg_read_addr_2);
      ID_stage_inst.reg_read_data_1(reg_read_data_1);
      ID_stage_inst.reg_read_data_2(reg_read_data_2);
      ID_stage_inst.decoding_op_src1(decoding_op_src1);
      ID_stage_inst.decoding_op_src2(decoding_op_src2);

      EX_stage_inst.clk(clk);
      EX_stage_inst.rst(rst);
      EX_stage_inst.pipeline_reg_in(ID_pipeline_reg_out);
      EX_stage_inst.pipeline_reg_out(EX_pipeline_reg_out);
      EX_stage_inst.ex_op_dest(ex_op_dest);

      MEM_stage_inst.clk(clk);
      MEM_stage_inst.rst(rst);
      MEM_stage_inst.pipeline_reg_in(EX_pipeline_reg_out);
      MEM_stage_inst.pipeline_reg_out(MEM_pipeline_reg_out);
      MEM_stage_inst.mem_op_dest(mem_op_dest);

      WB_stage_inst.pipeline_reg_in(MEM_pipeline_reg_out);
      WB_stage_inst.reg_write_en(reg_write_en);
      WB_stage_inst.reg_write_dest(reg_write_dest);
      WB_stage_inst.reg_write_data(reg_write_data);
      WB_stage_inst.wb_op_dest(wb_op_dest);

      register_file_inst.clk(clk);
      register_file_inst.rst(rst);
      register_file_inst.reg_write_en(reg_write_en);
      register_file_inst.reg_write_dest(reg_write_dest);
      register_file_inst.reg_write_data(reg_write_data);
      register_file_inst.reg_read_addr_1(reg_read_addr_1);
      register_file_inst.reg_read_data_1(reg_read_data_1);
      register_file_inst.reg_read_addr_2(reg_read_addr_2);
      register_file_inst.reg_read_data_2(reg_read_data_2);

      hazard_detection_unit_inst.decoding_op_src1(decoding_op_src1);
      hazard_detection_unit_inst.decoding_op_src2(decoding_op_src2);
      hazard_detection_unit_inst.ex_op_dest(ex_op_dest);
      hazard_detection_unit_inst.mem_op_dest(mem_op_dest);
      hazard_detection_unit_inst.wb_op_dest(wb_op_dest);
      hazard_detection_unit_inst.pipeline_stall_n(pipeline_stall_n);
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, rst, rst.name());
      sc_trace(wf, pc, pc.name());

      sc_trace(wf, pipeline_stall_n, pipeline_stall_n.name());
      sc_trace(wf, branch_offset_imm, branch_offset_imm.name());
      sc_trace(wf, branch_taken, branch_taken.name());
      sc_trace(wf, instruction, instruction.name());
      sc_trace(wf, ID_pipeline_reg_out, ID_pipeline_reg_out.name());
      sc_trace(wf, EX_pipeline_reg_out, EX_pipeline_reg_out.name());
      sc_trace(wf, MEM_pipeline_reg_out, MEM_pipeline_reg_out.name());
      sc_trace(wf, reg_read_addr_1, reg_read_addr_1.name());
      sc_trace(wf, reg_read_addr_2, reg_read_addr_2.name());
      sc_trace(wf, reg_read_data_1, reg_read_data_1.name());
      sc_trace(wf, reg_read_data_2, reg_read_data_2.name());
      sc_trace(wf, decoding_op_src1, decoding_op_src1.name());
      sc_trace(wf, decoding_op_src2, decoding_op_src2.name());
      sc_trace(wf, ex_op_dest, ex_op_dest.name());
      sc_trace(wf, mem_op_dest, mem_op_dest.name());
      sc_trace(wf, wb_op_dest, wb_op_dest.name());
      sc_trace(wf, reg_write_en, reg_write_en.name());
      sc_trace(wf, reg_write_dest, reg_write_dest.name());
      sc_trace(wf, reg_write_data, reg_write_data.name());

      IF_stage_inst.wf = wf;
      IF_stage_inst.trace_me();

      ID_stage_inst.wf = wf;
      ID_stage_inst.trace_me();

      EX_stage_inst.wf = wf;
      EX_stage_inst.trace_me();

      MEM_stage_inst.wf = wf;
      MEM_stage_inst.trace_me();

      WB_stage_inst.wf = wf;
      WB_stage_inst.trace_me();

      register_file_inst.wf = wf;
      register_file_inst.trace_me();

      hazard_detection_unit_inst.wf = wf;
      hazard_detection_unit_inst.trace_me();
   }
};
