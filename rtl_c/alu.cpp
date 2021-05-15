#include "systemc.h"
/************** ALU operation command ****************/

#define ALU_ADD 0
#define ALU_SUB 1
#define ALU_AND 2
#define ALU_OR  3
#define ALU_XOR 4
#define ALU_SL  5
#define ALU_SR  6
#define ALU_SRU 7

SC_MODULE (alu) {
   //=========================================================================
   // Ports
   //=========================================================================
   //Inputs
   sc_in_clk            clk;     //clk
   sc_in< sc_uint<16> > a;       //src1
   sc_in< sc_uint<16> > b;       //src2
   sc_in< sc_uint<3> > cmd;     //function sel

   //Outputs
   sc_out< sc_uint<16> > r; //result

   //=========================================================================
   // Internal
   //=========================================================================
   //Variables
   sc_uint<16> data1, data2;
   sc_uint<16> result;

   //Methods
   void operate()
   {
      data1 = a.read();
      data2 = b.read();
      switch(cmd.read())
      {
         case ALU_ADD:
            result = data1 + data2;
            break;
         case ALU_SUB:
            result = data1 - data2;
            break;
         case ALU_AND:
            result = data1 & data2;
            break;
         case ALU_OR:
            result = data1 | data2;
            break;
         case ALU_XOR:
            result = data1 ^ data2;
            break;
         case ALU_SL:
            result = data1 << data2;
            break;
         case ALU_SR:  //FIXME: Check Shift Right logic
            result = data1 >> data2;
            break;
         case ALU_SRU:  //FIXME: Check Shift Right logic
            result = data1 >> data2;
            break;
         default:
            result = 0;
      }
      r.write(result);
   }

   //=========================================================================
   // Constructor
   //=========================================================================
   SC_CTOR(alu)
   : clk("clk")
   , a("a")
   , b("b")
   , cmd("cmd")
   , r("r")
   {
      SC_METHOD(operate);
         sensitive << a;
         sensitive << b;
         sensitive << cmd;
   }

   //=========================================================================
   // Traces
   //=========================================================================
   sc_trace_file *wf;
   void trace_me() {
      sc_trace(wf, clk, clk.name());
      sc_trace(wf, a, a.name());
      sc_trace(wf, b, b.name());
      sc_trace(wf, cmd, cmd.name());
      sc_trace(wf, r, r.name());
   }
};
