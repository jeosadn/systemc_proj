#include "systemc.h"
#include "../../rtl_c/hazard_detection_unit.cpp"

int sc_main(int argc, char* argv[]) {

    //=========================================================================
    // Signals
    //=========================================================================
    sc_clock clk("clk", 1, SC_NS, 0.5);
    sc_signal< sc_uint<3> > decoding_op_src1;
    sc_signal< sc_uint<3> > decoding_op_src2;
    sc_signal< sc_uint<3> > ex_op_dest;
    sc_signal< sc_uint<3> > wb_op_dest;
    sc_signal< sc_uint<3> > mem_op_dest;

    sc_signal< bool > pipeline_stall_n;

    //=========================================================================
    // DUT
    //=========================================================================
    hazard_detection_unit hazard_detection_unit_inst("hazard_detection_unit_inst");
        hazard_detection_unit_inst.decoding_op_src1(decoding_op_src1);
        hazard_detection_unit_inst.decoding_op_src2(decoding_op_src2);
        hazard_detection_unit_inst.ex_op_dest(ex_op_dest);
        hazard_detection_unit_inst.wb_op_dest(wb_op_dest);
        hazard_detection_unit_inst.mem_op_dest(mem_op_dest);
        hazard_detection_unit_inst.pipeline_stall_n(pipeline_stall_n);

    //=========================================================================
    // VCD
    //=========================================================================
    sc_trace_file *wf = sc_create_vcd_trace_file("hazard_detection_unit");
    wf->set_time_unit(1, SC_PS);
    hazard_detection_unit_inst.wf = wf;
    hazard_detection_unit_inst.trace_me();

    //=========================================================================
    // Stimulus
    //=========================================================================
    cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
    sc_start(0, SC_NS);
    cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
    decoding_op_src1 = 0;
    decoding_op_src2 = 0;
    ex_op_dest = 0;
    wb_op_dest = 0;
    mem_op_dest = 0;
    sc_start(10, SC_NS);

    cout << "\n@" << sc_time_stamp() << ": " << "Inputs values:" << endl;
    decoding_op_src1 = 0;
    decoding_op_src2 = 0;
    ex_op_dest = 1;
    mem_op_dest = 2;
    wb_op_dest = 3;
    sc_start(1, SC_NS);
    cout << "\tdecoding_op_src1: " << decoding_op_src1 << "\n\tdecoding_op_src2: " << decoding_op_src2 
                << "\n\tex_op_dest: " << ex_op_dest << "\n\tmem_op_dest: " << mem_op_dest << "\n\twb_op_dest: " << wb_op_dest << endl;
    if (pipeline_stall_n.read() == 1) {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall OK" << endl;
    } else {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall ERROR" << endl;
    }
    sc_start(10, SC_NS);

    cout << "\n@" << sc_time_stamp() << ": " << "Inputs values:" << endl;
    decoding_op_src1 = 5;
    decoding_op_src2 = 0;
    ex_op_dest = 5;
    mem_op_dest = 5;
    wb_op_dest = 3;
    sc_start(1, SC_NS);
    cout << "\tdecoding_op_src1: " << decoding_op_src1 << "\n\tdecoding_op_src2: " << decoding_op_src2 
                << "\n\tex_op_dest: " << ex_op_dest << "\n\tmem_op_dest: " << mem_op_dest << "\n\twb_op_dest: " << wb_op_dest << endl;
    if (pipeline_stall_n.read()  == 0) {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall OK" << endl;
    } else {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall ERROR" << endl;
    }
    sc_start(10, SC_NS);

    cout << "\n@" << sc_time_stamp() << ": " << "Inputs values:" << endl;
    decoding_op_src1 = 5;
    decoding_op_src2 = 5;
    ex_op_dest = 5;
    mem_op_dest = 5;
    wb_op_dest = 0;
    sc_start(1, SC_NS);
    cout << "\tdecoding_op_src1: " << decoding_op_src1 << "\n\tdecoding_op_src2: " << decoding_op_src2 
                << "\n\tex_op_dest: " << ex_op_dest << "\n\tmem_op_dest: " << mem_op_dest << "\n\twb_op_dest: " << wb_op_dest << endl;
    if (pipeline_stall_n.read()  == 0) {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall OK" << endl;
    } else {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall ERROR" << endl;
    }
    sc_start(10, SC_NS);

    cout << "\n@" << sc_time_stamp() << ": " << "Inputs values:" << endl;
    decoding_op_src1 = 5;
    decoding_op_src2 = 5;
    ex_op_dest = 7;
    mem_op_dest = 5;
    wb_op_dest = 0;
    sc_start(1, SC_NS);
    cout << "\tdecoding_op_src1: " << decoding_op_src1 << "\n\tdecoding_op_src2: " << decoding_op_src2 
                << "\n\tex_op_dest: " << ex_op_dest << "\n\tmem_op_dest: " << mem_op_dest << "\n\twb_op_dest: " << wb_op_dest << endl;
    if (pipeline_stall_n.read()  == 0) {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall OK" << endl;
    } else {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall ERROR" << endl;
    }
    sc_start(10, SC_NS);

    cout << "\n@" << sc_time_stamp() << ": " << "Inputs values:" << endl;
    decoding_op_src1 = 5;
    decoding_op_src2 = 5;
    ex_op_dest = 0;
    mem_op_dest = 0;
    wb_op_dest = 0;
    sc_start(1, SC_NS);
    cout << "\tdecoding_op_src1: " << decoding_op_src1 << "\n\tdecoding_op_src2: " << decoding_op_src2 
                << "\n\tex_op_dest: " << ex_op_dest << "\n\tmem_op_dest: " << mem_op_dest << "\n\twb_op_dest: " << wb_op_dest << endl;
    if (pipeline_stall_n.read()  == 1) {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall OK" << endl;
    } else {
        cout << "@" << sc_time_stamp() << ": " << "Pipeline stall ERROR" << endl;
    }
    sc_start(10, SC_NS);

   //=========================================================================
   // EOT
   //=========================================================================
   sc_close_vcd_trace_file(wf);
   return 0;
}
