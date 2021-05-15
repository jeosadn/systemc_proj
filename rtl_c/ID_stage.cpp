#include "systemc.h"
#include "mips_16_defs.h"

SC_MODULE (ID_stage) {

    //=========================================================================
    // Ports
    //=========================================================================
    //Inputs
    sc_in_clk           clk;
    sc_in< bool > rst;
    sc_in< bool > instruction_decode_en;

    sc_in< sc_uint<16> > instruction;

    sc_in< sc_uint<16> > reg_read_data_1;
    sc_in< sc_uint<16> > reg_read_data_2;

    //Outputs
    sc_out< sc_uint<57> > pipeline_reg_out;

    sc_out< sc_int<6> > branch_offset_imm;
    sc_out< bool > branch_taken;

    sc_out< sc_uint<3> > reg_read_addr_1;
    sc_out< sc_uint<3> > reg_read_addr_2;

    sc_out< sc_uint<3> > decoding_op_src1;
    sc_out< sc_uint<3> > decoding_op_src2;


    //=========================================================================
    // Internal
    //=========================================================================
    //Variables
    sc_signal<sc_uint<16> > instruction_reg;
    sc_signal<sc_uint<4> > ir_op_code;
    sc_signal<sc_uint<3> > ir_dest;
    sc_signal<sc_uint<3> > ir_src1;
    sc_signal<sc_uint<3> > ir_src2;
    sc_signal<sc_int<6> > ir_imm;

    sc_uint<1> write_back_en;
    sc_signal<sc_uint<3> > write_back_dest;
    sc_logic write_back_result_mux;

    sc_signal<sc_uint<1> > mem_write_en;
    sc_signal<sc_uint<16> > mem_write_data;

    sc_uint<3> ex_alu_cmd;
    sc_signal<sc_uint<16> > ex_alu_src1;
    sc_signal<sc_uint<16> > ex_alu_src2;

    sc_signal<sc_logic > alu_src2_mux;
    sc_signal<sc_uint<1> > decoding_op_is_branch;
    sc_signal<sc_uint<1> > decoding_op_is_store;
    sc_signal<sc_uint<4> > ir_op_code_with_bubble;
    sc_signal<sc_uint<3> > ir_dest_with_bubble;


    //Methods
    void inst_reg() {
        while (true) {
            if (rst.read()) {
                instruction_reg = 0;
            } else {
                if (instruction_decode_en.read()) {
                    instruction_reg = instruction.read();
                }
            }
            wait();
        }
    }

    void instruction_decode() {
        sc_uint<16> instruction_reg_int = instruction_reg;

        ir_op_code = instruction_reg_int.range(15,12);
        ir_dest = instruction_reg_int.range(11,9);
        ir_src1 = instruction_reg_int.range(8,6);
        if (decoding_op_is_store != sc_uint<1>(0)) {
            ir_src2 = instruction_reg_int.range(11,9);
        } else {
            ir_src2 = instruction_reg_int.range(5,3);
        }
        ir_imm  = sc_int<6> (instruction_reg_int.range(5,0));
    }

    void op_code_with_bubble() {
        if (instruction_decode_en.read()) {
            ir_op_code_with_bubble = ir_op_code;
        } else {
            ir_op_code_with_bubble = 0;
        }
        //ir_op_code_with_bubble = ( instruction_decode_en.read() ) ? ir_op_code : (sc_signal<sc_uint<4> >) 0;
    }

    void dest_with_bubble() {
        if(instruction_decode_en.read()) {
            ir_dest_with_bubble = ir_dest;
        } else {
            ir_dest_with_bubble = 0;
        }
        //ir_dest_with_bubble = ( instruction_decode_en.read() ) ? ir_dest : 0;
    }

    void write_back() {
        write_back_dest = ir_dest_with_bubble;
        mem_write_en = decoding_op_is_store;
    }

    void branch() {
        sc_uint<4> ir_op_code_int = ir_op_code;

        if (ir_op_code_int == OP_BZ) {
            decoding_op_is_branch = 1;
        } else {
            decoding_op_is_branch = 0;
        }
        //decoding_op_is_branch = ( ir_op_code == OP_BZ ) ? 1 : 0;
    }

    void op_is_branch() {
        sc_uint<3> ir_dest_with_bubble_int = ir_dest_with_bubble;
        bool branch_taken_int = branch_taken;

        if(decoding_op_is_branch != sc_uint<1>(0)) {
           switch( ir_dest_with_bubble_int ) {
                case BRANCH_Z:
                        if(reg_read_data_1.read() == 0) {
                            branch_taken_int = 1;
                        } else {
                            branch_taken_int = 0;
                        }
                        break;
                default:
                        branch_taken_int = 0;
                        break;
            }
        } else {
            branch_taken_int = 0;
        }

        branch_taken.write(branch_taken_int);
        branch_offset_imm.write(ir_imm);
    }

    void store() {
        sc_uint<4> ir_op_code_int = ir_op_code;

        if (ir_op_code_int == OP_ST) {
            decoding_op_is_store = 1;
        } else {
            decoding_op_is_store = 0;
        }
        //decoding_op_is_store = ( (int)ir_op_code == OP_ST ) ? 1 : 0;
    }

    void write_data() {
        mem_write_data = reg_read_data_2.read();
    }

    void alu_src1() {
        ex_alu_src1 = reg_read_data_1.read();
    }

    void alu_src2() {
        sc_int<6> ir_imm_int = ir_imm;

        if (alu_src2_mux != sc_logic('0')) {
            ex_alu_src2 = (ir_imm_int.range(5,5),ir_imm_int); //FIXME
        } else {
            ex_alu_src2 = reg_read_data_2.read();
        }
        //ex_alu_src2 = (alu_src2_mux) ? (ir_imm.range(5,5),ir_imm) : reg_read_data_2.read();
    }

    void pipeline_out() {
        while (true) {
            sc_uint<57> pipeline_reg_out_int;
            sc_uint<1> write_back_result_mux_int;

            if (write_back_result_mux == sc_logic ('1')) {
                write_back_result_mux_int = 1;
            } else {
                write_back_result_mux_int = 0;
            }

            if (rst.read()) {
                pipeline_reg_out_int = 0;
            } else {
                pipeline_reg_out_int = (
                    ex_alu_cmd,
                    ex_alu_src1,
                    ex_alu_src2,
                    mem_write_en,
                    mem_write_data,
                    write_back_en,
                    write_back_dest,
                    write_back_result_mux_int
                    );
            }

            pipeline_reg_out.write(pipeline_reg_out_int);
            wait();
        }
    }

    void read_addr() {
        reg_read_addr_1.write(ir_src1);
        reg_read_addr_2.write(ir_src2);
    }

    void decoding_op() {
        sc_uint<3> decoding_op_src2_int;
        sc_uint<4> ir_op_code_int = ir_op_code;

        if (
            ir_op_code_int == OP_NOP   ||
            ir_op_code_int == OP_ADDI  ||
            ir_op_code_int == OP_LD    ||
            ir_op_code_int == OP_BZ
        ) {
            decoding_op_src2_int = 0;
        } else {
            decoding_op_src2_int = ir_src2;
        }
        /*decoding_op_src2_int = (
                    ir_op_code == OP_NOP   ||
                    ir_op_code == OP_ADDI  ||
                    ir_op_code == OP_LD    ||
                    ir_op_code == OP_BZ
                    )?
                    0 : ir_src2;*/

        decoding_op_src1.write(ir_src1);
        decoding_op_src2.write(decoding_op_src2_int);
    }

    void op_code() {
        sc_uint<4> ir_op_code_with_bubble_int = ir_op_code_with_bubble;

        if(rst.read()) {
            write_back_en           = 0;
            write_back_result_mux   = sc_logic ('0');
            ex_alu_cmd              = 0;
            alu_src2_mux            = sc_logic ('0');
        } else {
            switch ( ir_op_code_with_bubble_int ) {
                case OP_NOP :
                        write_back_en           = 0;        // S3
                        write_back_result_mux   = sc_logic ('x'); // S1
                        ex_alu_cmd              = ALU_NC;   // S2
                        alu_src2_mux            = sc_logic ('x'); // S4
                        break;
                case OP_ADD :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_ADD;  // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_SUB :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_SUB;  // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_AND :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_AND;  // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_OR  :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_OR;   // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_XOR :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_XOR;  // S2
                        alu_src2_mux            = sc_logic ('x'); // S4
                        break;
                case OP_SL  :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_SL;   // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_SR  :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_SR;   // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_SRU :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_SRU;  // S2
                        alu_src2_mux            = sc_logic ('0');        // S4
                        break;
                case OP_ADDI:
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('0');        // S1
                        ex_alu_cmd              = ALU_ADD;  // S2
                        alu_src2_mux            = sc_logic ('1');        // S4
                        break;
                case OP_LD  :
                        write_back_en           = 1;        // S3
                        write_back_result_mux   = sc_logic ('1');        // S1
                        ex_alu_cmd              = ALU_ADD;  // S2
                        alu_src2_mux            = sc_logic ('1');        // S4
                        break;
                case OP_ST  :
                        write_back_en           = 0;        // S3
                        write_back_result_mux   = sc_logic ('x'); // S1
                        ex_alu_cmd              = ALU_ADD;  // S2
                        alu_src2_mux            = sc_logic ('1');        // S4
                        break;
                case OP_BZ  :
                        write_back_en           = 0;        // S3
                        write_back_result_mux   = sc_logic ('x'); // S1
                        ex_alu_cmd              = ALU_NC;   // S2
                        alu_src2_mux            = sc_logic ('1');        // S4
                        break;
                default :
                        write_back_en           = 0;        // S3
                        write_back_result_mux   = sc_logic ('x'); // S1
                        ex_alu_cmd              = ALU_NC;   // S2
                        alu_src2_mux            = sc_logic ('x'); // S4
                        break;
            }
        }
    }

    //=========================================================================
    // Constructor
    //=========================================================================
    SC_CTOR(ID_stage)
    : clk("clk")
    , rst("rst")
    , instruction_decode_en("instruction_decode_en")
    , instruction("instruction")
    , reg_read_data_1("reg_read_data_1")
    , reg_read_data_2("reg_read_data_2")
    , pipeline_reg_out("pipeline_reg_out")
    , branch_offset_imm("branch_offset_imm")
    , branch_taken("branch_taken")
    , reg_read_addr_1("reg_read_addr_1")
    , reg_read_addr_2("reg_read_addr_2")
    , decoding_op_src1("decoding_op_src1")
    , decoding_op_src2("decoding_op_src2")
    {
        SC_CTHREAD(inst_reg, clk.pos());

        SC_METHOD(instruction_decode);
        sensitive << instruction_reg;
        sensitive << decoding_op_is_store;

        SC_METHOD(op_code_with_bubble);
        sensitive << ir_op_code;
        sensitive << instruction_decode_en;

        SC_METHOD(dest_with_bubble);
        sensitive << ir_dest;
        sensitive << instruction_decode_en;

        SC_METHOD(write_back);
        sensitive << ir_dest_with_bubble;
        sensitive << decoding_op_is_store;

        SC_METHOD(branch);
        sensitive << ir_op_code;

        SC_METHOD(op_is_branch);
        sensitive << decoding_op_is_branch;
        sensitive << ir_imm;

        SC_METHOD(store);
        sensitive << ir_op_code;

        SC_METHOD(write_data);
        sensitive << reg_read_data_2;

        SC_METHOD(alu_src1);
        sensitive << reg_read_data_1;

        SC_METHOD(alu_src2);
        sensitive << reg_read_data_2;
        sensitive << alu_src2_mux;
        sensitive << ir_imm;

        SC_CTHREAD(pipeline_out, clk.pos());
        sensitive << rst; 

        SC_METHOD(read_addr);
        sensitive << ir_src1;
        sensitive << ir_src2;

        SC_METHOD(decoding_op)
        sensitive << ir_src1;
        sensitive << ir_src2;
        sensitive << ir_op_code;

        SC_METHOD(op_code)
        sensitive << rst;
        sensitive << ir_op_code_with_bubble;
    }

    //=========================================================================
    // Traces
    //=========================================================================
    sc_trace_file *wf;
    void trace_me() {
        sc_trace(wf, clk, clk.name());
        sc_trace(wf, rst, rst.name());
        sc_trace(wf, instruction_decode_en, instruction_decode_en.name());
        sc_trace(wf, instruction, instruction.name());
        sc_trace(wf, reg_read_data_1, reg_read_data_1.name());
        sc_trace(wf, reg_read_data_2, reg_read_data_2.name());
        sc_trace(wf, pipeline_reg_out, pipeline_reg_out.name());
        sc_trace(wf, branch_offset_imm, branch_offset_imm.name());
        sc_trace(wf, branch_taken, branch_taken.name());
        sc_trace(wf, reg_read_addr_1, reg_read_addr_1.name());
        sc_trace(wf, reg_read_addr_2, reg_read_addr_2.name());
        sc_trace(wf, decoding_op_src1, decoding_op_src1.name());
        sc_trace(wf, decoding_op_src2, decoding_op_src2.name());
    }
};
