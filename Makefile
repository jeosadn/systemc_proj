SYSTEMC_HOME=/home/josea/local_inst
#SYSTEMC_HOME=/usr/local/systemc-2.3.2/

all: MEM_stage data_mem hazard_detection_unit register_file WB_stage ID_stage IF_stage EX_stage alu mips_16_core_top

MEM_stage:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/MEM_stage/MEM_stage_tb.cpp rtl_c/MEM_stage.cpp -lsystemc -lm -o objdir/MEM_stage.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./MEM_stage.o

data_mem:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/MEM_stage/data_mem_tb.cpp rtl_c/data_mem.cpp -lsystemc -lm -o objdir/data_mem.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./data_mem.o

mips_16_core_top:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/mips_16_core_top/mips_16_core_top_tb.cpp rtl_c/mips_16_core_top.cpp -lsystemc -lm -o objdir/mips_16_core_top.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./mips_16_core_top.o

hazard_detection_unit:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/hazard_detection_unit/hazard_detection_unit_tb.cpp rtl_c/hazard_detection_unit.cpp -lsystemc -o objdir/hazard_detection_unit.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./hazard_detection_unit.o

register_file:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/register_file/register_file_tb.cpp rtl_c/register_file.cpp -lsystemc -lm -o objdir/register_file.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./register_file.o

WB_stage:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/WB_stage/WB_stage_tb.cpp rtl_c/WB_stage.cpp -lsystemc -lm -o objdir/WB_stage.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./WB_stage.o

ID_stage:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/ID_stage/ID_stage_tb.cpp rtl_c/ID_stage.cpp -lsystemc -o objdir/ID_stage.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./ID_stage.o

IF_stage:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/IF_stage/IF_stage_tb.cpp rtl_c/IF_stage.cpp -lsystemc -o objdir/IF_stage.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./IF_stage.o

alu:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/alu_tb.cpp rtl_c/alu.cpp -lsystemc -lm -o objdir/alu.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./alu.o

EX_stage:
	mkdir -p objdir
	g++ -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib bench_c/EX_stage_tb.cpp rtl_c/EX_stage.cpp -lsystemc -lm -o objdir/EX_stage.o
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./EX_stage.o


clean:
	rm -rf objdir
