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
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ SOC__DOT__resetn;
        CData/*0:0*/ SOC__DOT__IO_mem_wr;
        CData/*0:0*/ SOC__DOT__uart_valid;
        CData/*0:0*/ SOC__DOT__uart_ready;
        CData/*0:0*/ SOC__DOT__CPU__DOT__F_stall;
        CData/*0:0*/ SOC__DOT__CPU__DOT__E_flush;
        CData/*0:0*/ SOC__DOT__CPU__DOT__halt;
        CData/*0:0*/ SOC__DOT__CPU__DOT__FD_nop;
        CData/*0:0*/ SOC__DOT__CPU__DOT__D_isJALR;
        CData/*0:0*/ SOC__DOT__CPU__DOT__D_predictBranch;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_nop;
        CData/*4:0*/ SOC__DOT__CPU__DOT__DE_rdId;
        CData/*4:0*/ SOC__DOT__CPU__DOT__DE_rs1Id;
        CData/*4:0*/ SOC__DOT__CPU__DOT__DE_rs2Id;
        CData/*1:0*/ SOC__DOT__CPU__DOT__DE_csrId;
        CData/*2:0*/ SOC__DOT__CPU__DOT__DE_funct3;
        CData/*7:0*/ SOC__DOT__CPU__DOT__DE_funct3_is;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_funct7;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isALUreg;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isBranch;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isJALR;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isLoad;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isStore;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isCSRRS;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isEBREAK;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isRV32M;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isDIV;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_wbEnable;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DE_predictBranch;
        CData/*0:0*/ SOC__DOT__CPU__DOT__E_LT;
        CData/*0:0*/ SOC__DOT__CPU__DOT__aluBusy;
        CData/*0:0*/ SOC__DOT__CPU__DOT__E_takeBranch;
        CData/*0:0*/ SOC__DOT__CPU__DOT__E_correctPC;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_nop;
        CData/*4:0*/ SOC__DOT__CPU__DOT__EM_rdId;
        CData/*3:0*/ SOC__DOT__CPU__DOT__EM_csrId_is;
        CData/*2:0*/ SOC__DOT__CPU__DOT__EM_funct3;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_isStore;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_isLoad;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_isCSRRS;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_wbEnable;
        CData/*0:0*/ SOC__DOT__CPU__DOT__EM_correctPC;
        CData/*0:0*/ SOC__DOT__CPU__DOT__MW_nop;
        CData/*4:0*/ SOC__DOT__CPU__DOT__MW_rdId;
        CData/*0:0*/ SOC__DOT__CPU__DOT__MW_wbEnable;
        CData/*0:0*/ SOC__DOT__CPU__DOT__dataHazard;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__sign_r;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__busy_r;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__finished_r;
        CData/*0:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__divstep_do;
        CData/*2:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__divsel;
        CData/*7:0*/ SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b;
        CData/*0:0*/ SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign;
        CData/*7:0*/ SOC__DOT__UART__DOT__cnt;
        SData/*11:0*/ SOC__DOT__CPU__DOT__DE_BHTindex;
        SData/*8:0*/ SOC__DOT__CPU__DOT__BP__DOT__branch_history;
        SData/*11:0*/ SOC__DOT__CPU__DOT__BP__DOT__bht_index;
        SData/*15:0*/ SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h;
        SData/*9:0*/ SOC__DOT__UART__DOT__data;
        SData/*15:0*/ SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt;
        IData/*31:0*/ SOC__DOT__inst_rdata;
        IData/*31:0*/ SOC__DOT__data_rdata;
        IData/*31:0*/ SOC__DOT__CPU__DOT__PC;
    };
    struct {
        IData/*31:0*/ SOC__DOT__CPU__DOT__F_PC;
        IData/*31:0*/ SOC__DOT__CPU__DOT__FD_PC;
        IData/*31:0*/ SOC__DOT__CPU__DOT__D_Bimm;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DE_IorSimm;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DE_PCplus4orBimm;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DE_PCplus4orUimm;
        IData/*31:0*/ SOC__DOT__CPU__DOT__E_rs1;
        IData/*31:0*/ SOC__DOT__CPU__DOT__E_rs2;
        IData/*31:0*/ SOC__DOT__CPU__DOT__E_aluIn2;
        IData/*31:0*/ SOC__DOT__CPU__DOT__E_aluPlus;
        IData/*31:0*/ SOC__DOT__CPU__DOT__E_addr;
        IData/*31:0*/ SOC__DOT__CPU__DOT__EM_rs2;
        IData/*31:0*/ SOC__DOT__CPU__DOT__EM_Eresult;
        IData/*31:0*/ SOC__DOT__CPU__DOT__EM_addr;
        IData/*31:0*/ SOC__DOT__CPU__DOT__EM_PCcorrection;
        IData/*31:0*/ SOC__DOT__CPU__DOT__M_STORE_data;
        IData/*31:0*/ SOC__DOT__CPU__DOT__MW_wbData;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DE_instr;
        IData/*31:0*/ SOC__DOT__CPU__DOT__EM_instr;
        IData/*31:0*/ SOC__DOT__CPU__DOT__MW_instr;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbBranch;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbBranchHit;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbJAL;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbJALR;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbJALRhit;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbLoad;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbStore;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbLoadHazard;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbMUL;
        IData/*31:0*/ SOC__DOT__CPU__DOT__nbDIV;
        IData/*31:0*/ SOC__DOT__CPU__DOT__BP__DOT__ras_0;
        IData/*31:0*/ SOC__DOT__CPU__DOT__BP__DOT__ras_1;
        IData/*31:0*/ SOC__DOT__CPU__DOT__BP__DOT__ras_2;
        IData/*31:0*/ SOC__DOT__CPU__DOT__BP__DOT__ras_3;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r;
        IData/*31:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
        IData/*31:0*/ SOC__DOT__CPU__DOT__ALU__DOT__shifter_in;
        IData/*31:0*/ SOC__DOT__CPU__DOT__ALU__DOT__shifter;
        IData/*31:0*/ SOC__DOT__CPU__DOT__ALU__DOT__leftshift;
        QData/*63:0*/ SOC__DOT__CPU__DOT__CSR__DOT__cycle;
        QData/*63:0*/ SOC__DOT__CPU__DOT__CSR__DOT__instret;
        QData/*62:0*/ SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r;
        QData/*32:0*/ SOC__DOT__CPU__DOT__ALU__DOT__alu_minus;
        QData/*63:0*/ SOC__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply;
        CData/*1:0*/ SOC__DOT__CPU__DOT__BP__DOT__BHT[4096];
        IData/*31:0*/ SOC__DOT__CPU__DOT__RF__DOT__regs[32];
        IData/*31:0*/ SOC__DOT__PROGROM__DOT__mem[16384];
        IData/*31:0*/ SOC__DOT__DATARAM__DOT__mem[16384];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ SOC__DOT__CPU__DOT____Vcellinp__DIVU__start;
    CData/*0:0*/ __Vclklast__TOP__CLK;
    CData/*0:0*/ __Vclklast__TOP__RESET;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x;
    
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
