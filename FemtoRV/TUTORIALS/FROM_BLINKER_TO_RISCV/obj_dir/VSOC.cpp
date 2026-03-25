// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSOC.h for the primary calling header

#include "VSOC.h"
#include "VSOC__Syms.h"

//==========

void VSOC::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VSOC::eval\n"); );
    VSOC__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("pipeline4.v", 537, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VSOC::_eval_initial_loop(VSOC__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("pipeline4.v", 537, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void VSOC::_sequent__TOP__1(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_sequent__TOP__1\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__Vfuncout;
    CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__Vfuncout;
    CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__37__Vfuncout;
    CData/*1:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__46__Vfuncout;
    CData/*1:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__47__Vfuncout;
    CData/*1:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__48__Vfuncout;
    CData/*4:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__110__Vfuncout;
    CData/*4:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__RegisterBank__v0;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__RegisterBank__v0;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v0;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v0;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v0;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v1;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v1;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v1;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v2;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v2;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v2;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v3;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v3;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v3;
    CData/*0:0*/ __Vdly__SOC__DOT__uart_ready;
    CData/*7:0*/ __Vdly__SOC__DOT__UART__DOT__cnt;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v0;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v1;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v2;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v3;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__isStore__37__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Simm__38__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Simm__38__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Iimm__39__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__Iimm__39__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__46__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__47__I;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__csrId__48__I;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__PC;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__rdId__110__I;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__PC;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__PC;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
    IData/*31:0*/ __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__PC;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__instr;
    IData/*19:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__instr;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__Vfuncout;
    IData/*31:0*/ __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__instr;
    IData/*31:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__RegisterBank__v0;
    QData/*63:0*/ __Vdly__SOC__DOT__CPU__DOT__cycle;
    // Body
    __Vdly__SOC__DOT__CPU__DOT__cycle = vlTOPp->SOC__DOT__CPU__DOT__cycle;
    __Vdlyvset__SOC__DOT__CPU__DOT__RegisterBank__v0 = 0U;
    __Vdly__SOC__DOT__uart_ready = vlTOPp->SOC__DOT__uart_ready;
    __Vdly__SOC__DOT__UART__DOT__cnt = vlTOPp->SOC__DOT__UART__DOT__cnt;
    __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v0 = 0U;
    __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v1 = 0U;
    __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v2 = 0U;
    __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v3 = 0U;
    if (VL_UNLIKELY(vlTOPp->SOC__DOT__CPU__DOT__halt)) {
        VL_FINISH_MT("pipeline4.v", 492, "");
    }
    if (VL_UNLIKELY(vlTOPp->SOC__DOT__uart_valid)) {
        VL_WRITEF("UART: %c\n",8,(0xffU & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
    }
    if (VL_UNLIKELY((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt)))) {
        VL_WRITEF("[W] PC=%x ",32,vlTOPp->SOC__DOT__CPU__DOT__MW_PC);
        VL_WRITEF("     ");
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__PC 
            = vlTOPp->SOC__DOT__CPU__DOT__MW_PC;
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
            = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
        if ((0x40U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
            if (VL_LIKELY((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("ebreak");
                                    } else {
                                        if (VL_LIKELY(
                                                      (2U 
                                                       == 
                                                       (7U 
                                                        & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                           >> 0xcU))))) {
                                            if ((0x8000000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    VL_WRITEF("rdinstreth x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycleh x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                                  >> 7U)));
                                                }
                                            } else {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    VL_WRITEF("rdinstret x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycle x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                                  >> 7U)));
                                                }
                                            }
                                        } else {
                                            VL_WRITEF("SYSTEM");
                                        }
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if ((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                        if (VL_LIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__Vfuncout 
                                        = ((0xfff00000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr 
                                                              >> 0x1fU)))) 
                                               << 0x14U)) 
                                           | ((0xff000U 
                                               & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr) 
                                              | ((0x800U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr 
                                                     >> 9U)) 
                                                 | (0x7feU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__instr 
                                                       >> 0x14U)))));
                                    VL_WRITEF("jal x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 7U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__100__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            VL_WRITEF("?????");
                        }
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF("jalr x%0#,x%0#,%0d",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 7U)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0xfU)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__101__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("bgeu");
                                            } else {
                                                VL_WRITEF("bltu");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("bge");
                                            } else {
                                                VL_WRITEF("blt");
                                            }
                                        }
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                            VL_WRITEF("B???");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("bne");
                                            } else {
                                                VL_WRITEF("beq");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__Vfuncout 
                                        = ((0xfffff000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr 
                                                              >> 0x1fU)))) 
                                               << 0xcU)) 
                                           | ((0x800U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr 
                                                  << 4U)) 
                                              | ((0x7e0U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__instr 
                                                       >> 7U)))));
                                    VL_WRITEF(" x%0#,x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0xfU)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0x14U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__102__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                VL_WRITEF("?????");
            }
        } else {
            if ((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__instr 
                                              >> 0xcU));
                                    VL_WRITEF("lui x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__103__Vfuncout,
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__104__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                    if ((0U == (0x1ffffffU 
                                                & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                   >> 7U)))) {
                                        VL_WRITEF("nop");
                                    } else {
                                        if ((0x2000000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("remu");
                                                    } else {
                                                        VL_WRITEF("rem");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("divu");
                                                    } else {
                                                        VL_WRITEF("div");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("mulhu");
                                                    } else {
                                                        VL_WRITEF("mulhsu");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("mulh");
                                                    } else {
                                                        VL_WRITEF("mul");
                                                    }
                                                }
                                            }
                                        } else {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("and");
                                                    } else {
                                                        VL_WRITEF("or");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)
                                                                    ? 0x737261U
                                                                    : 0x73726cU));
                                                    } else {
                                                        VL_WRITEF("xor");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("sltu");
                                                    } else {
                                                        VL_WRITEF("slt");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                        VL_WRITEF("sll");
                                                    } else {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)
                                                                    ? 0x737562U
                                                                    : 0x616464U));
                                                    }
                                                }
                                            }
                                        }
                                        VL_WRITEF(" x%0#,x%0#,x%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0x14U)));
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("sb");
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (1U 
                                                         == 
                                                         (7U 
                                                          & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                             >> 0xcU))))) {
                                            VL_WRITEF("sh");
                                        } else {
                                            if ((2U 
                                                 == 
                                                 (7U 
                                                  & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                     >> 0xcU)))) {
                                                VL_WRITEF("sw");
                                            } else {
                                                VL_WRITEF("s??");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | ((0x7e0U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__instr 
                                                  >> 0x14U)) 
                                              | (0x1fU 
                                                 & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__instr 
                                                    >> 7U))));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0x14U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__105__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__instr 
                                              >> 0xcU));
                                    VL_WRITEF("auipc x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__106__Vfuncout,
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__107__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("andi");
                                            } else {
                                                VL_WRITEF("ori");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("%s",
                                                          32,
                                                          ((0x40000000U 
                                                            & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)
                                                            ? 0x73726169U
                                                            : 0x73726c69U));
                                            } else {
                                                VL_WRITEF("xori");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("sltiu");
                                            } else {
                                                VL_WRITEF("slti");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("slli");
                                            } else {
                                                VL_WRITEF("addi");
                                            }
                                        }
                                    }
                                    if (((1U == (7U 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0xcU))) 
                                         | (5U == (7U 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0xcU))))) {
                                        VL_WRITEF(" x%0#,x%0#,%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0x14U)));
                                    } else {
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__instr 
                                            = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__Vfuncout 
                                            = ((0xfffff800U 
                                                & ((- (IData)(
                                                              (1U 
                                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__instr 
                                                                  >> 0x1fU)))) 
                                                   << 0xbU)) 
                                               | (0x7ffU 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__instr 
                                                     >> 0x14U)));
                                        VL_WRITEF(" x%0#,x%0#,%0d",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                      >> 0xfU)),
                                                  32,
                                                  __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__108__Vfuncout);
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr))) {
                                            VL_WRITEF("l??");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("lhu");
                                            } else {
                                                VL_WRITEF("lbu");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("l??");
                                            } else {
                                                VL_WRITEF("lw");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr)) {
                                                VL_WRITEF("lh");
                                            } else {
                                                VL_WRITEF("lb");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 7U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__109__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__99__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            }
        }
        if (VL_UNLIKELY(vlTOPp->SOC__DOT__CPU__DOT__wbEnable)) {
            __Vfunc_SOC__DOT__CPU__DOT__rdId__110__I 
                = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
            __Vfunc_SOC__DOT__CPU__DOT__rdId__110__Vfuncout 
                = (0x1fU & (__Vfunc_SOC__DOT__CPU__DOT__rdId__110__I 
                            >> 7U));
            VL_WRITEF("    x%0# <- 0x%0x",5,__Vfunc_SOC__DOT__CPU__DOT__rdId__110__Vfuncout,
                      32,vlTOPp->SOC__DOT__CPU__DOT__wbData);
        }
        VL_WRITEF("\n");
        VL_WRITEF("[M] PC=%x ",32,vlTOPp->SOC__DOT__CPU__DOT__EM_PC);
        VL_WRITEF("     ");
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__PC 
            = vlTOPp->SOC__DOT__CPU__DOT__EM_PC;
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
            = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
        if ((0x40U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
            if (VL_LIKELY((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("ebreak");
                                    } else {
                                        if (VL_LIKELY(
                                                      (2U 
                                                       == 
                                                       (7U 
                                                        & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                           >> 0xcU))))) {
                                            if ((0x8000000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    VL_WRITEF("rdinstreth x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycleh x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                                  >> 7U)));
                                                }
                                            } else {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    VL_WRITEF("rdinstret x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycle x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                                  >> 7U)));
                                                }
                                            }
                                        } else {
                                            VL_WRITEF("SYSTEM");
                                        }
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if ((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                        if (VL_LIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__Vfuncout 
                                        = ((0xfff00000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr 
                                                              >> 0x1fU)))) 
                                               << 0x14U)) 
                                           | ((0xff000U 
                                               & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr) 
                                              | ((0x800U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr 
                                                     >> 9U)) 
                                                 | (0x7feU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__instr 
                                                       >> 0x14U)))));
                                    VL_WRITEF("jal x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 7U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__112__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            VL_WRITEF("?????");
                        }
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF("jalr x%0#,x%0#,%0d",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 7U)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0xfU)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__113__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("bgeu");
                                            } else {
                                                VL_WRITEF("bltu");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("bge");
                                            } else {
                                                VL_WRITEF("blt");
                                            }
                                        }
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                            VL_WRITEF("B???");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("bne");
                                            } else {
                                                VL_WRITEF("beq");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__Vfuncout 
                                        = ((0xfffff000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr 
                                                              >> 0x1fU)))) 
                                               << 0xcU)) 
                                           | ((0x800U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr 
                                                  << 4U)) 
                                              | ((0x7e0U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__instr 
                                                       >> 7U)))));
                                    VL_WRITEF(" x%0#,x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0xfU)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0x14U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__114__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                VL_WRITEF("?????");
            }
        } else {
            if ((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__instr 
                                              >> 0xcU));
                                    VL_WRITEF("lui x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__115__Vfuncout,
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__116__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                    if ((0U == (0x1ffffffU 
                                                & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                   >> 7U)))) {
                                        VL_WRITEF("nop");
                                    } else {
                                        if ((0x2000000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("remu");
                                                    } else {
                                                        VL_WRITEF("rem");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("divu");
                                                    } else {
                                                        VL_WRITEF("div");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("mulhu");
                                                    } else {
                                                        VL_WRITEF("mulhsu");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("mulh");
                                                    } else {
                                                        VL_WRITEF("mul");
                                                    }
                                                }
                                            }
                                        } else {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("and");
                                                    } else {
                                                        VL_WRITEF("or");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)
                                                                    ? 0x737261U
                                                                    : 0x73726cU));
                                                    } else {
                                                        VL_WRITEF("xor");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("sltu");
                                                    } else {
                                                        VL_WRITEF("slt");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                        VL_WRITEF("sll");
                                                    } else {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)
                                                                    ? 0x737562U
                                                                    : 0x616464U));
                                                    }
                                                }
                                            }
                                        }
                                        VL_WRITEF(" x%0#,x%0#,x%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0x14U)));
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("sb");
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (1U 
                                                         == 
                                                         (7U 
                                                          & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                             >> 0xcU))))) {
                                            VL_WRITEF("sh");
                                        } else {
                                            if ((2U 
                                                 == 
                                                 (7U 
                                                  & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                     >> 0xcU)))) {
                                                VL_WRITEF("sw");
                                            } else {
                                                VL_WRITEF("s??");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | ((0x7e0U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__instr 
                                                  >> 0x14U)) 
                                              | (0x1fU 
                                                 & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__instr 
                                                    >> 7U))));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0x14U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__117__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__instr 
                                              >> 0xcU));
                                    VL_WRITEF("auipc x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__118__Vfuncout,
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__119__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("andi");
                                            } else {
                                                VL_WRITEF("ori");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("%s",
                                                          32,
                                                          ((0x40000000U 
                                                            & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)
                                                            ? 0x73726169U
                                                            : 0x73726c69U));
                                            } else {
                                                VL_WRITEF("xori");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("sltiu");
                                            } else {
                                                VL_WRITEF("slti");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("slli");
                                            } else {
                                                VL_WRITEF("addi");
                                            }
                                        }
                                    }
                                    if (((1U == (7U 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0xcU))) 
                                         | (5U == (7U 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0xcU))))) {
                                        VL_WRITEF(" x%0#,x%0#,%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0x14U)));
                                    } else {
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__instr 
                                            = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__Vfuncout 
                                            = ((0xfffff800U 
                                                & ((- (IData)(
                                                              (1U 
                                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__instr 
                                                                  >> 0x1fU)))) 
                                                   << 0xbU)) 
                                               | (0x7ffU 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__instr 
                                                     >> 0x14U)));
                                        VL_WRITEF(" x%0#,x%0#,%0d",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                      >> 0xfU)),
                                                  32,
                                                  __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__120__Vfuncout);
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr))) {
                                            VL_WRITEF("l??");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("lhu");
                                            } else {
                                                VL_WRITEF("lbu");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("l??");
                                            } else {
                                                VL_WRITEF("lw");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr)) {
                                                VL_WRITEF("lh");
                                            } else {
                                                VL_WRITEF("lb");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 7U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__121__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__111__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            }
        }
        VL_WRITEF("\n");
        VL_WRITEF("[E] PC=%x ",32,vlTOPp->SOC__DOT__CPU__DOT__DE_PC);
        VL_WRITEF("     ");
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__PC 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_PC;
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
        if ((0x40U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
            if (VL_LIKELY((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("ebreak");
                                    } else {
                                        if (VL_LIKELY(
                                                      (2U 
                                                       == 
                                                       (7U 
                                                        & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                           >> 0xcU))))) {
                                            if ((0x8000000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    VL_WRITEF("rdinstreth x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycleh x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                                  >> 7U)));
                                                }
                                            } else {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    VL_WRITEF("rdinstret x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycle x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                                  >> 7U)));
                                                }
                                            }
                                        } else {
                                            VL_WRITEF("SYSTEM");
                                        }
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if ((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                        if (VL_LIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__Vfuncout 
                                        = ((0xfff00000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr 
                                                              >> 0x1fU)))) 
                                               << 0x14U)) 
                                           | ((0xff000U 
                                               & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr) 
                                              | ((0x800U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr 
                                                     >> 9U)) 
                                                 | (0x7feU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__instr 
                                                       >> 0x14U)))));
                                    VL_WRITEF("jal x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 7U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__123__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            VL_WRITEF("?????");
                        }
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF("jalr x%0#,x%0#,%0d",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 7U)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0xfU)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__124__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("bgeu");
                                            } else {
                                                VL_WRITEF("bltu");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("bge");
                                            } else {
                                                VL_WRITEF("blt");
                                            }
                                        }
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                            VL_WRITEF("B???");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("bne");
                                            } else {
                                                VL_WRITEF("beq");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__Vfuncout 
                                        = ((0xfffff000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr 
                                                              >> 0x1fU)))) 
                                               << 0xcU)) 
                                           | ((0x800U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr 
                                                  << 4U)) 
                                              | ((0x7e0U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__instr 
                                                       >> 7U)))));
                                    VL_WRITEF(" x%0#,x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0xfU)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0x14U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__125__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                VL_WRITEF("?????");
            }
        } else {
            if ((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__instr 
                                              >> 0xcU));
                                    VL_WRITEF("lui x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__126__Vfuncout,
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__127__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                    if ((0U == (0x1ffffffU 
                                                & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                   >> 7U)))) {
                                        VL_WRITEF("nop");
                                    } else {
                                        if ((0x2000000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("remu");
                                                    } else {
                                                        VL_WRITEF("rem");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("divu");
                                                    } else {
                                                        VL_WRITEF("div");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("mulhu");
                                                    } else {
                                                        VL_WRITEF("mulhsu");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("mulh");
                                                    } else {
                                                        VL_WRITEF("mul");
                                                    }
                                                }
                                            }
                                        } else {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("and");
                                                    } else {
                                                        VL_WRITEF("or");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)
                                                                    ? 0x737261U
                                                                    : 0x73726cU));
                                                    } else {
                                                        VL_WRITEF("xor");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("sltu");
                                                    } else {
                                                        VL_WRITEF("slt");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                        VL_WRITEF("sll");
                                                    } else {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)
                                                                    ? 0x737562U
                                                                    : 0x616464U));
                                                    }
                                                }
                                            }
                                        }
                                        VL_WRITEF(" x%0#,x%0#,x%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0x14U)));
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("sb");
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (1U 
                                                         == 
                                                         (7U 
                                                          & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                             >> 0xcU))))) {
                                            VL_WRITEF("sh");
                                        } else {
                                            if ((2U 
                                                 == 
                                                 (7U 
                                                  & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                     >> 0xcU)))) {
                                                VL_WRITEF("sw");
                                            } else {
                                                VL_WRITEF("s??");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | ((0x7e0U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__instr 
                                                  >> 0x14U)) 
                                              | (0x1fU 
                                                 & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__instr 
                                                    >> 7U))));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0x14U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__128__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__instr 
                                              >> 0xcU));
                                    VL_WRITEF("auipc x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__129__Vfuncout,
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__130__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("andi");
                                            } else {
                                                VL_WRITEF("ori");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("%s",
                                                          32,
                                                          ((0x40000000U 
                                                            & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)
                                                            ? 0x73726169U
                                                            : 0x73726c69U));
                                            } else {
                                                VL_WRITEF("xori");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("sltiu");
                                            } else {
                                                VL_WRITEF("slti");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("slli");
                                            } else {
                                                VL_WRITEF("addi");
                                            }
                                        }
                                    }
                                    if (((1U == (7U 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0xcU))) 
                                         | (5U == (7U 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0xcU))))) {
                                        VL_WRITEF(" x%0#,x%0#,%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0x14U)));
                                    } else {
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__instr 
                                            = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__Vfuncout 
                                            = ((0xfffff800U 
                                                & ((- (IData)(
                                                              (1U 
                                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__instr 
                                                                  >> 0x1fU)))) 
                                                   << 0xbU)) 
                                               | (0x7ffU 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__instr 
                                                     >> 0x14U)));
                                        VL_WRITEF(" x%0#,x%0#,%0d",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                      >> 0xfU)),
                                                  32,
                                                  __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__131__Vfuncout);
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr))) {
                                            VL_WRITEF("l??");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("lhu");
                                            } else {
                                                VL_WRITEF("lbu");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("l??");
                                            } else {
                                                VL_WRITEF("lw");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr)) {
                                                VL_WRITEF("lh");
                                            } else {
                                                VL_WRITEF("lb");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 7U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__132__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__122__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            }
        }
        if (VL_UNLIKELY((0x33U != vlTOPp->SOC__DOT__CPU__DOT__DE_instr))) {
            VL_WRITEF("  rs1=0x%x  rs2=0x%x  ",32,vlTOPp->SOC__DOT__CPU__DOT__DE_rs1,
                      32,vlTOPp->SOC__DOT__CPU__DOT__DE_rs2);
        }
        VL_WRITEF("\n");
        VL_WRITEF("[D] PC=%x ",32,vlTOPp->SOC__DOT__CPU__DOT__FD_PC);
        VL_WRITEF("[%s%s] ",8,((IData)(vlTOPp->SOC__DOT__CPU__DOT__rs1Hazard)
                                ? 0x2aU : 0x20U),8,
                  ((IData)(vlTOPp->SOC__DOT__CPU__DOT__rs2Hazard)
                    ? 0x2aU : 0x20U));
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__PC 
            = vlTOPp->SOC__DOT__CPU__DOT__FD_PC;
        __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
            = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)
                ? 0x33U : vlTOPp->SOC__DOT__CPU__DOT__FD_instr);
        if ((0x40U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
            if (VL_LIKELY((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("ebreak");
                                    } else {
                                        if (VL_LIKELY(
                                                      (2U 
                                                       == 
                                                       (7U 
                                                        & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                           >> 0xcU))))) {
                                            if ((0x8000000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    VL_WRITEF("rdinstreth x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycleh x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                                  >> 7U)));
                                                }
                                            } else {
                                                if (
                                                    (0x200000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    VL_WRITEF("rdinstret x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                                  >> 7U)));
                                                } else {
                                                    VL_WRITEF("rdcycle x%0#",
                                                              5,
                                                              (0x1fU 
                                                               & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                                  >> 7U)));
                                                }
                                            }
                                        } else {
                                            VL_WRITEF("SYSTEM");
                                        }
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if ((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                        if (VL_LIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__Vfuncout 
                                        = ((0xfff00000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr 
                                                              >> 0x1fU)))) 
                                               << 0x14U)) 
                                           | ((0xff000U 
                                               & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr) 
                                              | ((0x800U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr 
                                                     >> 9U)) 
                                                 | (0x7feU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__instr 
                                                       >> 0x14U)))));
                                    VL_WRITEF("jal x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 7U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Jimm__134__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            VL_WRITEF("?????");
                        }
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF("jalr x%0#,x%0#,%0d",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 7U)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0xfU)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__135__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("bgeu");
                                            } else {
                                                VL_WRITEF("bltu");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("bge");
                                            } else {
                                                VL_WRITEF("blt");
                                            }
                                        }
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                            VL_WRITEF("B???");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("bne");
                                            } else {
                                                VL_WRITEF("beq");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__Vfuncout 
                                        = ((0xfffff000U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr 
                                                              >> 0x1fU)))) 
                                               << 0xcU)) 
                                           | ((0x800U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr 
                                                  << 4U)) 
                                              | ((0x7e0U 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__instr 
                                                       >> 7U)))));
                                    VL_WRITEF(" x%0#,x%0#,0x%0x",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0xfU)),
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0x14U)),
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Bimm__136__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                VL_WRITEF("?????");
            }
        } else {
            if ((0x20U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__instr 
                                              >> 0xcU));
                                    VL_WRITEF("lui x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__137__Vfuncout,
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__138__Vfuncout);
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                    if ((0U == (0x1ffffffU 
                                                & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                   >> 7U)))) {
                                        VL_WRITEF("nop");
                                    } else {
                                        if ((0x2000000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("remu");
                                                    } else {
                                                        VL_WRITEF("rem");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("divu");
                                                    } else {
                                                        VL_WRITEF("div");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("mulhu");
                                                    } else {
                                                        VL_WRITEF("mulhsu");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("mulh");
                                                    } else {
                                                        VL_WRITEF("mul");
                                                    }
                                                }
                                            }
                                        } else {
                                            if ((0x4000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("and");
                                                    } else {
                                                        VL_WRITEF("or");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)
                                                                    ? 0x737261U
                                                                    : 0x73726cU));
                                                    } else {
                                                        VL_WRITEF("xor");
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (0x2000U 
                                                     & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("sltu");
                                                    } else {
                                                        VL_WRITEF("slt");
                                                    }
                                                } else {
                                                    if (
                                                        (0x1000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                        VL_WRITEF("sll");
                                                    } else {
                                                        VL_WRITEF("%s",
                                                                  24,
                                                                  ((0x40000000U 
                                                                    & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)
                                                                    ? 0x737562U
                                                                    : 0x616464U));
                                                    }
                                                }
                                            }
                                        }
                                        VL_WRITEF(" x%0#,x%0#,x%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0x14U)));
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    if (VL_UNLIKELY(
                                                    (0U 
                                                     == 
                                                     (7U 
                                                      & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                         >> 0xcU))))) {
                                        VL_WRITEF("sb");
                                    } else {
                                        if (VL_UNLIKELY(
                                                        (1U 
                                                         == 
                                                         (7U 
                                                          & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                             >> 0xcU))))) {
                                            VL_WRITEF("sh");
                                        } else {
                                            if ((2U 
                                                 == 
                                                 (7U 
                                                  & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                     >> 0xcU)))) {
                                                VL_WRITEF("sw");
                                            } else {
                                                VL_WRITEF("s??");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | ((0x7e0U 
                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__instr 
                                                  >> 0x14U)) 
                                              | (0x1fU 
                                                 & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__instr 
                                                    >> 7U))));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0x14U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Simm__139__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            } else {
                if ((0x10U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if ((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__Vfuncout 
                                        = (0xfffff000U 
                                           & __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__instr);
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__Vfuncout 
                                        = (0xfffffU 
                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__instr 
                                              >> 0xcU));
                                    VL_WRITEF("auipc x%0#,0x%0x <0x%0x>",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 7U)),
                                              20,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm_raw__140__Vfuncout,
                                              32,(__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__PC 
                                                  + __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Uimm__141__Vfuncout));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if (VL_LIKELY((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("andi");
                                            } else {
                                                VL_WRITEF("ori");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("%s",
                                                          32,
                                                          ((0x40000000U 
                                                            & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)
                                                            ? 0x73726169U
                                                            : 0x73726c69U));
                                            } else {
                                                VL_WRITEF("xori");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("sltiu");
                                            } else {
                                                VL_WRITEF("slti");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("slli");
                                            } else {
                                                VL_WRITEF("addi");
                                            }
                                        }
                                    }
                                    if (((1U == (7U 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0xcU))) 
                                         | (5U == (7U 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0xcU))))) {
                                        VL_WRITEF(" x%0#,x%0#,%0#",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0xfU)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0x14U)));
                                    } else {
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__instr 
                                            = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                        __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__Vfuncout 
                                            = ((0xfffff800U 
                                                & ((- (IData)(
                                                              (1U 
                                                               & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__instr 
                                                                  >> 0x1fU)))) 
                                                   << 0xbU)) 
                                               | (0x7ffU 
                                                  & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__instr 
                                                     >> 0x14U)));
                                        VL_WRITEF(" x%0#,x%0#,%0d",
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 7U)),
                                                  5,
                                                  (0x1fU 
                                                   & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                      >> 0xfU)),
                                                  32,
                                                  __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__142__Vfuncout);
                                    }
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                } else {
                    if (VL_UNLIKELY((8U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                        VL_WRITEF("?????");
                    } else {
                        if (VL_UNLIKELY((4U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                            VL_WRITEF("?????");
                        } else {
                            if (VL_LIKELY((2U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                if ((1U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                    if ((0x4000U & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                        if (VL_UNLIKELY(
                                                        (0x2000U 
                                                         & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr))) {
                                            VL_WRITEF("l??");
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("lhu");
                                            } else {
                                                VL_WRITEF("lbu");
                                            }
                                        }
                                    } else {
                                        if ((0x2000U 
                                             & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("l??");
                                            } else {
                                                VL_WRITEF("lw");
                                            }
                                        } else {
                                            if ((0x1000U 
                                                 & __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr)) {
                                                VL_WRITEF("lh");
                                            } else {
                                                VL_WRITEF("lb");
                                            }
                                        }
                                    }
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__instr 
                                        = __Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr;
                                    __Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__Vfuncout 
                                        = ((0xfffff800U 
                                            & ((- (IData)(
                                                          (1U 
                                                           & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__instr 
                                                              >> 0x1fU)))) 
                                               << 0xbU)) 
                                           | (0x7ffU 
                                              & (__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__instr 
                                                 >> 0x14U)));
                                    VL_WRITEF(" x%0#,%0d(x%0#)",
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 7U)),
                                              32,__Vfunc_SOC__DOT__CPU__DOT__riscv_disasm_Iimm__143__Vfuncout,
                                              5,(0x1fU 
                                                 & (__Vtask_SOC__DOT__CPU__DOT__riscv_disasm__133__instr 
                                                    >> 0xfU)));
                                } else {
                                    VL_WRITEF("?????");
                                }
                            } else {
                                VL_WRITEF("?????");
                            }
                        }
                    }
                }
            }
        }
        VL_WRITEF("\n");
        VL_WRITEF("[F] PC=%x ",32,vlTOPp->SOC__DOT__CPU__DOT__F_PC);
        if (VL_UNLIKELY(vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranch)) {
            VL_WRITEF(" PC <- 0x%0x",32,vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranchAddr);
        }
        VL_WRITEF("\n\n");
    }
    __Vdly__SOC__DOT__CPU__DOT__cycle = ((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt))
                                          ? (1ULL + vlTOPp->SOC__DOT__CPU__DOT__cycle)
                                          : 0ULL);
    if (vlTOPp->SOC__DOT__CPU__DOT__wbEnable) {
        __Vdlyvval__SOC__DOT__CPU__DOT__RegisterBank__v0 
            = vlTOPp->SOC__DOT__CPU__DOT__wbData;
        __Vdlyvset__SOC__DOT__CPU__DOT__RegisterBank__v0 = 1U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__RegisterBank__v0 
            = vlTOPp->SOC__DOT__CPU__DOT__wbRdId;
    }
    vlTOPp->SOC__DOT__CPU__DOT__MW_Eresult = vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult;
    if ((1U & (((IData)(vlTOPp->SOC__DOT__UART__DOT__cnt) 
                >> 7U) & (~ (IData)((0U != (IData)(vlTOPp->SOC__DOT__UART__DOT__data))))))) {
        __Vdly__SOC__DOT__uart_ready = 1U;
    } else {
        if (((IData)(vlTOPp->SOC__DOT__uart_valid) 
             & (IData)(vlTOPp->SOC__DOT__uart_ready))) {
            __Vdly__SOC__DOT__uart_ready = 0U;
        }
    }
    __Vdly__SOC__DOT__UART__DOT__cnt = ((1U & ((IData)(vlTOPp->SOC__DOT__uart_ready) 
                                               | ((IData)(vlTOPp->SOC__DOT__UART__DOT__cnt) 
                                                  >> 7U)))
                                         ? 0x56U : 
                                        (0xffU & ((IData)(vlTOPp->SOC__DOT__UART__DOT__cnt) 
                                                  - (IData)(1U))));
    if ((0x80U & (IData)(vlTOPp->SOC__DOT__UART__DOT__cnt))) {
        vlTOPp->SOC__DOT__UART__DOT__data = (0x1ffU 
                                             & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                                                >> 1U));
    } else {
        if (((IData)(vlTOPp->SOC__DOT__uart_valid) 
             & (IData)(vlTOPp->SOC__DOT__uart_ready))) {
            vlTOPp->SOC__DOT__UART__DOT__data = (0x200U 
                                                 | (0x1feU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                                       << 1U)));
        }
    }
    vlTOPp->SOC__DOT__CPU__DOT__MW_IOresult = ((0x10U 
                                                & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                ? (0x200U 
                                                   & ((~ (IData)(vlTOPp->SOC__DOT__uart_ready)) 
                                                      << 9U))
                                                : 0U);
    vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata = vlTOPp->SOC__DOT__CPU__DOT__DATARAM
        [(0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                     >> 2U))];
    vlTOPp->SOC__DOT__CPU__DOT__MW_addr = vlTOPp->SOC__DOT__CPU__DOT__EM_addr;
    __Vfunc_SOC__DOT__CPU__DOT__csrId__46__I = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    __Vfunc_SOC__DOT__CPU__DOT__csrId__46__Vfuncout 
        = ((2U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__46__I 
                  >> 0x1aU)) | (1U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__46__I 
                                      >> 0x15U)));
    if ((2U & (IData)(__Vfunc_SOC__DOT__CPU__DOT__csrId__46__Vfuncout))) {
        __Vfunc_SOC__DOT__CPU__DOT__csrId__47__I = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
        __Vfunc_SOC__DOT__CPU__DOT__csrId__47__Vfuncout 
            = ((2U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__47__I 
                      >> 0x1aU)) | (1U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__47__I 
                                          >> 0x15U)));
        vlTOPp->SOC__DOT__CPU__DOT__MW_CSRresult = 
            ((1U & (IData)(__Vfunc_SOC__DOT__CPU__DOT__csrId__47__Vfuncout))
              ? (IData)((vlTOPp->SOC__DOT__CPU__DOT__instret 
                         >> 0x20U)) : (IData)((vlTOPp->SOC__DOT__CPU__DOT__cycle 
                                               >> 0x20U)));
    } else {
        __Vfunc_SOC__DOT__CPU__DOT__csrId__48__I = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
        __Vfunc_SOC__DOT__CPU__DOT__csrId__48__Vfuncout 
            = ((2U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__48__I 
                      >> 0x1aU)) | (1U & (__Vfunc_SOC__DOT__CPU__DOT__csrId__48__I 
                                          >> 0x15U)));
        vlTOPp->SOC__DOT__CPU__DOT__MW_CSRresult = 
            ((1U & (IData)(__Vfunc_SOC__DOT__CPU__DOT__csrId__48__Vfuncout))
              ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__instret)
              : (IData)(vlTOPp->SOC__DOT__CPU__DOT__cycle));
    }
    if ((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt))) {
        if ((0x33U != vlTOPp->SOC__DOT__CPU__DOT__MW_instr)) {
            vlTOPp->SOC__DOT__CPU__DOT__instret = (1ULL 
                                                   + vlTOPp->SOC__DOT__CPU__DOT__instret);
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__instret = 0ULL;
    }
    if (((IData)(vlTOPp->SOC__DOT__IO_mem_wr) & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                                 >> 2U))) {
        vlTOPp->LEDS = (0x1fU & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2);
    }
    if ((1U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_wmask))) {
        __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v0 
            = (0xffU & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data);
        __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v0 = 1U;
        __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v0 = 0U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v0 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((2U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_wmask))) {
        __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v1 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 8U));
        __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v1 = 1U;
        __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v1 = 8U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v1 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((4U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_wmask))) {
        __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v2 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 0x10U));
        __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v2 = 1U;
        __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v2 = 0x10U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v2 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((8U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_wmask))) {
        __Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v3 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 0x18U));
        __Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v3 = 1U;
        __Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v3 = 0x18U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v3 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    vlTOPp->SOC__DOT__UART__DOT__cnt = __Vdly__SOC__DOT__UART__DOT__cnt;
    vlTOPp->SOC__DOT__uart_ready = __Vdly__SOC__DOT__uart_ready;
    vlTOPp->SOC__DOT__CPU__DOT__cycle = __Vdly__SOC__DOT__CPU__DOT__cycle;
    if (__Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v0) {
        vlTOPp->SOC__DOT__CPU__DOT__DATARAM[__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v0] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v0))) 
                & vlTOPp->SOC__DOT__CPU__DOT__DATARAM
                [__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v0]) 
               | ((IData)(__Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v0) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v0)));
    }
    if (__Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v1) {
        vlTOPp->SOC__DOT__CPU__DOT__DATARAM[__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v1] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v1))) 
                & vlTOPp->SOC__DOT__CPU__DOT__DATARAM
                [__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v1]) 
               | ((IData)(__Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v1) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v1)));
    }
    if (__Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v2) {
        vlTOPp->SOC__DOT__CPU__DOT__DATARAM[__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v2] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v2))) 
                & vlTOPp->SOC__DOT__CPU__DOT__DATARAM
                [__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v2]) 
               | ((IData)(__Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v2) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v2)));
    }
    if (__Vdlyvset__SOC__DOT__CPU__DOT__DATARAM__v3) {
        vlTOPp->SOC__DOT__CPU__DOT__DATARAM[__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v3] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v3))) 
                & vlTOPp->SOC__DOT__CPU__DOT__DATARAM
                [__Vdlyvdim0__SOC__DOT__CPU__DOT__DATARAM__v3]) 
               | ((IData)(__Vdlyvval__SOC__DOT__CPU__DOT__DATARAM__v3) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__CPU__DOT__DATARAM__v3)));
    }
    vlTOPp->SOC__DOT__CPU__DOT__MW_PC = vlTOPp->SOC__DOT__CPU__DOT__EM_PC;
    vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult = vlTOPp->SOC__DOT__CPU__DOT__E_result;
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->SOC__DOT__UART__DOT__data))))));
    vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H = (0xffffU 
                                            & ((2U 
                                                & vlTOPp->SOC__DOT__CPU__DOT__MW_addr)
                                                ? (vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata 
                                                   >> 0x10U)
                                                : vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata));
    vlTOPp->SOC__DOT__CPU__DOT__MW_instr = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 = vlTOPp->SOC__DOT__CPU__DOT__DE_rs2;
    __Vfunc_SOC__DOT__CPU__DOT__Iimm__39__I = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    __Vfunc_SOC__DOT__CPU__DOT__Iimm__39__Vfuncout 
        = ((0xfffff800U & ((- (IData)((1U & (__Vfunc_SOC__DOT__CPU__DOT__Iimm__39__I 
                                             >> 0x1fU)))) 
                           << 0xbU)) | (0x7ffU & (__Vfunc_SOC__DOT__CPU__DOT__Iimm__39__I 
                                                  >> 0x14U)));
    __Vfunc_SOC__DOT__CPU__DOT__Simm__38__I = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    __Vfunc_SOC__DOT__CPU__DOT__Simm__38__Vfuncout 
        = ((0xfffff800U & ((- (IData)((1U & (__Vfunc_SOC__DOT__CPU__DOT__Simm__38__I 
                                             >> 0x1fU)))) 
                           << 0xbU)) | ((0x7e0U & (__Vfunc_SOC__DOT__CPU__DOT__Simm__38__I 
                                                   >> 0x14U)) 
                                        | (0x1fU & 
                                           (__Vfunc_SOC__DOT__CPU__DOT__Simm__38__I 
                                            >> 7U))));
    __Vfunc_SOC__DOT__CPU__DOT__isStore__37__I = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    __Vfunc_SOC__DOT__CPU__DOT__isStore__37__Vfuncout 
        = (0x23U == (0x7fU & __Vfunc_SOC__DOT__CPU__DOT__isStore__37__I));
    vlTOPp->SOC__DOT__CPU__DOT__EM_addr = ((IData)(__Vfunc_SOC__DOT__CPU__DOT__isStore__37__Vfuncout)
                                            ? (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                               + __Vfunc_SOC__DOT__CPU__DOT__Simm__38__Vfuncout)
                                            : (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                               + __Vfunc_SOC__DOT__CPU__DOT__Iimm__39__Vfuncout));
    vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_B = (0xffU & 
                                            ((1U & vlTOPp->SOC__DOT__CPU__DOT__MW_addr)
                                              ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H) 
                                                 >> 8U)
                                              : (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H)));
    vlTOPp->SOC__DOT__CPU__DOT__EM_PC = vlTOPp->SOC__DOT__CPU__DOT__DE_PC;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__I 
                    >> 7U));
    vlTOPp->SOC__DOT__CPU__DOT__wbRdId = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__55__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__55__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__55__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__54__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__57__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__54__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__57__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__57__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__56__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__54__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__56__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__56__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__54__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__56__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__57__Vfuncout))));
    vlTOPp->SOC__DOT__CPU__DOT__wbEnable = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__54__Vfuncout) 
                                            & (0U != (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__55__Vfuncout)));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__49__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__49__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__49__I 
                 >> 0xcU));
    vlTOPp->SOC__DOT__CPU__DOT__W_funct3 = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__49__Vfuncout;
    vlTOPp->SOC__DOT__CPU__DOT__EM_instr = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffff00ffU 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff00U 
                                                   & (((1U 
                                                        & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->SOC__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                                        >> 8U)) 
                                                      << 8U)));
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xff00ffffU 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff0000U 
                                                   & (((2U 
                                                        & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->SOC__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                                        >> 0x10U)) 
                                                      << 0x10U)));
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffffffU 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff000000U 
                                                   & (((1U 
                                                        & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->SOC__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       ((2U 
                                                         & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                         ? 
                                                        (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                                         >> 8U)
                                                         : 
                                                        (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                                         >> 0x18U))) 
                                                      << 0x18U)));
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__I = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__I = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__Vfuncout 
        = (0x1fU & (__Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__I 
                    >> 0xfU));
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__Vfuncout 
        = (0x1fU & (__Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__I 
                    >> 0x14U));
    vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 = vlTOPp->SOC__DOT__CPU__DOT__RegisterBank
        [__Vfunc_SOC__DOT__CPU__DOT__rs1Id__0__Vfuncout];
    vlTOPp->SOC__DOT__CPU__DOT__DE_rs2 = vlTOPp->SOC__DOT__CPU__DOT__RegisterBank
        [__Vfunc_SOC__DOT__CPU__DOT__rs2Id__1__Vfuncout];
    vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_sign = (1U & 
                                               ((~ 
                                                 ((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_funct3) 
                                                  >> 2U)) 
                                                & ((0U 
                                                    == 
                                                    (3U 
                                                     & (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_funct3)))
                                                    ? 
                                                   ((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_B) 
                                                    >> 7U)
                                                    : 
                                                   ((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H) 
                                                    >> 0xfU))));
    if (__Vdlyvset__SOC__DOT__CPU__DOT__RegisterBank__v0) {
        vlTOPp->SOC__DOT__CPU__DOT__RegisterBank[__Vdlyvdim0__SOC__DOT__CPU__DOT__RegisterBank__v0] 
            = __Vdlyvval__SOC__DOT__CPU__DOT__RegisterBank__v0;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_PC = vlTOPp->SOC__DOT__CPU__DOT__FD_PC;
    }
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__53__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__53__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__53__I 
                 >> 0xcU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__Vfuncout 
        = (0x73U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__Vfuncout 
        = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__Vfuncout) 
           & (2U == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__53__Vfuncout)));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLoad__50__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLoad__50__Vfuncout 
        = (3U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLoad__50__I));
    vlTOPp->SOC__DOT__CPU__DOT__wbData = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLoad__50__Vfuncout)
                                           ? ((0x400000U 
                                               & vlTOPp->SOC__DOT__CPU__DOT__MW_addr)
                                               ? vlTOPp->SOC__DOT__CPU__DOT__MW_IOresult
                                               : ((0U 
                                                   == 
                                                   (3U 
                                                    & (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_funct3)))
                                                   ? 
                                                  ((0xffffff00U 
                                                    & ((- (IData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_sign))) 
                                                       << 8U)) 
                                                   | (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_B))
                                                   : 
                                                  ((1U 
                                                    == 
                                                    (3U 
                                                     & (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_funct3)))
                                                    ? 
                                                   ((0xffff0000U 
                                                     & ((- (IData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_sign))) 
                                                        << 0x10U)) 
                                                    | (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H))
                                                    : vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata)))
                                           : ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__Vfuncout)
                                               ? vlTOPp->SOC__DOT__CPU__DOT__MW_CSRresult
                                               : vlTOPp->SOC__DOT__CPU__DOT__MW_Eresult));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__44__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__44__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__44__I));
    vlTOPp->SOC__DOT__IO_mem_wr = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__44__Vfuncout) 
                                   & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                      >> 0x16U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__43__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__43__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__43__I 
                 >> 0xcU));
    vlTOPp->SOC__DOT__CPU__DOT__M_funct3 = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__43__Vfuncout;
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_instr = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
                                                 | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))
                                                 ? 0x33U
                                                 : vlTOPp->SOC__DOT__CPU__DOT__FD_instr);
    }
    if (vlTOPp->SOC__DOT__CPU__DOT__E_flush) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_instr = 0x33U;
    }
    vlTOPp->SOC__DOT__uart_valid = ((IData)(vlTOPp->SOC__DOT__IO_mem_wr) 
                                    & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                       >> 3U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__45__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__45__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__45__I));
    vlTOPp->SOC__DOT__CPU__DOT__M_wmask = ((- (IData)(
                                                      ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__45__Vfuncout) 
                                                       & (~ 
                                                          (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                                           >> 0x16U))))) 
                                           & ((0U == 
                                               (3U 
                                                & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_funct3)))
                                               ? ((2U 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                   ? 
                                                  ((1U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 8U
                                                    : 4U)
                                                   : 
                                                  ((1U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 2U
                                                    : 1U))
                                               : ((1U 
                                                   == 
                                                   (3U 
                                                    & (IData)(vlTOPp->SOC__DOT__CPU__DOT__M_funct3)))
                                                   ? 
                                                  ((2U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 0xcU
                                                    : 3U)
                                                   : 0xfU)));
    vlTOPp->SOC__DOT__CPU__DOT__FD_nop = (1U & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranch) 
                                                | (~ (IData)(
                                                             (0xffffU 
                                                              == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt))))));
    if ((1U & (~ ((IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard) 
                  | (IData)(vlTOPp->SOC__DOT__CPU__DOT__halt))))) {
        vlTOPp->SOC__DOT__CPU__DOT__FD_instr = vlTOPp->SOC__DOT__CPU__DOT__PROGROM
            [(0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__F_PC 
                         >> 2U))];
        vlTOPp->SOC__DOT__CPU__DOT__FD_PC = vlTOPp->SOC__DOT__CPU__DOT__F_PC;
        vlTOPp->SOC__DOT__CPU__DOT__F_PC = ((IData)(4U) 
                                            + vlTOPp->SOC__DOT__CPU__DOT__F_PC);
    }
    if (vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranch) {
        vlTOPp->SOC__DOT__CPU__DOT__F_PC = vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranchAddr;
    }
    if ((1U & (~ (IData)((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt)))))) {
        vlTOPp->SOC__DOT__CPU__DOT__F_PC = 0U;
    }
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__Vfuncout 
        = (0x33U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__I));
    vlTOPp->SOC__DOT__CPU__DOT__E_minus = ((vlTOPp->SOC__DOT__CPU__DOT__DE_instr 
                                            >> 0x1eU) 
                                           & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__Vfuncout));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_rs1;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__8__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__8__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__8__I 
                 >> 0xcU));
    vlTOPp->SOC__DOT__CPU__DOT__E_shifter_in = ((1U 
                                                 == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__8__Vfuncout))
                                                 ? vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__9__Vfuncout
                                                 : vlTOPp->SOC__DOT__CPU__DOT__DE_rs1);
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Iimm__4__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Iimm__4__Vfuncout 
        = ((0xfffff800U & ((- (IData)((1U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Iimm__4__I 
                                             >> 0x1fU)))) 
                           << 0xbU)) | (0x7ffU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Iimm__4__I 
                                                  >> 0x14U)));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__3__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__3__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__3__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__Vfuncout 
        = (0x33U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__I));
    vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2 = (((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__Vfuncout) 
                                             | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__3__Vfuncout))
                                             ? vlTOPp->SOC__DOT__CPU__DOT__DE_rs2
                                             : vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Iimm__4__Vfuncout);
    vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus = (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                             + vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2);
    vlTOPp->SOC__DOT__CPU__DOT__E_shifter = (IData)(
                                                    (0x1ffffffffULL 
                                                     & VL_SHIFTRS_QQI(33,33,5, 
                                                                      (((QData)((IData)(
                                                                                (1U 
                                                                                & ((vlTOPp->SOC__DOT__CPU__DOT__DE_instr 
                                                                                >> 0x1eU) 
                                                                                & (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                                                                >> 0x1fU))))) 
                                                                        << 0x20U) 
                                                                       | (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_shifter_in))), 
                                                                      (0x1fU 
                                                                       & vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2))));
    vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus = (0x1ffffffffULL 
                                              & (1ULL 
                                                 + 
                                                 ((0x100000000ULL 
                                                   | (QData)((IData)(
                                                                     (~ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)))) 
                                                  + (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__69__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__69__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__69__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__I 
                    >> 0xfU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__67__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__71__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__67__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__71__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__71__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__70__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__67__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__70__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__70__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__67__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__70__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__71__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__66__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__66__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__66__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__I 
                    >> 0xfU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__64__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__73__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__64__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__73__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__73__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__72__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__64__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__72__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__72__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__64__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__72__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__73__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__63__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__63__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__63__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__I 
                    >> 0xfU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__61__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__75__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__61__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__75__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__75__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__74__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__61__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__74__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__74__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__61__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__74__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__75__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__I 
                    >> 0xfU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__78__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__78__Vfuncout 
        = (0x37U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__78__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__Vfuncout 
        = (0x17U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__76__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__76__Vfuncout 
        = (0x6fU == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__76__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__Vfuncout 
        = (1U & (~ (((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__76__Vfuncout) 
                     | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__Vfuncout)) 
                    | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__78__Vfuncout))));
    vlTOPp->SOC__DOT__CPU__DOT__rs1Hazard = ((((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                               & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__Vfuncout)) 
                                              & (0U 
                                                 != (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__Vfuncout))) 
                                             & ((((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__61__Vfuncout) 
                                                  & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__Vfuncout) 
                                                     == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__63__Vfuncout))) 
                                                 | ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__64__Vfuncout) 
                                                    & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__Vfuncout) 
                                                       == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__66__Vfuncout)))) 
                                                | ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__67__Vfuncout) 
                                                   & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__Vfuncout) 
                                                      == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__69__Vfuncout)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__89__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__89__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__89__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__I 
                    >> 0x14U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__87__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__91__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__87__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__91__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__91__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__90__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__87__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__90__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__90__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__87__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__90__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__91__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__86__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__86__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__86__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__I 
                    >> 0x14U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__84__I 
        = vlTOPp->SOC__DOT__CPU__DOT__EM_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__93__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__84__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__93__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__93__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__92__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__84__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__92__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__92__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__84__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__92__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__93__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__83__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__83__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__83__I 
                    >> 7U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__I 
                    >> 0x14U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__81__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__95__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__81__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__95__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__95__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__94__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__81__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__94__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__94__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__81__Vfuncout 
        = (1U & ((~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__94__Vfuncout)) 
                 & (~ (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__95__Vfuncout))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__I 
                    >> 0x14U));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I 
        = vlTOPp->SOC__DOT__CPU__DOT__FD_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__98__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__98__Vfuncout 
        = (0x23U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__98__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__97__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__97__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__97__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__Vfuncout 
        = (0x33U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__Vfuncout 
        = (((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__Vfuncout) 
            | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__97__Vfuncout)) 
           | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isStore__98__Vfuncout));
    vlTOPp->SOC__DOT__CPU__DOT__rs2Hazard = ((((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                               & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__Vfuncout)) 
                                              & (0U 
                                                 != (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__Vfuncout))) 
                                             & ((((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__81__Vfuncout) 
                                                  & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__Vfuncout) 
                                                     == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__83__Vfuncout))) 
                                                 | ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__84__Vfuncout) 
                                                    & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__Vfuncout) 
                                                       == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__86__Vfuncout)))) 
                                                | ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__writesRd__87__Vfuncout) 
                                                   & ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__Vfuncout) 
                                                      == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__89__Vfuncout)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__Vfuncout 
        = ((0xfff00000U & ((- (IData)((1U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I 
                                             >> 0x1fU)))) 
                           << 0x14U)) | ((0xff000U 
                                          & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I) 
                                         | ((0x800U 
                                             & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I 
                                                >> 9U)) 
                                            | (0x7feU 
                                               & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I 
                                                  >> 0x14U)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__29__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__29__Vfuncout 
        = (0x6fU == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__29__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__Vfuncout 
        = ((0xfffff000U & ((- (IData)((1U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I 
                                             >> 0x1fU)))) 
                           << 0xcU)) | ((0x800U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I 
                                                   << 4U)) 
                                        | ((0x7e0U 
                                            & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I 
                                               >> 0x14U)) 
                                           | (0x1eU 
                                              & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I 
                                                 >> 7U)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__27__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__27__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__27__I));
    vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranchAddr 
        = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__27__Vfuncout)
            ? (vlTOPp->SOC__DOT__CPU__DOT__DE_PC + vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Bimm__28__Vfuncout)
            : ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__29__Vfuncout)
                ? (vlTOPp->SOC__DOT__CPU__DOT__DE_PC 
                   + vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Jimm__30__Vfuncout)
                : (0xfffffffeU & vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus)));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
        = vlTOPp->SOC__DOT__CPU__DOT__E_shifter;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->SOC__DOT__CPU__DOT__E_leftshift = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__flip32__10__Vfuncout;
    vlTOPp->SOC__DOT__CPU__DOT__E_LT = (1U & ((0x80000000U 
                                               & (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                                  ^ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2))
                                               ? (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                                                  >> 0x1fU)
                                               : (IData)(
                                                         (vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus 
                                                          >> 0x20U))));
    vlTOPp->SOC__DOT__CPU__DOT__dataHazard = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__rs1Hazard) 
                                              | (IData)(vlTOPp->SOC__DOT__CPU__DOT__rs2Hazard));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__18__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__18__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__18__I 
                 >> 0xcU));
    if ((4U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__18__Vfuncout))) {
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__19__I 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__19__Vfuncout 
            = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__19__I 
                     >> 0xcU));
        if ((2U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__19__Vfuncout))) {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__20__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__20__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__20__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch 
                = (1U & ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__20__Vfuncout))
                          ? (~ (IData)((vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus 
                                        >> 0x20U)))
                          : (IData)((vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus 
                                     >> 0x20U))));
        } else {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__21__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__21__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__21__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch 
                = (1U & ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__21__Vfuncout))
                          ? (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT))
                          : (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT)));
        }
    } else {
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__22__I 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__22__Vfuncout 
            = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__22__I 
                     >> 0xcU));
        if ((2U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__22__Vfuncout))) {
            vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch = 0U;
        } else {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__23__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__23__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__23__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch 
                = ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__23__Vfuncout))
                    ? (0U != (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus))
                    : (0U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus)));
        }
    }
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__11__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__11__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__11__I 
                 >> 0xcU));
    if ((4U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__11__Vfuncout))) {
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__12__I 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__12__Vfuncout 
            = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__12__I 
                     >> 0xcU));
        if ((2U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__12__Vfuncout))) {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__13__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__13__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__13__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_aluOut = 
                ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__13__Vfuncout))
                  ? (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                     & vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)
                  : (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                     | vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2));
        } else {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__14__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__14__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__14__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_aluOut = 
                ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__14__Vfuncout))
                  ? vlTOPp->SOC__DOT__CPU__DOT__E_shifter
                  : (vlTOPp->SOC__DOT__CPU__DOT__DE_rs1 
                     ^ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2));
        }
    } else {
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__15__I 
            = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
        vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__15__Vfuncout 
            = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__15__I 
                     >> 0xcU));
        if ((2U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__15__Vfuncout))) {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__16__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__16__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__16__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_aluOut = 
                ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__16__Vfuncout))
                  ? (1U & (IData)((vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus 
                                   >> 0x20U))) : (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT));
        } else {
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__17__I 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
            vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__17__Vfuncout 
                = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__17__I 
                         >> 0xcU));
            vlTOPp->SOC__DOT__CPU__DOT__E_aluOut = 
                ((1U & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__17__Vfuncout))
                  ? vlTOPp->SOC__DOT__CPU__DOT__E_leftshift
                  : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_minus)
                      ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_aluMinus)
                      : vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus));
        }
    }
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__26__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__26__Vfuncout 
        = (0x63U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__26__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__25__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__25__Vfuncout 
        = (0x67U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__25__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__24__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__24__Vfuncout 
        = (0x6fU == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__24__I));
    vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranch = (((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__24__Vfuncout) 
                                                   | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__25__Vfuncout)) 
                                                  | ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isBranch__26__Vfuncout) 
                                                     & (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch)));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__36__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__36__Vfuncout 
        = (0xfffff000U & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__36__I);
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__Vfuncout 
        = (0x17U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__34__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__34__Vfuncout 
        = (0xfffff000U & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__34__I);
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__33__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__33__Vfuncout 
        = (0x37U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__33__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__32__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__32__Vfuncout 
        = (0x67U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__32__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__31__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__31__Vfuncout 
        = (0x6fU == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__31__I));
    vlTOPp->SOC__DOT__CPU__DOT__E_result = (((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJAL__31__Vfuncout) 
                                             | (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isJALR__32__Vfuncout))
                                             ? ((IData)(4U) 
                                                + vlTOPp->SOC__DOT__CPU__DOT__DE_PC)
                                             : ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isLUI__33__Vfuncout)
                                                 ? vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__34__Vfuncout
                                                 : 
                                                ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__Vfuncout)
                                                  ? 
                                                 (vlTOPp->SOC__DOT__CPU__DOT__DE_PC 
                                                  + vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__Uimm__36__Vfuncout)
                                                  : vlTOPp->SOC__DOT__CPU__DOT__E_aluOut)));
    vlTOPp->SOC__DOT__CPU__DOT__E_flush = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_JumpOrBranch) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard));
}

VL_INLINE_OPT void VSOC::_sequent__TOP__2(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_sequent__TOP__2\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt 
        = ((IData)(vlTOPp->RESET) ? 0U : (0xffffU & 
                                          ((IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt) 
                                           + (1U & 
                                              (~ (IData)(vlTOPp->SOC__DOT__resetn))))));
    vlTOPp->SOC__DOT__resetn = (0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__I 
        = vlTOPp->SOC__DOT__CPU__DOT__DE_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__42__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__42__Vfuncout 
        = (7U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__42__I 
                 >> 0xcU));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__I 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__I;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__Vfuncout 
        = (0x73U == (0x7fU & vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__I));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__Vfuncout 
        = ((IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__Vfuncout) 
           & (0U == (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__funct3__42__Vfuncout)));
    vlTOPp->SOC__DOT__CPU__DOT__halt = ((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt)) 
                                        & (IData)(vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__Vfuncout));
    vlTOPp->SOC__DOT__CPU__DOT__D_stall = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__halt));
}

void VSOC::_eval(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_eval\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->CLK) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLK)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    if ((((IData)(vlTOPp->CLK) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLK))) 
         | ((IData)(vlTOPp->RESET) & (~ (IData)(vlTOPp->__Vclklast__TOP__RESET))))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__CLK = vlTOPp->CLK;
    vlTOPp->__Vclklast__TOP__RESET = vlTOPp->RESET;
}

VL_INLINE_OPT QData VSOC::_change_request(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_change_request\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData VSOC::_change_request_1(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_change_request_1\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VSOC::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((CLK & 0xfeU))) {
        Verilated::overWidthError("CLK");}
    if (VL_UNLIKELY((RESET & 0xfeU))) {
        Verilated::overWidthError("RESET");}
    if (VL_UNLIKELY((RXD & 0xfeU))) {
        Verilated::overWidthError("RXD");}
}
#endif  // VL_DEBUG
