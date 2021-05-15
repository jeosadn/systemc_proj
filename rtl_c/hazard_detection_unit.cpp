#include "systemc.h"

SC_MODULE(hazard_detection_unit) {

    //=========================================================================
    // Ports
    //=========================================================================
    //Inputs
    sc_in< sc_uint<3> >  decoding_op_src1;
    sc_in< sc_uint<3> >  decoding_op_src2;

    sc_in< sc_uint<3> >  ex_op_dest;
    sc_in< sc_uint<3> >  wb_op_dest;
    sc_in< sc_uint<3> >  mem_op_dest;

    //Outputs
    sc_out< bool > pipeline_stall_n;

    //=========================================================================
    // Internal
    //=========================================================================
    //Variables
    sc_uint<1> pipeline_stall;

    //Methods
    void hazard_detection() {
        pipeline_stall = 1;
        if (decoding_op_src1.read() != 0 &&
            (
                decoding_op_src1.read() == ex_op_dest.read() ||
                decoding_op_src1.read() == wb_op_dest.read() ||
                decoding_op_src1.read() == mem_op_dest.read() )
        ) {
            pipeline_stall = 0;
        }

        if (decoding_op_src2.read() != 0 &&
            (
                decoding_op_src2.read() == ex_op_dest.read() ||
                decoding_op_src2.read() == wb_op_dest.read() ||
                decoding_op_src2.read() == mem_op_dest.read() )
        ) {
            pipeline_stall = 0;
        }

        pipeline_stall_n.write(pipeline_stall);
    }

    //=========================================================================
    // Constructor
    //=========================================================================
    SC_CTOR(hazard_detection_unit)
    : decoding_op_src1("decoding_op_src1")
    , decoding_op_src2("decoding_op_src2")
    , ex_op_dest("ex_op_dest")
    , wb_op_dest("wb_op_dest")
    , mem_op_dest("mem_op_dest")
    , pipeline_stall_n("pipeline_stall_n")
    {
        SC_METHOD(hazard_detection);
            sensitive << decoding_op_src1;
            sensitive << decoding_op_src2;
            sensitive << ex_op_dest;
            sensitive << wb_op_dest;
            sensitive << mem_op_dest;
    }


   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, decoding_op_src1, decoding_op_src1.name());
      sc_trace(wf, decoding_op_src2, decoding_op_src2.name());
      sc_trace(wf, ex_op_dest, ex_op_dest.name());
      sc_trace(wf, wb_op_dest, wb_op_dest.name());
      sc_trace(wf, mem_op_dest, mem_op_dest.name());
      sc_trace(wf, pipeline_stall_n, pipeline_stall_n.name());
   }

};

