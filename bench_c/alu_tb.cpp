#include "systemc.h"
#include "../rtl_c/alu.cpp"
#include <time.h>
#include <stdlib.h>

// ALU_ADD
// ALU_SUB
// ALU_AND
// ALU_OR
// ALU_XOR
// ALU_SL
// ALU_SR
// ALU_SRU

// SystemC main function (Testbench):
int sc_main(int argc, char* argv[])
{
  //=========================================================================
  // Signals
  //=========================================================================

  // Create clock (period=10 ns  duty=0.5  delay=2 ns):
  //sc_clock clk("clk",1,SC_NS,0.5,2,SC_NS,false);
  sc_clock clk("clk", 1, SC_NS, 0.5);
  sc_signal<sc_uint<16> > a;
  sc_signal<sc_uint<16> > b;
  sc_signal<sc_uint<3> > cmd;
  sc_signal<sc_uint<16> > r;

  //=========================================================================
  // DUT
  //=========================================================================
  alu DUT("alu");
    DUT.clk(clk);
    DUT.a(a);
    DUT.b(b);
    DUT.cmd(cmd);
    DUT.r(r);

  //=========================================================================
  // VCD
  //=========================================================================

  // Create VCD file (wave.vcd file for GTKWave):
  sc_trace_file *wf = sc_create_vcd_trace_file("alu_wave");
   wf->set_time_unit(1, SC_PS);
   DUT.wf = wf;
   DUT.trace_me();

  //=========================================================================
  // Stimulus
  //=========================================================================
  /*Random numbers*/
  srand(time(NULL));   // should only be called once
  uint16_t random_num = rand();
  uint16_t random_num2 = rand();

  // RETRIEVED VALUES
  uint16_t out_a;
  uint16_t out_b;
  uint16_t out_cmd;
  uint16_t out_r;
  uint16_t result;


  // SIMULATION START
  cout << "@" << sc_time_stamp() << ": " << "Start of simulation" << endl;
  sc_start(0, SC_NS);
  cout << "@" << sc_time_stamp() << ": " << "Initialize everything to 0 and wait 10 clocks" << endl;
  a=0;
  b=0;
  cmd = 0;
  sc_start(10, SC_NS);

  // TEST ADD
  //cout << "@" << sc_time_stamp() << ": " << "ALU ADD COMMAND TEST" << endl;
  a = 1;
  b = 1;
  cmd = ALU_ADD;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a + out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST SUB
  a = 5;
  b = 4;
  cmd = ALU_SUB;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a - out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST AND
  a = 6;
  b = 7;
  cmd = ALU_AND;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a & out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST OR
  a = 5;
  b = 2;
  cmd = ALU_OR;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a | out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST XOR
  a = 7;
  b = 1;
  cmd = ALU_XOR;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a ^ out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST SL
  a = 1;
  b = 3;
  cmd = ALU_SL;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a << out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST SR
  a = 4;
  b = 4;
  cmd = ALU_SR;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a >> out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // TEST SRU
  a = 3;
  b = 4;
  cmd = ALU_SRU;
  sc_start(10, SC_NS);
  out_r  = r.read().range(15,0);
  out_a  = a.read().range(15,0);
  out_b  = b.read().range(15,0);
  out_cmd  = cmd.read().range(2,0);
  result = out_a >> out_b;
  if (out_r == result) {
    cout <<"ALU op: " << out_cmd << " -> OK"<< endl;
  } else {
    cout <<"ERROR in ALU op: " << out_cmd <<", EXPECTED: ";
    cout << result <<", RETRIEVED: ";
    cout << out_r << endl;
  }

  // End of Simulation
  cout << "@" << sc_time_stamp() << ": " << "End of simulation" << endl;

  //=========================================================================
  // EOT
  //=========================================================================
  sc_close_vcd_trace_file(wf);
  return 0;
}
