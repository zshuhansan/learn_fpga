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
            VL_FATAL_MT("src/Soc.v", 18, "",
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
            VL_FATAL_MT("src/Soc.v", 18, "",
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
    CData/*1:0*/ __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__Vfuncout;
    CData/*1:0*/ __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev;
    CData/*0:0*/ __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir;
    CData/*0:0*/ __Vdly__SOC__DOT__CPU__DOT__FD_nop;
    CData/*0:0*/ __Vdly__SOC__DOT__CPU__DOT__FB_valid;
    CData/*1:0*/ __Vdly__SOC__DOT__CPU__DOT__FB_epoch;
    CData/*1:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0;
    CData/*4:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__RF__DOT__regs__v0;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__CPU__DOT__RF__DOT__regs__v0;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v0;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v0;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v0;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v1;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v1;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v1;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v2;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v2;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v2;
    CData/*4:0*/ __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v3;
    CData/*7:0*/ __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v3;
    CData/*0:0*/ __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v3;
    CData/*0:0*/ __Vdly__SOC__DOT__uart_ready;
    CData/*7:0*/ __Vdly__SOC__DOT__UART__DOT__cnt;
    SData/*11:0*/ __Vdlyvdim0__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v0;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v1;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v2;
    SData/*13:0*/ __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v3;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__FB_PC;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__FB_instr;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    IData/*31:0*/ __Vdlyvval__SOC__DOT__CPU__DOT__RF__DOT__regs__v0;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    IData/*31:0*/ __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    QData/*63:0*/ __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__cycle;
    QData/*63:0*/ __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__instret;
    // Body
    __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__cycle = vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__cycle;
    __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__instret = vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__instret;
    __Vdly__SOC__DOT__CPU__DOT__FB_epoch = vlTOPp->SOC__DOT__CPU__DOT__FB_epoch;
    __Vdly__SOC__DOT__CPU__DOT__FB_instr = vlTOPp->SOC__DOT__CPU__DOT__FB_instr;
    __Vdly__SOC__DOT__CPU__DOT__FB_PC = vlTOPp->SOC__DOT__CPU__DOT__FB_PC;
    __Vdly__SOC__DOT__CPU__DOT__FB_valid = vlTOPp->SOC__DOT__CPU__DOT__FB_valid;
    __Vdly__SOC__DOT__CPU__DOT__FD_nop = vlTOPp->SOC__DOT__CPU__DOT__FD_nop;
    __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v0 = 0U;
    __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v1 = 0U;
    __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v2 = 0U;
    __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v3 = 0U;
    __Vdly__SOC__DOT__uart_ready = vlTOPp->SOC__DOT__uart_ready;
    __Vdly__SOC__DOT__UART__DOT__cnt = vlTOPp->SOC__DOT__UART__DOT__cnt;
    __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
        = vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r 
        = vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r 
        = vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    __Vdlyvset__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0 = 0U;
    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1 
        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3 
        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 
        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 
        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    __Vdlyvset__SOC__DOT__CPU__DOT__RF__DOT__regs__v0 = 0U;
    if (VL_UNLIKELY(vlTOPp->SOC__DOT__uart_valid)) {
        VL_WRITEF("%c",8,(0xffU & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
        if (0x80000001U) { VL_FFLUSH_I(0x80000001U); }
    }
    __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__cycle = ((IData)(vlTOPp->RESET)
                                                    ? 
                                                   (1ULL 
                                                    + vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__cycle)
                                                    : 0ULL);
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__F_redirect) {
            vlTOPp->SOC__DOT__CPU__DOT__F_epoch = vlTOPp->SOC__DOT__CPU__DOT__F_epochNext;
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__F_epoch = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_predictRA = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictPC) 
             & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
            vlTOPp->SOC__DOT__CPU__DOT__PC = vlTOPp->SOC__DOT__CPU__DOT__D_PCprediction;
        }
        if (vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) {
            vlTOPp->SOC__DOT__CPU__DOT__PC = vlTOPp->SOC__DOT__CPU__DOT__E_PCcorrection;
        }
        if (vlTOPp->SOC__DOT__inst_en) {
            vlTOPp->SOC__DOT__CPU__DOT__PC = ((IData)(4U) 
                                              + vlTOPp->SOC__DOT__CPU__DOT__F_PC);
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__PC = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_predictBranch 
            = vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_nop)))) {
            __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__instret 
                = (1ULL + vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__instret);
        }
    } else {
        __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__instret = 0ULL;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_PCplus4orBimm 
            = (vlTOPp->SOC__DOT__CPU__DOT__FD_PC + 
               ((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch)
                 ? 4U : vlTOPp->SOC__DOT__CPU__DOT__D_Bimm));
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__aluBusy) {
            vlTOPp->SOC__DOT__CPU__DOT__EM_isStore = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_isStore 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_isStore;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_isStore = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isEBREAK = (
                                                   (0x1cU 
                                                    == 
                                                    (0x1fU 
                                                     & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                        >> 2U))) 
                                                   & (~ 
                                                      (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 0xdU)));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isEBREAK = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is = 
            (0xffU & ((IData)(1U) << (7U & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                            >> 0xcU))));
    }
    if ((1U & (IData)(vlTOPp->SOC__DOT__data_wmask))) {
        __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v0 
            = (0xffU & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data);
        __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v0 = 1U;
        __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v0 = 0U;
        __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v0 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((2U & (IData)(vlTOPp->SOC__DOT__data_wmask))) {
        __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v1 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 8U));
        __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v1 = 1U;
        __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v1 = 8U;
        __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v1 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((4U & (IData)(vlTOPp->SOC__DOT__data_wmask))) {
        __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v2 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 0x10U));
        __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v2 = 1U;
        __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v2 = 0x10U;
        __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v2 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((8U & (IData)(vlTOPp->SOC__DOT__data_wmask))) {
        __Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v3 
            = (0xffU & (vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data 
                        >> 0x18U));
        __Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v3 = 1U;
        __Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v3 = 0x18U;
        __Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v3 
            = (0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if (vlTOPp->RESET) {
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
                                             ? 0x56U
                                             : (0xffU 
                                                & ((IData)(vlTOPp->SOC__DOT__UART__DOT__cnt) 
                                                   - (IData)(1U))));
        if ((0x80U & (IData)(vlTOPp->SOC__DOT__UART__DOT__cnt))) {
            vlTOPp->SOC__DOT__UART__DOT__data = (0x1ffU 
                                                 & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                                                    >> 1U));
        } else {
            if (((IData)(vlTOPp->SOC__DOT__uart_valid) 
                 & (IData)(vlTOPp->SOC__DOT__uart_ready))) {
                vlTOPp->SOC__DOT__UART__DOT__data = 
                    (0x200U | (0x1feU & (vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                                         << 1U)));
            }
        }
    } else {
        __Vdly__SOC__DOT__uart_ready = 1U;
        __Vdly__SOC__DOT__UART__DOT__cnt = 0x56U;
        vlTOPp->SOC__DOT__UART__DOT__data = 0U;
    }
    if (((IData)(vlTOPp->SOC__DOT__IO_mem_wr) & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                                 >> 2U))) {
        vlTOPp->LEDS = (0x1fU & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2);
    }
    if (vlTOPp->RESET) {
        if (VL_UNLIKELY(((IData)(vlTOPp->SOC__DOT__IO_mem_wr) 
                         & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                            >> 8U)))) {
            VL_WRITEF("COREMARK_TICKS=%0#\n",32,vlTOPp->SOC__DOT__MBOX0);
            VL_WRITEF("COREMARK_MHZ_X1000=%0#\n",32,
                      vlTOPp->SOC__DOT__MBOX1);
            VL_WRITEF("COREMARK_CPI_X1000=%0#\n",32,
                      vlTOPp->SOC__DOT__MBOX2);
            VL_FINISH_MT("src/Soc.v", 265, "");
        }
        if (((IData)(vlTOPp->SOC__DOT__IO_mem_wr) & 
             (vlTOPp->SOC__DOT__CPU__DOT__EM_addr >> 5U))) {
            vlTOPp->SOC__DOT__MBOX0 = vlTOPp->SOC__DOT__CPU__DOT__EM_rs2;
        }
        if (((IData)(vlTOPp->SOC__DOT__IO_mem_wr) & 
             (vlTOPp->SOC__DOT__CPU__DOT__EM_addr >> 6U))) {
            vlTOPp->SOC__DOT__MBOX1 = vlTOPp->SOC__DOT__CPU__DOT__EM_rs2;
        }
        if (((IData)(vlTOPp->SOC__DOT__IO_mem_wr) & 
             (vlTOPp->SOC__DOT__CPU__DOT__EM_addr >> 7U))) {
            vlTOPp->SOC__DOT__MBOX2 = vlTOPp->SOC__DOT__CPU__DOT__EM_rs2;
        }
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_IorSimm = ((8U 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)))
                                                   ? 
                                                  ((0xfffff800U 
                                                    & ((- (IData)(
                                                                  (1U 
                                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                                      >> 0x1fU)))) 
                                                       << 0xbU)) 
                                                   | ((0x7e0U 
                                                       & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 0x14U)) 
                                                      | (0x1fU 
                                                         & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 7U))))
                                                   : 
                                                  ((0xfffff800U 
                                                    & ((- (IData)(
                                                                  (1U 
                                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                                      >> 0x1fU)))) 
                                                       << 0xbU)) 
                                                   | (0x7ffU 
                                                      & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 0x14U))));
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r) {
            if (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divstep_do) {
                __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                    = (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                       - (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r));
                __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r 
                    = (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r 
                       | vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r);
            }
            vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                = (0x7fffffffffffffffULL & (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                                            >> 1U));
            __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
                = (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
                   >> 1U);
            vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r 
                = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                   & (~ vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r));
        } else {
            if (((IData)(vlTOPp->SOC__DOT__CPU__DOT____Vcellinp__DIVU__start) 
                 & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r)))) {
                __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                    = ((1U & ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                              & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                 >> 0x1fU))) ? (- vlTOPp->SOC__DOT__CPU__DOT__E_rs1)
                        : vlTOPp->SOC__DOT__CPU__DOT__E_rs1);
                __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r = 0U;
                __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = 0x80000000U;
                vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r = 1U;
                vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                    = (0x7fffffffffffffffULL & ((QData)((IData)(
                                                                ((1U 
                                                                  & ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                                     & (vlTOPp->SOC__DOT__CPU__DOT__E_rs2 
                                                                        >> 0x1fU)))
                                                                  ? 
                                                                 (- vlTOPp->SOC__DOT__CPU__DOT__E_rs2)
                                                                  : vlTOPp->SOC__DOT__CPU__DOT__E_rs2))) 
                                                << 0x1fU));
                vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__sign_r 
                    = (1U & ((2U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3))
                              ? ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                 & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                    >> 0x1fU)) : ((
                                                   (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                   & ((1U 
                                                       & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                          >> 0x1fU)) 
                                                      != 
                                                      (1U 
                                                       & (vlTOPp->SOC__DOT__CPU__DOT__E_rs2 
                                                          >> 0x1fU)))) 
                                                  & (0U 
                                                     != vlTOPp->SOC__DOT__CPU__DOT__E_rs2))));
            } else {
                vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r = 0U;
            }
        }
        vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r 
            = (1U & vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r);
    } else {
        __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r = 0U;
        __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r = 0ULL;
        __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__sign_r = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r = 0U;
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch) 
             & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__branch_history 
                = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch) 
                    << 8U) | (0xffU & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__branch_history) 
                                       >> 1U)));
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__branch_history = 0U;
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch) 
             & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir 
                = vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch;
            __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev 
                = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__BHT
                [vlTOPp->SOC__DOT__CPU__DOT__DE_BHTindex];
            __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__Vfuncout 
                = ((0U == (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                            << 2U) | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                    ? 0U : ((1U == (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                                     << 2U) | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                             ? 0U : ((2U == (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                                              << 2U) 
                                             | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                                      ? 1U : ((3U == 
                                               (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                                                 << 2U) 
                                                | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                                               ? 2U
                                               : ((4U 
                                                   == 
                                                   (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                                                     << 2U) 
                                                    | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                                                   ? 1U
                                                   : 
                                                  ((5U 
                                                    == 
                                                    (((IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__dir) 
                                                      << 2U) 
                                                     | (IData)(__Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__prev)))
                                                    ? 2U
                                                    : 3U))))));
            __Vdlyvval__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0 
                = __Vfunc_SOC__DOT__CPU__DOT__BP__DOT__incdec_sat__7__Vfuncout;
            __Vdlyvset__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0 = 1U;
            __Vdlyvdim0__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0 
                = vlTOPp->SOC__DOT__CPU__DOT__DE_BHTindex;
        }
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_rs2Id = (0x1fU 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0x14U));
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) {
            __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_0;
            __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2;
        } else {
            if ((1U & (((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)) 
                        & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC))) 
                       & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))))) {
                if (((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                      >> 3U) & (1U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                >> 7U))))) {
                    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1;
                    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                        = ((IData)(4U) + vlTOPp->SOC__DOT__CPU__DOT__FD_PC);
                }
                if ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_isJALR) 
                      & (0U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                         >> 7U)))) 
                     & ((1U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                         >> 0xfU))) 
                        | (5U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                           >> 0xfU)))))) {
                    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1;
                    __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_3;
                }
            }
        }
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJAL) 
                 & (1U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId)))) {
                __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2;
                __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_0;
            }
            if ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR) 
                  & (0U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))) 
                 & ((1U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id)) 
                    | (5U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id))))) {
                __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2;
            }
        }
    } else {
        __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 = 0U;
        __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 = 0U;
        __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1 = 0U;
        __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3 = 0U;
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_wbEnable) 
         & (0U != (IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_rdId)))) {
        __Vdlyvval__SOC__DOT__CPU__DOT__RF__DOT__regs__v0 
            = vlTOPp->SOC__DOT__CPU__DOT__MW_wbData;
        __Vdlyvset__SOC__DOT__CPU__DOT__RF__DOT__regs__v0 = 1U;
        __Vdlyvdim0__SOC__DOT__CPU__DOT__RF__DOT__regs__v0 
            = vlTOPp->SOC__DOT__CPU__DOT__MW_rdId;
    }
    vlTOPp->SOC__DOT__uart_ready = __Vdly__SOC__DOT__uart_ready;
    vlTOPp->SOC__DOT__UART__DOT__cnt = __Vdly__SOC__DOT__UART__DOT__cnt;
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
        = __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    if (__Vdlyvset__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0) {
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__BHT[__Vdlyvdim0__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0] 
            = __Vdlyvval__SOC__DOT__CPU__DOT__BP__DOT__BHT__v0;
    }
    if (__Vdlyvset__SOC__DOT__CPU__DOT__RF__DOT__regs__v0) {
        vlTOPp->SOC__DOT__CPU__DOT__RF__DOT__regs[__Vdlyvdim0__SOC__DOT__CPU__DOT__RF__DOT__regs__v0] 
            = __Vdlyvval__SOC__DOT__CPU__DOT__RF__DOT__regs__v0;
    }
    vlTOPp->SOC__DOT__CPU__DOT__RF__DOT__regs[0U] = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__MW_nop = (1U & ((~ (IData)(vlTOPp->RESET)) 
                                                | (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_nop)));
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isStore = (8U 
                                                  == 
                                                  (0x1fU 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 2U)));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isStore = 0U;
    }
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->SOC__DOT__UART__DOT__data))))));
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 
                    = vlTOPp->SOC__DOT__CPU__DOT__E_rs2;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_rs2 = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_BHTindex = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__bht_index;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch = (4U 
                                                   == 
                                                   ((4U 
                                                     & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                        >> 4U)) 
                                                    | ((2U 
                                                        & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                           >> 3U)) 
                                                       | (1U 
                                                          & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                             >> 2U)))));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) {
            vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
            vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_3 
                = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3;
        } else {
            if ((1U & (((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)) 
                        & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC))) 
                       & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))))) {
                if (((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                      >> 3U) & (1U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                >> 7U))))) {
                    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_3 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2;
                    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0;
                }
                if ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_isJALR) 
                      & (0U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                         >> 7U)))) 
                     & ((1U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                         >> 0xfU))) 
                        | (5U == (0x1fU & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                           >> 0xfU)))))) {
                    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                        = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2;
                }
            }
        }
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJAL) 
                 & (1U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId)))) {
                vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
                vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_0 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_PCplus4orUimm;
            }
            if ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR) 
                  & (0U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))) 
                 & ((1U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id)) 
                    | (5U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id))))) {
                vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_0 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
                vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2 
                    = vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_1 = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_3 = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_0 = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_2 = 0U;
    }
    if (vlTOPp->RESET) {
        vlTOPp->SOC__DOT__CPU__DOT__MW_rdId = vlTOPp->SOC__DOT__CPU__DOT__EM_rdId;
        vlTOPp->SOC__DOT__CPU__DOT__MW_wbEnable = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable) 
                                                   & 1U);
        vlTOPp->SOC__DOT__CPU__DOT__MW_wbData = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_isLoad)
                                                  ? 
                                                 ((0x400000U 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                   ? 0U
                                                   : 
                                                  ((0U 
                                                    == 
                                                    (3U 
                                                     & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                                    ? 
                                                   ((0xffffff00U 
                                                     & ((- (IData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign))) 
                                                        << 8U)) 
                                                    | (IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b))
                                                    : 
                                                   ((1U 
                                                     == 
                                                     (3U 
                                                      & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                                     ? 
                                                    ((0xffff0000U 
                                                      & ((- (IData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign))) 
                                                         << 0x10U)) 
                                                     | (IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h))
                                                     : vlTOPp->SOC__DOT__data_rdata)))
                                                  : 
                                                 ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_isCSRRS)
                                                   ? 
                                                  (((((1U 
                                                       & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is))
                                                       ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__cycle)
                                                       : 0U) 
                                                     | ((4U 
                                                         & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is))
                                                         ? (IData)(
                                                                   (vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__cycle 
                                                                    >> 0x20U))
                                                         : 0U)) 
                                                    | ((2U 
                                                        & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is))
                                                        ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__instret)
                                                        : 0U)) 
                                                   | ((8U 
                                                       & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is))
                                                       ? (IData)(
                                                                 (vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__instret 
                                                                  >> 0x20U))
                                                       : 0U))
                                                   : vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult));
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__MW_rdId = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__MW_wbEnable = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__MW_wbData = 0U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1 
        = __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3 
        = __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2 
        = __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0 
        = __Vdly__SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__cycle = __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__cycle;
    vlTOPp->SOC__DOT__CPU__DOT__CSR__DOT__instret = __Vdly__SOC__DOT__CPU__DOT__CSR__DOT__instret;
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__aluBusy) {
            vlTOPp->SOC__DOT__CPU__DOT__EM_nop = 1U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_nop 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_nop;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_nop = 1U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR = vlTOPp->SOC__DOT__CPU__DOT__D_isJALR;
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJAL = (1U 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 3U));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJAL = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id = (0x1fU 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0xfU));
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_rdId 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_rdId;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_rdId = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__aluBusy) {
            vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable 
                    = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_wbEnable) 
                       & (0U != (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId)));
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable = 0U;
    }
    vlTOPp->SOC__DOT__data_rdata = vlTOPp->SOC__DOT__DATARAM__DOT__mem
        [(0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__E_addr 
                     >> 2U))];
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                    = vlTOPp->SOC__DOT__CPU__DOT__E_addr;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_addr = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is 
                    = (0xfU & ((IData)(1U) << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_csrId)));
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_csrId_is = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_funct3 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_funct3;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_funct3 = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__aluBusy) {
            vlTOPp->SOC__DOT__CPU__DOT__EM_isLoad = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_isLoad 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_isLoad;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_isLoad = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__CPU__DOT__aluBusy) {
            vlTOPp->SOC__DOT__CPU__DOT__EM_isCSRRS = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_isCSRRS 
                    = vlTOPp->SOC__DOT__CPU__DOT__DE_isCSRRS;
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_isCSRRS = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult 
                    = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC)
                        ? vlTOPp->SOC__DOT__CPU__DOT__DE_PCplus4orUimm
                        : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isRV32M)
                            ? ((((1U & ((IData)(1U) 
                                        << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                  ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply)
                                  : 0U) | ((0U != (7U 
                                                   & (((IData)(1U) 
                                                       << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                      >> 1U)))
                                            ? (IData)(
                                                      (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply 
                                                       >> 0x20U))
                                            : 0U)) 
                               | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV)
                                   ? ((4U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divsel))
                                       ? vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r
                                       : ((5U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divsel))
                                           ? (- vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r)
                                           : ((6U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divsel))
                                               ? vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r
                                               : (- vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r))))
                                   : 0U)) : (((((((
                                                   ((1U 
                                                     & ((IData)(1U) 
                                                        << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                     ? 
                                                    (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct7) 
                                                      & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isALUreg))
                                                      ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus)
                                                      : vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus)
                                                     : 0U) 
                                                   | ((2U 
                                                       & ((IData)(1U) 
                                                          << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                       ? vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__leftshift
                                                       : 0U)) 
                                                  | ((4U 
                                                      & ((IData)(1U) 
                                                         << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                      ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT)
                                                      : 0U)) 
                                                 | ((8U 
                                                     & ((IData)(1U) 
                                                        << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                     ? 
                                                    (1U 
                                                     & (IData)(
                                                               (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                >> 0x20U)))
                                                     : 0U)) 
                                                | ((0x10U 
                                                    & ((IData)(1U) 
                                                       << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                    ? 
                                                   (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                    ^ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)
                                                    : 0U)) 
                                               | ((0x20U 
                                                   & ((IData)(1U) 
                                                      << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                   ? vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__shifter
                                                   : 0U)) 
                                              | ((0x40U 
                                                  & ((IData)(1U) 
                                                     << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                  ? 
                                                 (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                  | vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)
                                                  : 0U)) 
                                             | ((0x80U 
                                                 & ((IData)(1U) 
                                                    << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)))
                                                 ? 
                                                (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)
                                                 : 0U))));
            }
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult = 0U;
    }
    if (__Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v0) {
        vlTOPp->SOC__DOT__DATARAM__DOT__mem[__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v0] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v0))) 
                & vlTOPp->SOC__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v0]) 
               | ((IData)(__Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v0) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v0)));
    }
    if (__Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v1) {
        vlTOPp->SOC__DOT__DATARAM__DOT__mem[__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v1] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v1))) 
                & vlTOPp->SOC__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v1]) 
               | ((IData)(__Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v1) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v1)));
    }
    if (__Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v2) {
        vlTOPp->SOC__DOT__DATARAM__DOT__mem[__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v2] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v2))) 
                & vlTOPp->SOC__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v2]) 
               | ((IData)(__Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v2) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v2)));
    }
    if (__Vdlyvset__SOC__DOT__DATARAM__DOT__mem__v3) {
        vlTOPp->SOC__DOT__DATARAM__DOT__mem[__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v3] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v3))) 
                & vlTOPp->SOC__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__SOC__DOT__DATARAM__DOT__mem__v3]) 
               | ((IData)(__Vdlyvval__SOC__DOT__DATARAM__DOT__mem__v3) 
                  << (IData)(__Vdlyvlsb__SOC__DOT__DATARAM__DOT__mem__v3)));
    }
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r 
        = __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r 
        = __Vdly__SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_nop = 0U;
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_nop = 1U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_rdId = (0x1fU 
                                               & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                  >> 7U));
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_wbEnable = (8U 
                                                   != 
                                                   (0xfU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_wbEnable = 0U;
    }
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
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h 
        = (0xffffU & ((2U & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                       ? (vlTOPp->SOC__DOT__data_rdata 
                          >> 0x10U) : vlTOPp->SOC__DOT__data_rdata));
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_csrId = ((2U 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x1aU)) 
                                                | (1U 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 0x15U)));
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isLoad = (0U 
                                                 == 
                                                 (0x1fU 
                                                  & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 2U)));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isLoad = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isCSRRS = ((0x1cU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U))) 
                                                  & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 0xdU));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isCSRRS = 0U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divstep_do 
        = (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r 
           <= (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r)));
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_funct7 = (1U 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x1eU));
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_funct3 = (7U 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0xcU));
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC 
            = (1U & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                     >> 2U));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isRV32M = ((0xcU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U))) 
                                                  & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 0x19U));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isRV32M = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV = (((0xcU 
                                                  == 
                                                  (0x1fU 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 2U))) 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x19U)) 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0xeU));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_PCplus4orUimm 
            = (((- (IData)((1U != (3U & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                         >> 5U))))) 
                & vlTOPp->SOC__DOT__CPU__DOT__FD_PC) 
               + ((1U & ((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                          >> 2U) & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                    >> 6U))) ? 4U : 
                  (0xfffff000U & vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg)));
    }
    if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isALUreg = (0xcU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)));
    }
    if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->SOC__DOT__CPU__DOT__DE_isALUreg = 0U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__E_rs1 = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id)))
                                          ? vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id)))
                                              ? vlTOPp->SOC__DOT__CPU__DOT__MW_wbData
                                              : ((0U 
                                                  == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id))
                                                  ? 0U
                                                  : 
                                                 vlTOPp->SOC__DOT__CPU__DOT__RF__DOT__regs
                                                 [vlTOPp->SOC__DOT__CPU__DOT__DE_rs1Id])));
    vlTOPp->SOC__DOT__CPU__DOT__E_rs2 = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs2Id)))
                                          ? vlTOPp->SOC__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs2Id)))
                                              ? vlTOPp->SOC__DOT__CPU__DOT__MW_wbData
                                              : ((0U 
                                                  == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rs2Id))
                                                  ? 0U
                                                  : 
                                                 vlTOPp->SOC__DOT__CPU__DOT__RF__DOT__regs
                                                 [vlTOPp->SOC__DOT__CPU__DOT__DE_rs2Id])));
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b 
        = (0xffU & ((1U & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                     ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                        >> 8U) : (IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h)));
    vlTOPp->SOC__DOT__CPU__DOT__E_addr = (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                          + vlTOPp->SOC__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign 
        = (1U & ((~ ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3) 
                     >> 2U)) & ((0U == (3U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                 ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b) 
                                    >> 7U) : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                                              >> 0xfU))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
        = vlTOPp->SOC__DOT__CPU__DOT__E_rs1;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__shifter_in 
        = ((1U == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3))
            ? vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__Vfuncout
            : vlTOPp->SOC__DOT__CPU__DOT__E_rs1);
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divsel = 
        (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
          << 2U) | ((2U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                    | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__sign_r)));
    vlTOPp->SOC__DOT__CPU__DOT__aluBusy = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                                           | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
                                              & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r))));
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictPC) 
             & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
            __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
            __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (vlTOPp->SOC__DOT__CPU__DOT__F_redirect) {
            __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) {
            __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
            __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (((IData)(vlTOPp->SOC__DOT__inst_valid_bypass) 
             & (IData)(vlTOPp->SOC__DOT__CPU__DOT__F_pendingValid))) {
            if ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__F_pendingEpoch) 
                  != (IData)(vlTOPp->SOC__DOT__CPU__DOT__F_epochNext)) 
                 | (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC))) {
                if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
                    __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
                }
            } else {
                if ((1U & ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)) 
                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)))) {
                    vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                        = vlTOPp->SOC__DOT__CPU__DOT__F_pendingPC;
                    vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                        = vlTOPp->SOC__DOT__inst_rdata;
                    __Vdly__SOC__DOT__CPU__DOT__FD_nop = 0U;
                } else {
                    __Vdly__SOC__DOT__CPU__DOT__FB_PC 
                        = vlTOPp->SOC__DOT__CPU__DOT__F_pendingPC;
                    __Vdly__SOC__DOT__CPU__DOT__FB_instr 
                        = vlTOPp->SOC__DOT__inst_rdata;
                    __Vdly__SOC__DOT__CPU__DOT__FB_epoch 
                        = vlTOPp->SOC__DOT__CPU__DOT__F_pendingEpoch;
                    __Vdly__SOC__DOT__CPU__DOT__FB_valid = 1U;
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall)))) {
                if (((IData)(vlTOPp->SOC__DOT__CPU__DOT__FB_valid) 
                     & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__FB_epoch) 
                        == (IData)(vlTOPp->SOC__DOT__CPU__DOT__F_epochNext)))) {
                    vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                        = vlTOPp->SOC__DOT__CPU__DOT__FB_PC;
                    vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                        = vlTOPp->SOC__DOT__CPU__DOT__FB_instr;
                    __Vdly__SOC__DOT__CPU__DOT__FD_nop = 0U;
                    __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
                } else {
                    if (vlTOPp->SOC__DOT__CPU__DOT__FB_valid) {
                        __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
                        __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
                    } else {
                        __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
                    }
                }
            }
        }
    } else {
        __Vdly__SOC__DOT__CPU__DOT__FB_valid = 0U;
        __Vdly__SOC__DOT__CPU__DOT__FD_nop = 1U;
        __Vdly__SOC__DOT__CPU__DOT__FB_PC = 0U;
        __Vdly__SOC__DOT__CPU__DOT__FB_instr = 0x33U;
        __Vdly__SOC__DOT__CPU__DOT__FB_epoch = 0U;
        vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg = 0x33U;
        vlTOPp->SOC__DOT__CPU__DOT__FD_PC = 0U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2 = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isALUreg) 
                                             | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch))
                                             ? vlTOPp->SOC__DOT__CPU__DOT__E_rs2
                                             : vlTOPp->SOC__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->SOC__DOT__CPU__DOT__FB_PC = __Vdly__SOC__DOT__CPU__DOT__FB_PC;
    vlTOPp->SOC__DOT__CPU__DOT__FB_instr = __Vdly__SOC__DOT__CPU__DOT__FB_instr;
    vlTOPp->SOC__DOT__CPU__DOT__FB_epoch = __Vdly__SOC__DOT__CPU__DOT__FB_epoch;
    vlTOPp->SOC__DOT__CPU__DOT__FB_valid = __Vdly__SOC__DOT__CPU__DOT__FB_valid;
    vlTOPp->SOC__DOT__CPU__DOT__FD_nop = __Vdly__SOC__DOT__CPU__DOT__FD_nop;
    vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply 
        = VL_MULS_QQQ(64,64,64, VL_EXTENDS_QQ(64,33, 
                                              (((QData)((IData)(
                                                                (1U 
                                                                 & ((vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                                     >> 0x1fU) 
                                                                    & (((IData)(1U) 
                                                                        << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                                       >> 1U))))) 
                                                << 0x20U) 
                                               | (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_rs1)))), 
                      VL_EXTENDS_QQ(64,33, (((QData)((IData)(
                                                             (1U 
                                                              & ((vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2 
                                                                  >> 0x1fU) 
                                                                 & ((0x7fU 
                                                                     & (((IData)(1U) 
                                                                         << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                                        >> 1U)) 
                                                                    | (0x3fU 
                                                                       & (((IData)(1U) 
                                                                           << (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                                                                          >> 2U))))))) 
                                             << 0x20U) 
                                            | (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)))));
    vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__shifter = (IData)(
                                                            (0x1ffffffffULL 
                                                             & VL_SHIFTRS_QQI(33,33,5, 
                                                                              (((QData)((IData)(
                                                                                ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct7) 
                                                                                & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                                                >> 0x1fU)))) 
                                                                                << 0x20U) 
                                                                               | (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__shifter_in))), 
                                                                              (0x1fU 
                                                                               & vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2))));
    vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus = (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                             + vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2);
    vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
        = (0x1ffffffffULL & (1ULL + ((0x100000000ULL 
                                      | (QData)((IData)(
                                                        (~ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2)))) 
                                     + (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_rs1)))));
    if (vlTOPp->SOC__DOT__inst_en) {
        vlTOPp->SOC__DOT__inst_rdata = vlTOPp->SOC__DOT__PROGROM__DOT__mem
            [(0x3fffU & (vlTOPp->SOC__DOT__CPU__DOT__F_PC 
                         >> 2U))];
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__inst_en) {
            vlTOPp->SOC__DOT__CPU__DOT__F_pendingEpoch 
                = vlTOPp->SOC__DOT__CPU__DOT__F_epochNext;
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__F_pendingEpoch = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->SOC__DOT__inst_en) {
            vlTOPp->SOC__DOT__CPU__DOT__F_pendingPC 
                = vlTOPp->SOC__DOT__CPU__DOT__F_PC;
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__F_pendingPC = 0U;
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->SOC__DOT__inst_valid_bypass) 
             & (~ (IData)(vlTOPp->SOC__DOT__inst_en)))) {
            vlTOPp->SOC__DOT__CPU__DOT__F_pendingValid = 0U;
        }
        if (vlTOPp->SOC__DOT__inst_en) {
            vlTOPp->SOC__DOT__CPU__DOT__F_pendingValid = 1U;
        }
    } else {
        vlTOPp->SOC__DOT__CPU__DOT__F_pendingValid = 0U;
    }
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__bht_index 
        = (0xfffU & ((vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                      >> 2U) ^ ((IData)(vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__branch_history) 
                                << 3U)));
    vlTOPp->SOC__DOT__CPU__DOT__D_isJALR = (5U == (
                                                   (4U 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 4U)) 
                                                   | (3U 
                                                      & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 2U))));
    vlTOPp->SOC__DOT__CPU__DOT__dataHazard = ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                              & (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isLoad) 
                                                  | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isCSRRS)) 
                                                 & ((((~ 
                                                       ((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 3U) 
                                                        | ((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 4U) 
                                                           & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                              >> 6U)))) 
                                                      & ((0x1fU 
                                                          & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                             >> 0xfU)) 
                                                         == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))) 
                                                     & (0U 
                                                        != 
                                                        (0x1fU 
                                                         & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 0xfU)))) 
                                                    | ((((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 5U) 
                                                         & (0U 
                                                            == 
                                                            (3U 
                                                             & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                                >> 2U)))) 
                                                        & ((0x1fU 
                                                            & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                               >> 0x14U)) 
                                                           == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))) 
                                                       & (0U 
                                                          != 
                                                          (0x1fU 
                                                           & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                              >> 0x14U)))))));
    vlTOPp->SOC__DOT__CPU__DOT__D_Bimm = ((0xfffff000U 
                                           & ((- (IData)(
                                                         (1U 
                                                          & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                             >> 0x1fU)))) 
                                              << 0xcU)) 
                                          | ((0x800U 
                                              & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                 << 4U)) 
                                             | ((0x7e0U 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x14U)) 
                                                | (0x1eU 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 7U)))));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
        = vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__shifter;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__leftshift 
        = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout;
    vlTOPp->SOC__DOT__CPU__DOT__E_PCcorrection = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch)
                                                   ? vlTOPp->SOC__DOT__CPU__DOT__DE_PCplus4orBimm
                                                   : 
                                                  (0xfffffffeU 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus));
    vlTOPp->SOC__DOT__CPU__DOT__E_LT = (1U & ((0x80000000U 
                                               & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                  ^ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2))
                                               ? (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                  >> 0x1fU)
                                               : (IData)(
                                                         (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                          >> 0x20U))));
    vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch = (1U 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__BHT
                                                      [vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__bht_index] 
                                                      >> 1U));
    vlTOPp->SOC__DOT__CPU__DOT____Vcellinp__DIVU__start 
        = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
            & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard))) 
           & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r)));
    vlTOPp->SOC__DOT__CPU__DOT__D_PCprediction = ((1U 
                                                   == 
                                                   (3U 
                                                    & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)))
                                                   ? vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_spec_0
                                                   : 
                                                  (vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                                                   + 
                                                   ((8U 
                                                     & vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg)
                                                     ? 
                                                    ((0xfff00000U 
                                                      & ((- (IData)(
                                                                    (1U 
                                                                     & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                                        >> 0x1fU)))) 
                                                         << 0x14U)) 
                                                     | ((0xff000U 
                                                         & vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg) 
                                                        | ((0x800U 
                                                            & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                               >> 9U)) 
                                                           | (0x7feU 
                                                              & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                                 >> 0x14U)))))
                                                     : vlTOPp->SOC__DOT__CPU__DOT__D_Bimm)));
    vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch) 
                                                & ((1U 
                                                    & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is))
                                                    ? 
                                                   (0U 
                                                    == (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is))
                                                     ? 
                                                    (0U 
                                                     != (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus))
                                                     : 
                                                    ((0x10U 
                                                      & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is))
                                                      ? (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT)
                                                      : 
                                                     ((0x20U 
                                                       & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is))
                                                       ? 
                                                      (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT))
                                                       : 
                                                      ((0x40U 
                                                        & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is))
                                                        ? (IData)(
                                                                  (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                   >> 0x20U))
                                                        : 
                                                       (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                         >> 7U) 
                                                        & (~ (IData)(
                                                                     (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                      >> 0x20U))))))))));
    vlTOPp->SOC__DOT__inst_valid_bypass = ((IData)(vlTOPp->RESET) 
                                           & (IData)(vlTOPp->SOC__DOT__inst_en));
    vlTOPp->SOC__DOT__CPU__DOT__D_predictPC = (1U & 
                                               ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                                & ((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 3U) 
                                                   | ((6U 
                                                       == 
                                                       (7U 
                                                        & (vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                           >> 4U))) 
                                                      & ((vlTOPp->SOC__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 2U) 
                                                         | (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch))))));
    vlTOPp->SOC__DOT__CPU__DOT__E_correctPC = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR) 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__DE_predictRA 
                                                   != 
                                                   (0xfffffffeU 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus))) 
                                               | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch) 
                                                  & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch) 
                                                     ^ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_predictBranch))));
    vlTOPp->SOC__DOT__CPU__DOT__E_flush = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard));
    vlTOPp->SOC__DOT__CPU__DOT__F_PC = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC)
                                         ? vlTOPp->SOC__DOT__CPU__DOT__E_PCcorrection
                                         : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictPC)
                                             ? vlTOPp->SOC__DOT__CPU__DOT__D_PCprediction
                                             : vlTOPp->SOC__DOT__CPU__DOT__PC));
    vlTOPp->SOC__DOT__CPU__DOT__F_redirect = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) 
                                              | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictPC) 
                                                 & (vlTOPp->SOC__DOT__CPU__DOT__F_pendingPC 
                                                    != vlTOPp->SOC__DOT__CPU__DOT__D_PCprediction)));
    vlTOPp->SOC__DOT__CPU__DOT__F_epochNext = (3U & 
                                               ((IData)(vlTOPp->SOC__DOT__CPU__DOT__F_epoch) 
                                                + ((IData)(vlTOPp->SOC__DOT__CPU__DOT__F_redirect)
                                                    ? 1U
                                                    : 0U)));
}

VL_INLINE_OPT void VSOC::_combo__TOP__4(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_combo__TOP__4\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->HALT = ((IData)(vlTOPp->RESET) & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isEBREAK));
    vlTOPp->SOC__DOT__IO_mem_wr = ((IData)(vlTOPp->RESET) 
                                   & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_isStore) 
                                      & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                         >> 0x16U)));
    vlTOPp->SOC__DOT__data_wmask = ((IData)(vlTOPp->RESET)
                                     ? ((- (IData)(
                                                   ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_isStore) 
                                                    & (~ 
                                                       (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                                        >> 0x16U))))) 
                                        & ((0U == (3U 
                                                   & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                            ? ((2U 
                                                & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                ? (
                                                   (1U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 8U
                                                    : 4U)
                                                : (
                                                   (1U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 2U
                                                    : 1U))
                                            : ((1U 
                                                == 
                                                (3U 
                                                 & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                                ? (
                                                   (2U 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                                                    ? 0xcU
                                                    : 3U)
                                                : 0xfU)))
                                     : 0U);
    vlTOPp->SOC__DOT__CPU__DOT__D_stall = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy) 
                                            | (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard)) 
                                           | (IData)(vlTOPp->HALT));
    vlTOPp->SOC__DOT__uart_valid = ((IData)(vlTOPp->SOC__DOT__IO_mem_wr) 
                                    & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                       >> 3U));
    vlTOPp->SOC__DOT__inst_en = ((((IData)(vlTOPp->RESET) 
                                   & (~ (IData)(vlTOPp->HALT))) 
                                  & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_stall))) 
                                 & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FB_valid)));
}

void VSOC::_eval(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_eval\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->CLK) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLK)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__CLK = vlTOPp->CLK;
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
