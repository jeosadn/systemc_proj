/***************************************************
 * Module:
 * Project: mips_16
 * Author:
 * Description:
 *
 *
 * Revise history:
 *
 ***************************************************/
 #ifndef _MIPS_16_DEFS
 #define _MIPS_16_DEFS

    #define PC_WIDTH                8
    #define INSTR_MEM_ADDR_WIDTH    256
    #define DATA_MEM_ADDR_WIDTH     8

    /************** Operation Code in instructions ****************/
    #define OP_NOP          0
    #define OP_ADD          1
    #define OP_SUB          2
    #define OP_AND          3
    #define OP_OR           4
    #define OP_XOR          5
    #define OP_SL           6
    #define OP_SR           7
    #define OP_SRU          8
    #define OP_ADDI         9
    #define OP_LD           10
    #define OP_ST           11
    #define OP_BZ           12

    /************** ALU operation command ****************/
    #define ALU_NC          0//sc_lv<3> ('x') // not care
    #define ALU_ADD         0
    #define ALU_SUB         1
    #define ALU_AND         2
    #define ALU_OR          3
    #define ALU_XOR         4
    #define ALU_SL          5
    #define ALU_SR          6
    #define ALU_SRU         7

    /************** Branch condition code ****************/
    #define BRANCH_Z        0
    //`define BRANCH_GT     3'b001
    //`define BRANCH_LE     3'b010



#endif
