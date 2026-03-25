// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSOC_H_
#define _VSOC_H_  // guard

#include "verilated_heavy.h"

//==========

class VSOC__Syms;

//----------

VL_MODULE(VSOC) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(CLK,0,0);
    VL_IN8(RESET,0,0);
    VL_OUT8(LEDS,4,0);
    VL_IN8(RXD,0,0);
    VL_OUT8(TXD,0,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ SOC__DOT__resetn;
    CData/*0:0*/ SOC__DOT__IO_mem_wr;
    CData/*0:0*/ SOC__DOT__uart_valid;
    CData/*0:0*/ SOC__DOT__uart_ready;
    CData/*0:0*/ SOC__DOT__CPU__DOT__E_flush;
    CData/*0:0*/ SOC__DOT__CPU__DOT__D_stall;
    CData/*0:0*/ SOC__DOT__CPU__DOT__halt;
    CData/*0:0*/ SOC__DOT__CPU__DOT__FD_nop;
    CData/*0:0*/ SOC__DOT__CPU__DOT__wbEnable;
    CData/*4:0*/ SOC__DOT__CPU__DOT__wbRdId;
    CData/*0:0*/ SOC__DOT__CPU__DOT__E_minus;
    CData/*0:0*/ SOC__DOT__CPU__DOT__E_LT;
    CData/*0:0*/ SOC__DOT__CPU__DOT__E_takeBranch;
    CData/*0:0*/ SOC__DOT__CPU__DOT__E_JumpOrBranch;
    CData/*2:0*/ SOC__DOT__CPU__DOT__M_funct3;
    CData/*3:0*/ SOC__DOT__CPU__DOT__M_wmask;
    CData/*2:0*/ SOC__DOT__CPU__DOT__W_funct3;
    CData/*7:0*/ SOC__DOT__CPU__DOT__W_LOAD_B;
    CData/*0:0*/ SOC__DOT__CPU__DOT__W_LOAD_sign;
    CData/*0:0*/ SOC__DOT__CPU__DOT__rs1Hazard;
    CData/*0:0*/ SOC__DOT__CPU__DOT__rs2Hazard;
    CData/*0:0*/ SOC__DOT__CPU__DOT__dataHazard;
    CData/*7:0*/ SOC__DOT__UART__DOT__cnt;
    SData/*15:0*/ SOC__DOT__CPU__DOT__W_LOAD_H;
    SData/*9:0*/ SOC__DOT__UART__DOT__data;
    SData/*15:0*/ SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt;
    IData/*31:0*/ SOC__DOT__CPU__DOT__F_PC;
    IData/*31:0*/ SOC__DOT__CPU__DOT__FD_PC;
    IData/*31:0*/ SOC__DOT__CPU__DOT__FD_instr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__wbData;
    IData/*31:0*/ SOC__DOT__CPU__DOT__DE_PC;
    IData/*31:0*/ SOC__DOT__CPU__DOT__DE_instr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__DE_rs1;
    IData/*31:0*/ SOC__DOT__CPU__DOT__DE_rs2;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_aluIn2;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_aluPlus;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_shifter_in;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_shifter;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_leftshift;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_aluOut;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_JumpOrBranchAddr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__E_result;
    IData/*31:0*/ SOC__DOT__CPU__DOT__EM_PC;
    IData/*31:0*/ SOC__DOT__CPU__DOT__EM_instr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__EM_rs2;
    IData/*31:0*/ SOC__DOT__CPU__DOT__EM_Eresult;
    IData/*31:0*/ SOC__DOT__CPU__DOT__EM_addr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__M_STORE_data;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_PC;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_instr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_Eresult;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_addr;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_Mdata;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_IOresult;
    IData/*31:0*/ SOC__DOT__CPU__DOT__MW_CSRresult;
    QData/*63:0*/ SOC__DOT__CPU__DOT__cycle;
    QData/*63:0*/ SOC__DOT__CPU__DOT__instret;
    QData/*32:0*/ SOC__DOT__CPU__DOT__E_aluMinus;
    IData/*31:0*/ SOC__DOT__CPU__DOT__PROGROM[16384];
    IData/*31:0*/ SOC__DOT__CPU__DOT__RegisterBank[32];
    IData/*31:0*/ SOC__DOT__CPU__DOT__DATARAM[16384];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__3__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__8__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__11__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__12__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__13__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__14__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__15__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__16__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__17__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__18__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__19__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__20__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__21__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__22__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__23__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__24__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJALR__25__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__26__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__27__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__29__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__31__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJALR__32__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLUI__33__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__42__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__43__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__44__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__45__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__49__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLoad__50__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__Vfuncout;
        CData/*2:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__53__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__54__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__55__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__56__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__57__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__61__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__63__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__64__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__66__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__67__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__69__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__70__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__71__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__72__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__73__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__74__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__75__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__76__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLUI__78__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__Vfuncout;
    };
    struct {
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__81__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__83__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__84__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__86__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__87__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__Vfuncout;
        CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__89__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__90__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__91__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__92__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__93__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__94__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__95__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__97__Vfuncout;
        CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__98__Vfuncout;
        CData/*0:0*/ __Vclklast__TOP__CLK;
        CData/*0:0*/ __Vclklast__TOP__RESET;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__3__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Iimm__4__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Iimm__4__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__8__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__flip32__9__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__flip32__9__x;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__flip32__10__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__flip32__10__x;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__11__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__12__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__13__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__14__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__15__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__16__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__17__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__18__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__19__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__20__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__21__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__22__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__23__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__24__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJALR__25__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__26__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__27__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Bimm__28__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__29__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Jimm__30__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__31__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJALR__32__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLUI__33__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Uimm__34__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Uimm__34__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Uimm__36__Vfuncout;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Uimm__36__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__42__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__43__I;
    };
    struct {
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__44__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__45__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__49__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLoad__50__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__funct3__53__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__54__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__55__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__56__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__57__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__58__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__61__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__63__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__64__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__66__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__67__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__69__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__70__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__71__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__72__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__73__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__74__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__75__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isJAL__76__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isLUI__78__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__81__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__83__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__84__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__86__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__writesRd__87__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__89__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__90__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__91__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__92__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__93__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__94__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__95__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isBranch__97__I;
        IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__98__I;
    };
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VSOC__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VSOC);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VSOC(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VSOC();
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VSOC__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VSOC__Syms* symsp, bool first);
  private:
    static QData _change_request(VSOC__Syms* __restrict vlSymsp);
    static QData _change_request_1(VSOC__Syms* __restrict vlSymsp);
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(VSOC__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(VSOC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(VSOC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__3(VSOC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(VSOC__Syms* __restrict vlSymsp);
    static void _sequent__TOP__2(VSOC__Syms* __restrict vlSymsp);
    static void _settle__TOP__4(VSOC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
