#include "systemc.h"
#include "../../rtl_c/ID_stage.cpp"

int sc_main(int argc, char* argv[]) {

    //=========================================================================
    // Signals
    //=========================================================================
    sc_clock clk("clk", 1, SC_NS, 0.5);
    sc_signal< bool > rst;
    sc_signal< bool > instruction_decode_en;

    sc_signal< sc_uint<57> > pipeline_reg_out;
    sc_uint<57> pipeline_reg_out_int;

    sc_signal< sc_uint<16> > instruction;
    sc_signal< sc_int<6> > branch_offset_imm;
    sc_signal< bool > branch_taken;

    sc_signal< sc_uint<3> > reg_read_addr_1;
    sc_signal< sc_uint<3> > reg_read_addr_2;
    sc_signal< sc_uint<16> > reg_read_data_1;
    sc_signal< sc_uint<16> > reg_read_data_2;

    sc_signal< sc_uint<3> > decoding_op_src1;
    sc_signal< sc_uint<3> > decoding_op_src2;


    //=========================================================================
    // DUT
    //=========================================================================
    ID_stage ID_stage_inst("ID_stage_inst");
        ID_stage_inst.clk(clk);
        ID_stage_inst.rst(rst);
        ID_stage_inst.instruction_decode_en(instruction_decode_en);
        ID_stage_inst.pipeline_reg_out(pipeline_reg_out);
        ID_stage_inst.instruction(instruction);
        ID_stage_inst.branch_offset_imm(branch_offset_imm);
        ID_stage_inst.branch_taken(branch_taken);
        ID_stage_inst.reg_read_addr_1(reg_read_addr_1);
        ID_stage_inst.reg_read_addr_2(reg_read_addr_2);
        ID_stage_inst.reg_read_data_1(reg_read_data_1);
        ID_stage_inst.reg_read_data_2(reg_read_data_2);
        ID_stage_inst.decoding_op_src1(decoding_op_src1);
        ID_stage_inst.decoding_op_src2(decoding_op_src2);

    //=========================================================================
    // VCD
    //=========================================================================
    sc_trace_file *wf = sc_create_vcd_trace_file("ID_stage");
    wf->set_time_unit(1, SC_PS);
    ID_stage_inst.wf = wf;
    ID_stage_inst.trace_me();

    //=========================================================================
    // Stimulus
    //=========================================================================
    cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
    sc_start(0, SC_NS);
    cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
    rst = 0;
    instruction_decode_en = 0;
    instruction = 0;
    reg_read_data_1 = 0;
    reg_read_data_2 = 0;
    sc_start(10, SC_NS);

    cout << "@" << sc_time_stamp() << ": " << "Sending Reset" << endl;
    rst = 0;
    rst = 1;
    sc_start(2, SC_NS);
    rst = 0;
    instruction_decode_en = 1;

    cout << "@" << sc_time_stamp() << ": " << "Test 1:" << endl;
    cout << "@" << sc_time_stamp() << ": " << "check datapath control logic S1~S6 correct or not" << endl;

    sc_start(1, SC_NS);
    instruction = ( OP_NOP, sc_uint<12>(0));
    cout << "@" << sc_time_stamp() << ": " << "OP_NOP" << endl;

    // ADD
    sc_start(2, SC_NS);
    reg_read_data_1 = 31;
    reg_read_data_2 = 28;
    instruction = ( OP_ADD, sc_uint<3>(0), sc_uint<3>(1), sc_uint<3>(2), sc_uint<3>(0));
    cout << "@" << sc_time_stamp() << ": " << "OP_ADD" << endl;
    sc_start(1, SC_NS);
    if(
        ID_stage_inst.write_back_en           != sc_uint<1>(1)        ||  // S3
        ID_stage_inst.write_back_result_mux   != sc_logic('0')        ||  // S1
        ID_stage_inst.ex_alu_cmd              != ALU_ADD  ||  // S2
        ID_stage_inst.alu_src2_mux            != sc_logic('0')        ||  // S4
        ID_stage_inst.decoding_op_is_branch   != sc_uint<1>(0)        ||  // S5
        ID_stage_inst.decoding_op_is_store    != sc_uint<1>(0)        ||  // S6
        decoding_op_src1                      != sc_uint<3>(1)        ||
        decoding_op_src2                      != sc_uint<3>(2)        ||
        branch_taken                          != sc_uint<1>(0)        ||
        branch_offset_imm                     != sc_int<6>(16)        ||
        reg_read_addr_1                       != sc_uint<3>(1)        ||
        reg_read_addr_2                       != sc_uint<3>(2)
    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR1" << endl;

    sc_start(1, SC_NS);
    pipeline_reg_out_int = pipeline_reg_out;
    if(
        pipeline_reg_out_int.range(56,54)     != ALU_ADD  ||  // S2 ex_alu_cmd
        pipeline_reg_out_int.range(53,38)     != sc_uint<16>(31)      ||  //    alu src1
        pipeline_reg_out_int.range(37,22)     != sc_uint<16>(28)      ||  //    alu src2
        pipeline_reg_out_int.range(21,21)     != sc_uint<1>(0)        ||  //    mem_write_en
        pipeline_reg_out_int.range(20,5)      != sc_uint<16>(28)      ||  //    mem_write_data
        pipeline_reg_out_int.range(4,4)       != sc_uint<1>(1)        ||  // S3 write_back_en
        pipeline_reg_out_int.range(3,1)       != sc_uint<3>(0)        ||  //    write_back_dest
        pipeline_reg_out_int.range(0,0)       != sc_uint<1>(0)            // S1 write_back_result_mux

    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR2" << endl;

    // ST
    sc_start(2, SC_NS);
    instruction = ( sc_uint<4>(OP_ST), sc_uint<3>(4), sc_uint<3>(5), sc_uint<6>(31));
    reg_read_data_1 = 7;
    reg_read_data_2 = 0x9a3c;
    cout << "@" << sc_time_stamp() << ": " << "OP_ST" << endl;
    sc_start(1, SC_NS);
    if(
        ID_stage_inst.write_back_en           != sc_uint<1>(0)        ||  // S3
        ID_stage_inst.write_back_result_mux   != sc_logic('x')        ||  // S1
        ID_stage_inst.ex_alu_cmd              != ALU_ADD  ||  // S2
        ID_stage_inst.alu_src2_mux            != sc_logic('1')        ||  // S4
        ID_stage_inst.decoding_op_is_branch   != sc_uint<1>(0)        ||  // S5
        ID_stage_inst.decoding_op_is_store    != sc_uint<1>(1)        ||  // S6
        decoding_op_src1                      != sc_uint<3>(5)        ||
        decoding_op_src2                      != sc_uint<3>(4)        ||
        branch_taken                          != sc_uint<1>(0)        ||
        branch_offset_imm                     != sc_int<6>(31)        ||
        reg_read_addr_1                       != sc_uint<3>(5)        ||
        reg_read_addr_2                       != sc_uint<3>(4)
    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR1" << endl;

    sc_start(2, SC_NS);
    pipeline_reg_out_int = pipeline_reg_out;
    if(
        pipeline_reg_out_int.range(53,38)     != sc_uint<16>(7)        ||  //    ex_alu_src1
        pipeline_reg_out_int.range(37,22)     != sc_uint<16>(31)       ||  //    ex_alu_src2
        pipeline_reg_out_int.range(21,21)     != sc_uint<1>(1)         ||  //    mem_write_en
        pipeline_reg_out_int.range(20,5)      != sc_uint<16>(0x9a3c)   ||  //    mem_write_data
        pipeline_reg_out_int.range(3,1)       != sc_uint<3>(4)            //    write_back_dest
    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR2" << endl;

    // BZ
    sc_start(2, SC_NS);
    instruction = ( sc_uint<4>(OP_BZ), sc_uint<3>(0), sc_uint<3>(7), sc_uint<6>(10));
    reg_read_data_1 = 0;
    reg_read_data_2 = 0xc32e;
    cout << "@" << sc_time_stamp() << ": " << "OP_BZ" << endl;
    sc_start(1, SC_NS);
    if(
        ID_stage_inst.write_back_en           != sc_uint<1>(0)        ||  // S3
        ID_stage_inst.write_back_result_mux   != sc_logic('x')        ||  // S1
        ID_stage_inst.ex_alu_cmd              != ALU_NC               ||  // S2
        ID_stage_inst.alu_src2_mux            != sc_logic('1')        ||  // S4
        ID_stage_inst.decoding_op_is_branch   != sc_uint<1>(1)        ||  // S5
        ID_stage_inst.decoding_op_is_store    != sc_uint<1>(0)        ||  // S6
        decoding_op_src1                      != sc_uint<3>(7)        ||
        decoding_op_src2                      != sc_uint<3>(0)        ||
        branch_taken                          != sc_uint<1>(1)        ||
        // branch_taken                          != 0        ||
        branch_offset_imm                     != sc_int<6>(10)        ||
        reg_read_addr_1                       != sc_uint<3>(7)        
        //reg_read_addr_2                       != sc_uint<3>(6)
    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR1" << endl;

    sc_start(1, SC_NS);
    pipeline_reg_out_int = pipeline_reg_out;
    if(
        pipeline_reg_out_int.range(53,38)     != sc_uint<16>(0)        ||  //    ex_alu_src1
        // pipeline_reg_out_int.range(53,38)      != 3        ||  //    ex_alu_src1
        pipeline_reg_out_int.range(37,22)     != sc_uint<16>(10)       ||  //    ex_alu_src2
        pipeline_reg_out_int.range(21,21)     != sc_uint<1>(0)         ||  //    mem_write_en
        pipeline_reg_out_int.range(20,5)      != sc_uint<16>(49966)    ||  //    mem_write_data
        pipeline_reg_out_int.range(3,1)       != sc_uint<3>(0)            //    write_back_dest
    )
        cout << "@" << sc_time_stamp() << ": " << "ERROR2" << endl;


   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
