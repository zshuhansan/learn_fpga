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

class VSoc__Syms;

//----------

VL_MODULE(VSoc) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(CLK,0,0);
    VL_IN8(RESET,0,0);
    VL_OUT8(LEDS,4,0);
    VL_IN8(RXD,0,0);
    VL_OUT8(TXD,0,0);
    VL_OUT8(HALT,0,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ Soc__DOT__inst_en;
        CData/*3:0*/ Soc__DOT__data_wmask;
        CData/*0:0*/ Soc__DOT__IO_mem_wr;
        CData/*0:0*/ Soc__DOT__axi_arvalid;
        CData/*0:0*/ Soc__DOT__axi_arready;
        CData/*0:0*/ Soc__DOT__axi_rvalid;
        CData/*0:0*/ Soc__DOT__axi_rready;
        CData/*0:0*/ Soc__DOT__uart_valid;
        CData/*0:0*/ Soc__DOT__uart_ready;
        CData/*0:0*/ Soc__DOT__CPU__DOT__D_stall;
        CData/*0:0*/ Soc__DOT__CPU__DOT__E_flush;
        CData/*0:0*/ Soc__DOT__CPU__DOT__dataHazard;
        CData/*0:0*/ Soc__DOT__CPU__DOT__aluBusy;
        CData/*0:0*/ Soc__DOT__CPU__DOT__E_correctPC;
        CData/*0:0*/ Soc__DOT__CPU__DOT__E_takeBranch;
        CData/*1:0*/ Soc__DOT__CPU__DOT__F_pendingEpoch;
        CData/*0:0*/ Soc__DOT__CPU__DOT__F_pendingValid;
        CData/*1:0*/ Soc__DOT__CPU__DOT__F_epoch;
        CData/*1:0*/ Soc__DOT__CPU__DOT__FB_epoch;
        CData/*0:0*/ Soc__DOT__CPU__DOT__FB_valid;
        CData/*0:0*/ Soc__DOT__CPU__DOT__F_reqFire;
        CData/*0:0*/ Soc__DOT__CPU__DOT__F_redirect;
        CData/*1:0*/ Soc__DOT__CPU__DOT__F_epochNext;
        CData/*0:0*/ Soc__DOT__CPU__DOT__FD_nop;
        CData/*0:0*/ Soc__DOT__CPU__DOT__D_isJALR;
        CData/*0:0*/ Soc__DOT__CPU__DOT__D_predictPC;
        CData/*0:0*/ Soc__DOT__CPU__DOT__D_predictBranch;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_nop;
        CData/*4:0*/ Soc__DOT__CPU__DOT__DE_rdId;
        CData/*4:0*/ Soc__DOT__CPU__DOT__DE_rs1Id;
        CData/*4:0*/ Soc__DOT__CPU__DOT__DE_rs2Id;
        CData/*1:0*/ Soc__DOT__CPU__DOT__DE_csrId;
        CData/*2:0*/ Soc__DOT__CPU__DOT__DE_funct3;
        CData/*7:0*/ Soc__DOT__CPU__DOT__DE_funct3_is;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_funct7;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isALUreg;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isBranch;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isJALR;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isJAL;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isLoad;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isStore;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isCSRRS;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isEBREAK;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isRV32M;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isDIV;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_wbEnable;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DE_predictBranch;
        CData/*0:0*/ Soc__DOT__CPU__DOT__E_LT;
        CData/*0:0*/ Soc__DOT__CPU__DOT__EM_nop;
        CData/*4:0*/ Soc__DOT__CPU__DOT__EM_rdId;
        CData/*3:0*/ Soc__DOT__CPU__DOT__EM_csrId_is;
        CData/*2:0*/ Soc__DOT__CPU__DOT__EM_funct3;
        CData/*0:0*/ Soc__DOT__CPU__DOT__EM_isStore;
        CData/*0:0*/ Soc__DOT__CPU__DOT__EM_isLoad;
        CData/*0:0*/ Soc__DOT__CPU__DOT__EM_isCSRRS;
        CData/*0:0*/ Soc__DOT__CPU__DOT__EM_wbEnable;
        CData/*0:0*/ Soc__DOT__CPU__DOT__MW_nop;
        CData/*4:0*/ Soc__DOT__CPU__DOT__MW_rdId;
        CData/*0:0*/ Soc__DOT__CPU__DOT__MW_wbEnable;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__sign_r;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__busy_r;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__finished_r;
        CData/*0:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__divstep_do;
    };
    struct {
        CData/*2:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__divsel;
        CData/*7:0*/ Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b;
        CData/*0:0*/ Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__hit_way0;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__hit_way1_raw;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__cache_hit;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__is_hit;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__is_miss;
        CData/*2:0*/ Soc__DOT__ICACHE__DOT__state;
        CData/*2:0*/ Soc__DOT__ICACHE__DOT__burst_cnt;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__replace_way;
        CData/*2:0*/ Soc__DOT__ICACHE__DOT__miss_word_offset;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__resp_valid_r;
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__stats_printed;
        CData/*1:0*/ Soc__DOT__AXI_DDR__DOT__state;
        CData/*7:0*/ Soc__DOT__AXI_DDR__DOT__burst_left;
        CData/*3:0*/ Soc__DOT__AXI_DDR__DOT__latency_cnt;
        CData/*7:0*/ Soc__DOT__UART__DOT__cnt;
        SData/*11:0*/ Soc__DOT__CPU__DOT__DE_BHTindex;
        SData/*8:0*/ Soc__DOT__CPU__DOT__BP__DOT__branch_history;
        SData/*11:0*/ Soc__DOT__CPU__DOT__BP__DOT__bht_index;
        SData/*15:0*/ Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h;
        SData/*9:0*/ Soc__DOT__UART__DOT__data;
        IData/*31:0*/ Soc__DOT__data_rdata;
        IData/*31:0*/ Soc__DOT__axi_rdata;
        IData/*31:0*/ Soc__DOT__MBOX0;
        IData/*31:0*/ Soc__DOT__MBOX1;
        IData/*31:0*/ Soc__DOT__MBOX2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__PC;
        IData/*31:0*/ Soc__DOT__CPU__DOT__F_pendingPC;
        IData/*31:0*/ Soc__DOT__CPU__DOT__FB_PC;
        IData/*31:0*/ Soc__DOT__CPU__DOT__FB_instr;
        IData/*31:0*/ Soc__DOT__CPU__DOT__F_PC;
        IData/*31:0*/ Soc__DOT__CPU__DOT__FD_PC;
        IData/*31:0*/ Soc__DOT__CPU__DOT__FD_instr_reg;
        IData/*31:0*/ Soc__DOT__CPU__DOT__D_Bimm;
        IData/*31:0*/ Soc__DOT__CPU__DOT__D_PCprediction;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DE_IorSimm;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DE_PCplus4orBimm;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DE_predictRA;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DE_PCplus4orUimm;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_rs1;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_rs2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_aluIn2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_aluPlus;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_PCcorrection;
        IData/*31:0*/ Soc__DOT__CPU__DOT__E_addr;
        IData/*31:0*/ Soc__DOT__CPU__DOT__EM_rs2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__EM_Eresult;
        IData/*31:0*/ Soc__DOT__CPU__DOT__EM_addr;
        IData/*31:0*/ Soc__DOT__CPU__DOT__M_STORE_data;
        IData/*31:0*/ Soc__DOT__CPU__DOT__MW_wbData;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
        IData/*31:0*/ Soc__DOT__CPU__DOT__BP__DOT__i;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r;
        IData/*31:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    };
    struct {
        IData/*31:0*/ Soc__DOT__CPU__DOT__ALU__DOT__shifter_in;
        IData/*31:0*/ Soc__DOT__CPU__DOT__ALU__DOT__shifter;
        IData/*31:0*/ Soc__DOT__CPU__DOT__ALU__DOT__leftshift;
        WData/*255:0*/ Soc__DOT__ICACHE__DOT__cache_line[8];
        WData/*255:0*/ Soc__DOT__ICACHE__DOT__refill_line[8];
        IData/*31:0*/ Soc__DOT__ICACHE__DOT__miss_addr;
        IData/*31:0*/ Soc__DOT__ICACHE__DOT__resp_data_r;
        IData/*31:0*/ Soc__DOT__ICACHE__DOT__hit_count;
        IData/*31:0*/ Soc__DOT__ICACHE__DOT__miss_count;
        IData/*31:0*/ Soc__DOT__ICACHE__DOT__i;
        IData/*31:0*/ Soc__DOT__AXI_DDR__DOT__i;
        IData/*31:0*/ Soc__DOT__AXI_DDR__DOT__current_addr;
        QData/*63:0*/ Soc__DOT__CPU__DOT__CSR__DOT__cycle;
        QData/*63:0*/ Soc__DOT__CPU__DOT__CSR__DOT__instret;
        QData/*62:0*/ Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r;
        QData/*32:0*/ Soc__DOT__CPU__DOT__ALU__DOT__alu_minus;
        QData/*63:0*/ Soc__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply;
        CData/*1:0*/ Soc__DOT__CPU__DOT__BP__DOT__BHT[4096];
        IData/*31:0*/ Soc__DOT__CPU__DOT__RF__DOT__regs[32];
        WData/*255:0*/ Soc__DOT__ICACHE__DOT__data_array_way0[128][8];
        IData/*19:0*/ Soc__DOT__ICACHE__DOT__tag_array_way0[128];
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__valid_array_way0[128];
        WData/*255:0*/ Soc__DOT__ICACHE__DOT__data_array_way1[128][8];
        IData/*19:0*/ Soc__DOT__ICACHE__DOT__tag_array_way1[128];
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__valid_array_way1[128];
        CData/*0:0*/ Soc__DOT__ICACHE__DOT__lru_array[128];
        IData/*31:0*/ Soc__DOT__AXI_DDR__DOT__memory[262144];
        IData/*31:0*/ Soc__DOT__DATARAM__DOT__mem[16384];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ Soc__DOT__CPU__DOT____Vcellinp__DIVU__start;
    CData/*0:0*/ __Vclklast__TOP__CLK;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__8__Vfuncout;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__8__x;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VSoc__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VSoc);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VSoc(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VSoc();
    
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
    static void _eval_initial_loop(VSoc__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VSoc__Syms* symsp, bool first);
  private:
    static QData _change_request(VSoc__Syms* __restrict vlSymsp);
    static QData _change_request_1(VSoc__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__4(VSoc__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(VSoc__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(VSoc__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(VSoc__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__1(VSoc__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(VSoc__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(VSoc__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
