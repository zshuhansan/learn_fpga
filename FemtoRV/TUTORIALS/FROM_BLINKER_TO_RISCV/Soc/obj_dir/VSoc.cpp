// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSoc.h for the primary calling header

#include "VSoc.h"
#include "VSoc__Syms.h"

//==========

void VSoc::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VSoc::eval\n"); );
    VSoc__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("src/Soc.v", 17, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VSoc::_eval_initial_loop(VSoc__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("src/Soc.v", 17, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void VSoc::_sequent__TOP__2(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_sequent__TOP__2\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__Vfuncout;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__Vfuncout;
    CData/*1:0*/ __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__Vfuncout;
    CData/*1:0*/ __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev;
    CData/*0:0*/ __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir;
    CData/*0:0*/ __Vdly__Soc__DOT__CPU__DOT__FD_nop;
    CData/*0:0*/ __Vdly__Soc__DOT__CPU__DOT__FB_valid;
    CData/*1:0*/ __Vdly__Soc__DOT__CPU__DOT__FB_epoch;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtUseCount__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtPC0__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtAliasCount__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtPC1__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtPC2__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtPC3__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__phtUniqueCount__v4;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfValid__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfPC__v0;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v0;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v0;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v1;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__brProfTotal__v1;
    CData/*7:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v1;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__brProfHit__v1;
    CData/*1:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0;
    CData/*4:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__RF__DOT__regs__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__CPU__DOT__RF__DOT__regs__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v1;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v1;
    CData/*2:0*/ __Vdly__Soc__DOT__ICACHE__DOT__state;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way1__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way1__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v2;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way0__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way0__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0;
    CData/*6:0*/ __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v3;
    CData/*1:0*/ __Vdly__Soc__DOT__AXI_DDR__DOT__state;
    CData/*7:0*/ __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left;
    CData/*3:0*/ __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt;
    CData/*4:0*/ __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v0;
    CData/*7:0*/ __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v0;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v0;
    CData/*4:0*/ __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v1;
    CData/*7:0*/ __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v1;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v1;
    CData/*4:0*/ __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v2;
    CData/*7:0*/ __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v2;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v2;
    CData/*4:0*/ __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v3;
    CData/*7:0*/ __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v3;
    CData/*0:0*/ __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v3;
    CData/*0:0*/ __Vdly__Soc__DOT__uart_ready;
    CData/*7:0*/ __Vdly__Soc__DOT__UART__DOT__cnt;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUseCount__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC0__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtAliasCount__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC1__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v1;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC2__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v2;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC3__v0;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v3;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v4;
    SData/*11:0*/ __Vdlyvdim0__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0;
    SData/*13:0*/ __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v0;
    SData/*13:0*/ __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v1;
    SData/*13:0*/ __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v2;
    SData/*13:0*/ __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v3;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__I;
    IData/*31:0*/ __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__I;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__FB_PC;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__FB_instr;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtUseCount__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtPC0__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtAliasCount__v0;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtPC1__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtPC2__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__phtPC3__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__brProfPC__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v0;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__brProfTotal__v1;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v1;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    IData/*31:0*/ __Vdlyvval__Soc__DOT__CPU__DOT__RF__DOT__regs__v0;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    IData/*31:0*/ __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    IData/*19:0*/ __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way1__v0;
    WData/*255:0*/ __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[8];
    IData/*19:0*/ __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way0__v0;
    WData/*255:0*/ __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[8];
    IData/*31:0*/ __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr;
    QData/*63:0*/ __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__cycle;
    QData/*63:0*/ __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__instret;
    // Body
    __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__cycle = vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle;
    __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt = vlTOPp->Soc__DOT__AXI_DDR__DOT__latency_cnt;
    __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left = vlTOPp->Soc__DOT__AXI_DDR__DOT__burst_left;
    __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr = vlTOPp->Soc__DOT__AXI_DDR__DOT__current_addr;
    __Vdly__Soc__DOT__AXI_DDR__DOT__state = vlTOPp->Soc__DOT__AXI_DDR__DOT__state;
    __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v0 = 0U;
    __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v1 = 0U;
    __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__instret = vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret;
    __Vdly__Soc__DOT__ICACHE__DOT__state = vlTOPp->Soc__DOT__ICACHE__DOT__state;
    __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0 = 0U;
    __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0 = 0U;
    __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v0 = 0U;
    __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v1 = 0U;
    __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v2 = 0U;
    __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v3 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0 = 0U;
    __Vdly__Soc__DOT__CPU__DOT__FB_epoch = vlTOPp->Soc__DOT__CPU__DOT__FB_epoch;
    __Vdly__Soc__DOT__CPU__DOT__FB_instr = vlTOPp->Soc__DOT__CPU__DOT__FB_instr;
    __Vdly__Soc__DOT__CPU__DOT__FB_PC = vlTOPp->Soc__DOT__CPU__DOT__FB_PC;
    __Vdly__Soc__DOT__CPU__DOT__FB_valid = vlTOPp->Soc__DOT__CPU__DOT__FB_valid;
    __Vdly__Soc__DOT__CPU__DOT__FD_nop = vlTOPp->Soc__DOT__CPU__DOT__FD_nop;
    __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
        = vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtAliasCount__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtUseCount__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__brProfTotal__v1 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__brProfHit__v1 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtPC3__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtPC2__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtPC1__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtUniqueCount__v4 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__phtPC0__v0 = 0U;
    __Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0 = 0U;
    __Vdly__Soc__DOT__uart_ready = vlTOPp->Soc__DOT__uart_ready;
    __Vdly__Soc__DOT__UART__DOT__cnt = vlTOPp->Soc__DOT__UART__DOT__cnt;
    __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
        = vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r 
        = vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r 
        = vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    __Vdlyvset__Soc__DOT__CPU__DOT__RF__DOT__regs__v0 = 0U;
    if (VL_UNLIKELY(vlTOPp->HALT)) {
        VL_FINISH_MT("src/processor.v", 802, "");
    }
    if (VL_UNLIKELY(vlTOPp->HALT)) {
        VL_WRITEF("Simulated processor's report\n----------------------------\n");
        VL_WRITEF("Branch hit = %3.3f%%\n",64,((100.0 
                                                * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranchHit)) 
                                               / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranch)));
        VL_WRITEF("Branch bwd = %3.3f%% (%0d/%0d)\n",
                  64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackwardHit)) 
                      / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackward)),
                  32,vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackwardHit,
                  32,vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackward);
        VL_WRITEF("Branch fwd = %3.3f%% (%0d/%0d)\n",
                  64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranchForwardHit)) 
                      / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranchForward)),
                  32,vlTOPp->Soc__DOT__CPU__DOT__nbBranchForwardHit,
                  32,vlTOPp->Soc__DOT__CPU__DOT__nbBranchForward);
        VL_WRITEF("JALR   hit = %3.3f%%\n",64,((100.0 
                                                * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbJALRhit)) 
                                               / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbJALR)));
        VL_WRITEF("Load hzrds = %3.3f%%\n",64,((100.0 
                                                * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbLoadHazard)) 
                                               / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbLoad)));
        VL_WRITEF("CPI        = %3.3f\n",64,((1.0 * 
                                              VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle))) 
                                             / (1.0 
                                                * VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret)))));
        VL_WRITEF("PHT shared entries = %0d\n",32,vlTOPp->Soc__DOT__CPU__DOT__nbPHTSharedEntries);
        VL_WRITEF("PHT extra PC refs  = %0d\n",32,vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1);
        VL_WRITEF("Instr. mix = (");
        VL_WRITEF("Branch:%3.3f%%",64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbBranch)) 
                                       / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" JAL:%3.3f%%",64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbJAL)) 
                                     / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" JALR:%3.3f%%",64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbJALR)) 
                                      / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" Load:%3.3f%%",64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbLoad)) 
                                      / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" Store:%3.3f%%",64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbStore)) 
                                       / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" MUL(HSU):%3.3f%% ",64,((100.0 * 
                                            VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbMUL)) 
                                           / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(" DIV/REM:%3.3f%% ",64,((100.0 * 
                                           VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__nbDIV)) 
                                          / VL_ITOR_D_I((IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret))));
        VL_WRITEF(")\n");
        vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_i)) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
            vlTOPp->Soc__DOT__CPU__DOT__stats_i = ((IData)(1U) 
                                                   + vlTOPp->Soc__DOT__CPU__DOT__stats_i);
        }
        VL_WRITEF("Top hot branches\n");
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                 [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                 & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                    [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_i)) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
            vlTOPp->Soc__DOT__CPU__DOT__stats_i = ((IData)(1U) 
                                                   + vlTOPp->Soc__DOT__CPU__DOT__stats_i);
        }
        VL_WRITEF("Worst hot branches\n");
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                  [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)] 
                  & (~ vlTOPp->Soc__DOT__CPU__DOT__reportUsed
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) 
                 & VL_LTES_III(1,32,32, 0x14U, vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                               [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]))) {
                if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best)) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                } else {
                    if (VL_LTS_III(1,32,32, VL_DIVS_III(32, 
                                                        VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                                    vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                                    [
                                                                    (0xffU 
                                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                                        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                                        [
                                                        (0xffU 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                   VL_DIVS_III(32, 
                                               VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                           vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                           [
                                                           (0xffU 
                                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                               vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                        vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                            = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                    } else {
                        if (((VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                          vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                          [
                                                          (0xffU 
                                                           & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]), 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]) 
                              == VL_DIVS_III(32, VL_MULS_III(32,32,32, (IData)(0x3e8U), 
                                                             vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                                             [
                                                             (0xffU 
                                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]), 
                                             vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                             [(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                             & VL_GTS_III(1,32,32, 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                          vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                          [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                            vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                                = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                        }
                    }
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best))) {
            vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 1U;
            VL_WRITEF("  pc=%x count=%0d hit=%3.3f%%\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      64,((100.0 * VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                                               [(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])) 
                          / VL_ITOR_D_I(vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])));
        }
        VL_WRITEF("Hot shared PHT entries\n");
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_best = 0xffffffffU;
        vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0U;
        while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
            if (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                           [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)])) {
                if (((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                     | VL_GTS_III(1,32,32, vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)], 
                                  vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                  [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_best 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_slot;
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
        }
        if (VL_UNLIKELY(((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_best) 
                         & VL_LTS_III(1,32,32, 0U, 
                                      vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)])))) {
            VL_WRITEF("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%x pc1=%x\n",
                      32,vlTOPp->Soc__DOT__CPU__DOT__stats_best,
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)],
                      32,vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_best)]);
            vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__stats_best)] = 0xffffffffU;
        }
        vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0xaU;
        VL_FINISH_MT("src/processor.v", 1277, "");
    }
    if (VL_UNLIKELY(vlTOPp->Soc__DOT__uart_valid)) {
        VL_WRITEF("%c",8,(0xffU & vlTOPp->Soc__DOT__CPU__DOT__EM_rs2));
        if (0x80000001U) { VL_FFLUSH_I(0x80000001U); }
    }
    __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__cycle = ((IData)(vlTOPp->RESET)
                                                    ? 
                                                   (1ULL 
                                                    + vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle)
                                                    : 0ULL);
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__F_redirect) {
            vlTOPp->Soc__DOT__CPU__DOT__F_epoch = vlTOPp->Soc__DOT__CPU__DOT__F_epochNext;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__F_epoch = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__gshare_commit) {
            vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch_next;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch = 0U;
    }
    if (vlTOPp->RESET) {
        if ((0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state))) {
            if (((1U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
                 & (IData)(vlTOPp->Soc__DOT__axi_arready))) {
                __Vdly__Soc__DOT__AXI_DDR__DOT__state = 1U;
                __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr 
                    = vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr;
                __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left = 7U;
                __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt = 0xaU;
            }
        } else {
            if ((1U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state))) {
                if ((0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__latency_cnt))) {
                    __Vdly__Soc__DOT__AXI_DDR__DOT__state = 2U;
                } else {
                    __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt 
                        = (0xfU & ((IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__latency_cnt) 
                                   - (IData)(1U)));
                }
            } else {
                if ((2U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state))) {
                    if (((IData)(vlTOPp->Soc__DOT__axi_rvalid) 
                         & (2U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)))) {
                        if ((0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__burst_left))) {
                            __Vdly__Soc__DOT__AXI_DDR__DOT__state = 0U;
                        } else {
                            __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left 
                                = (0xffU & ((IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__burst_left) 
                                            - (IData)(1U)));
                            __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr 
                                = ((IData)(4U) + vlTOPp->Soc__DOT__AXI_DDR__DOT__current_addr);
                        }
                    }
                }
            }
        }
    } else {
        __Vdly__Soc__DOT__AXI_DDR__DOT__state = 0U;
        __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr = 0U;
        __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left = 0U;
        __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt = 0U;
    }
    if (vlTOPp->Soc__DOT__CPU__DOT__dataHazard) {
        vlTOPp->Soc__DOT__CPU__DOT__nbLoadHazard = 
            ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbLoadHazard);
    }
    if (vlTOPp->RESET) {
        if (VL_UNLIKELY(((IData)(vlTOPp->HALT) & (~ (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__stats_printed))))) {
            VL_WRITEF("ICACHE_STATS hit=%0d miss=%0d\n",
                      32,vlTOPp->Soc__DOT__ICACHE__DOT__hit_count,
                      32,vlTOPp->Soc__DOT__ICACHE__DOT__miss_count);
            vlTOPp->Soc__DOT__ICACHE__DOT__stats_printed = 1U;
        }
    } else {
        vlTOPp->Soc__DOT__ICACHE__DOT__stats_printed = 0U;
    }
    if (((IData)(vlTOPp->RESET) & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__is_hit))) {
        if (vlTOPp->Soc__DOT__ICACHE__DOT__hit_way0) {
            __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v0 = 1U;
            __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v0 
                = (0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                            >> 5U));
        }
        if (vlTOPp->Soc__DOT__ICACHE__DOT__hit_way1_raw) {
            __Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v1 = 1U;
            __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v1 
                = (0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                            >> 5U));
        }
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_nop)))) {
            __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__instret 
                = (1ULL + vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret);
        }
    } else {
        __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__instret = 0ULL;
    }
    if ((1U & (IData)(vlTOPp->Soc__DOT__data_wmask))) {
        __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v0 
            = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data);
        __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v0 = 1U;
        __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v0 = 0U;
        __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v0 
            = (0x3fffU & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((2U & (IData)(vlTOPp->Soc__DOT__data_wmask))) {
        __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v1 
            = (0xffU & (vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data 
                        >> 8U));
        __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v1 = 1U;
        __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v1 = 8U;
        __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v1 
            = (0x3fffU & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((4U & (IData)(vlTOPp->Soc__DOT__data_wmask))) {
        __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v2 
            = (0xffU & (vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data 
                        >> 0x10U));
        __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v2 = 1U;
        __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v2 = 0x10U;
        __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v2 
            = (0x3fffU & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if ((8U & (IData)(vlTOPp->Soc__DOT__data_wmask))) {
        __Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v3 
            = (0xffU & (vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data 
                        >> 0x18U));
        __Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v3 = 1U;
        __Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v3 = 0x18U;
        __Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v3 
            = (0x3fffU & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                          >> 2U));
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC) 
             & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
            vlTOPp->Soc__DOT__CPU__DOT__PC = vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) {
            vlTOPp->Soc__DOT__CPU__DOT__PC = vlTOPp->Soc__DOT__CPU__DOT__E_PCcorrection;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__F_reqFire) {
            vlTOPp->Soc__DOT__CPU__DOT__PC = ((IData)(4U) 
                                              + vlTOPp->Soc__DOT__CPU__DOT__F_PC);
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__PC = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history 
        = ((IData)(vlTOPp->RESET) ? ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC)
                                      ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch_next)
                                      : (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_spec_next))
            : 0U);
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__gshare_commit) {
            __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir 
                = vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch;
            __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__BHT
                [vlTOPp->Soc__DOT__CPU__DOT__DE_BHTindex];
            __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__Vfuncout 
                = ((0U == (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                            << 2U) | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                    ? 0U : ((1U == (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                                     << 2U) | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                             ? 0U : ((2U == (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                                              << 2U) 
                                             | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                                      ? 1U : ((3U == 
                                               (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                                                 << 2U) 
                                                | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                                               ? 2U
                                               : ((4U 
                                                   == 
                                                   (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                                                     << 2U) 
                                                    | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                                                   ? 1U
                                                   : 
                                                  ((5U 
                                                    == 
                                                    (((IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__dir) 
                                                      << 2U) 
                                                     | (IData)(__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__prev)))
                                                    ? 2U
                                                    : 3U))))));
            __Vdlyvval__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0 
                = __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__incdec_sat__8__Vfuncout;
            __Vdlyvset__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0 = 1U;
            __Vdlyvdim0__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0 
                = vlTOPp->Soc__DOT__CPU__DOT__DE_BHTindex;
        }
    }
    if (vlTOPp->RESET) {
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__I 
            = vlTOPp->Soc__DOT__CPU__DOT__DE_instr;
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__Vfuncout 
            = (0x6fU == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__I));
        if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJAL__1__Vfuncout) {
            vlTOPp->Soc__DOT__CPU__DOT__nbJAL = ((IData)(1U) 
                                                 + vlTOPp->Soc__DOT__CPU__DOT__nbJAL);
        }
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_isStore = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_isStore 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_isStore;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_isStore = 0U;
    }
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__I 
        = vlTOPp->Soc__DOT__CPU__DOT__MW_instr;
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__Vfuncout 
        = (0x23U == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__I));
    if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isStore__4__Vfuncout) {
        vlTOPp->Soc__DOT__CPU__DOT__nbStore = ((IData)(1U) 
                                               + vlTOPp->Soc__DOT__CPU__DOT__nbStore);
    }
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__I 
        = vlTOPp->Soc__DOT__CPU__DOT__MW_instr;
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__Vfuncout 
        = (3U == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__I));
    if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isLoad__3__Vfuncout) {
        vlTOPp->Soc__DOT__CPU__DOT__nbLoad = ((IData)(1U) 
                                              + vlTOPp->Soc__DOT__CPU__DOT__nbLoad);
    }
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__I 
        = vlTOPp->Soc__DOT__CPU__DOT__MW_instr;
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__I 
        = __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__I;
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__Vfuncout 
        = (0x33U == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__I));
    __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__Vfuncout 
        = ((IData)(__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isALUreg__6__Vfuncout) 
           & (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__I 
              >> 0x19U));
    if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isRV32M__5__Vfuncout) {
        if ((0x4000U & vlTOPp->Soc__DOT__CPU__DOT__MW_instr)) {
            vlTOPp->Soc__DOT__CPU__DOT__nbDIV = ((IData)(1U) 
                                                 + vlTOPp->Soc__DOT__CPU__DOT__nbDIV);
        } else {
            vlTOPp->Soc__DOT__CPU__DOT__nbMUL = ((IData)(1U) 
                                                 + vlTOPp->Soc__DOT__CPU__DOT__nbMUL);
        }
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isEBREAK = (
                                                   (0x1cU 
                                                    == 
                                                    (0x1fU 
                                                     & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                        >> 2U))) 
                                                   & (~ 
                                                      (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 0xdU)));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isEBREAK = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is = 
            (0xffU & ((IData)(1U) << (7U & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                            >> 0xcU))));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_PCplus4orBimm 
            = (vlTOPp->Soc__DOT__CPU__DOT__FD_PC + 
               ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch)
                 ? 4U : vlTOPp->Soc__DOT__CPU__DOT__D_Bimm));
    }
    if (vlTOPp->RESET) {
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__I 
            = vlTOPp->Soc__DOT__CPU__DOT__DE_instr;
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__Vfuncout 
            = (0x67U == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__I));
        if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isJALR__2__Vfuncout) {
            vlTOPp->Soc__DOT__CPU__DOT__nbJALR = ((IData)(1U) 
                                                  + vlTOPp->Soc__DOT__CPU__DOT__nbJALR);
            if ((vlTOPp->Soc__DOT__CPU__DOT__DE_predictRA 
                 == (0xfffffffeU & vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus))) {
                vlTOPp->Soc__DOT__CPU__DOT__nbJALRhit 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbJALRhit);
            }
        }
    }
    if (vlTOPp->RESET) {
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__I 
            = vlTOPp->Soc__DOT__CPU__DOT__DE_instr;
        __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__Vfuncout 
            = (0x63U == (0x7fU & __Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__I));
        if (__Vfunc_Soc__DOT__CPU__DOT__riscv_disasm_isBranch__0__Vfuncout) {
            vlTOPp->Soc__DOT__CPU__DOT__nbBranch = 
                ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranch);
            if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                 == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))) {
                vlTOPp->Soc__DOT__CPU__DOT__nbBranchHit 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranchHit);
            }
            if ((0x80000000U & vlTOPp->Soc__DOT__CPU__DOT__DE_instr)) {
                vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackward 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackward);
                if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                     == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))) {
                    vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackwardHit 
                        = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackwardHit);
                }
            } else {
                vlTOPp->Soc__DOT__CPU__DOT__nbBranchForward 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranchForward);
                if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                     == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))) {
                    vlTOPp->Soc__DOT__CPU__DOT__nbBranchForwardHit 
                        = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbBranchForwardHit);
                }
            }
            vlTOPp->Soc__DOT__CPU__DOT__stats_slot = 0xffffffffU;
            vlTOPp->Soc__DOT__CPU__DOT__stats_idx = vlTOPp->Soc__DOT__CPU__DOT__DE_BHTindex;
            vlTOPp->Soc__DOT__CPU__DOT__stats_free = 0xffffffffU;
            vlTOPp->Soc__DOT__CPU__DOT__stats_unique 
                = vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount
                [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)];
            vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0U;
            vlTOPp->Soc__DOT__CPU__DOT__stats_alias 
                = vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount
                [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)];
            while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_i)) {
                if ((vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                     [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] 
                     & (vlTOPp->Soc__DOT__CPU__DOT__brProfPC
                        [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] 
                        == vlTOPp->Soc__DOT__CPU__DOT__DE_PC))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_i;
                }
                if (((~ vlTOPp->Soc__DOT__CPU__DOT__brProfValid
                      [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_i)]) 
                     & (0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_free))) {
                    vlTOPp->Soc__DOT__CPU__DOT__stats_free 
                        = vlTOPp->Soc__DOT__CPU__DOT__stats_i;
                }
                vlTOPp->Soc__DOT__CPU__DOT__stats_i 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_i);
            }
            __Vdlyvval__Soc__DOT__CPU__DOT__phtUseCount__v0 
                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__phtUseCount
                   [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)]);
            __Vdlyvset__Soc__DOT__CPU__DOT__phtUseCount__v0 = 1U;
            __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUseCount__v0 
                = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
            if ((0U == vlTOPp->Soc__DOT__CPU__DOT__stats_unique)) {
                __Vdlyvval__Soc__DOT__CPU__DOT__phtPC0__v0 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_PC;
                __Vdlyvset__Soc__DOT__CPU__DOT__phtPC0__v0 = 1U;
                __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC0__v0 
                    = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v0 
                    = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
            } else {
                if ((1U & (~ ((((vlTOPp->Soc__DOT__CPU__DOT__phtPC0
                                 [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)] 
                                 == vlTOPp->Soc__DOT__CPU__DOT__DE_PC) 
                                | (VL_LTS_III(1,32,32, 1U, vlTOPp->Soc__DOT__CPU__DOT__stats_unique) 
                                   & (vlTOPp->Soc__DOT__CPU__DOT__phtPC1
                                      [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)] 
                                      == vlTOPp->Soc__DOT__CPU__DOT__DE_PC))) 
                               | (VL_LTS_III(1,32,32, 2U, vlTOPp->Soc__DOT__CPU__DOT__stats_unique) 
                                  & (vlTOPp->Soc__DOT__CPU__DOT__phtPC2
                                     [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)] 
                                     == vlTOPp->Soc__DOT__CPU__DOT__DE_PC))) 
                              | (VL_LTS_III(1,32,32, 3U, vlTOPp->Soc__DOT__CPU__DOT__stats_unique) 
                                 & (vlTOPp->Soc__DOT__CPU__DOT__phtPC3
                                    [(0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx)] 
                                    == vlTOPp->Soc__DOT__CPU__DOT__DE_PC)))))) {
                    __Vdlyvval__Soc__DOT__CPU__DOT__phtAliasCount__v0 
                        = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__stats_alias);
                    __Vdlyvset__Soc__DOT__CPU__DOT__phtAliasCount__v0 = 1U;
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__phtAliasCount__v0 
                        = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                    if ((1U == vlTOPp->Soc__DOT__CPU__DOT__stats_unique)) {
                        vlTOPp->Soc__DOT__CPU__DOT__nbPHTSharedEntries 
                            = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbPHTSharedEntries);
                        __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
                            = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1);
                        __Vdlyvval__Soc__DOT__CPU__DOT__phtPC1__v0 
                            = vlTOPp->Soc__DOT__CPU__DOT__DE_PC;
                        __Vdlyvset__Soc__DOT__CPU__DOT__phtPC1__v0 = 1U;
                        __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC1__v0 
                            = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                        __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v1 
                            = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                    } else {
                        if ((2U == vlTOPp->Soc__DOT__CPU__DOT__stats_unique)) {
                            __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
                                = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1);
                            __Vdlyvval__Soc__DOT__CPU__DOT__phtPC2__v0 
                                = vlTOPp->Soc__DOT__CPU__DOT__DE_PC;
                            __Vdlyvset__Soc__DOT__CPU__DOT__phtPC2__v0 = 1U;
                            __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC2__v0 
                                = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                            __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v2 
                                = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                        } else {
                            if ((3U == vlTOPp->Soc__DOT__CPU__DOT__stats_unique)) {
                                __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
                                    = ((IData)(1U) 
                                       + vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1);
                                __Vdlyvval__Soc__DOT__CPU__DOT__phtPC3__v0 
                                    = vlTOPp->Soc__DOT__CPU__DOT__DE_PC;
                                __Vdlyvset__Soc__DOT__CPU__DOT__phtPC3__v0 = 1U;
                                __Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC3__v0 
                                    = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                                __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v3 
                                    = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                            } else {
                                if (VL_GTS_III(1,32,32, 5U, vlTOPp->Soc__DOT__CPU__DOT__stats_unique)) {
                                    __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
                                        = ((IData)(1U) 
                                           + vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1);
                                    __Vdlyvset__Soc__DOT__CPU__DOT__phtUniqueCount__v4 = 1U;
                                    __Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v4 
                                        = (0xfffU & vlTOPp->Soc__DOT__CPU__DOT__stats_idx);
                                }
                            }
                        }
                    }
                }
            }
            if ((0xffffffffU == vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
                vlTOPp->Soc__DOT__CPU__DOT__stats_slot 
                    = vlTOPp->Soc__DOT__CPU__DOT__stats_free;
                if ((0xffffffffU != vlTOPp->Soc__DOT__CPU__DOT__stats_slot)) {
                    __Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0 = 1U;
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfValid__v0 
                        = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                    __Vdlyvval__Soc__DOT__CPU__DOT__brProfPC__v0 
                        = vlTOPp->Soc__DOT__CPU__DOT__DE_PC;
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfPC__v0 
                        = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v0 
                        = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                    __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v0 
                        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                            == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))
                            ? 1U : 0U);
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v0 
                        = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                }
            } else {
                __Vdlyvval__Soc__DOT__CPU__DOT__brProfTotal__v1 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__brProfTotal
                       [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]);
                __Vdlyvset__Soc__DOT__CPU__DOT__brProfTotal__v1 = 1U;
                __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v1 
                    = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                     == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))) {
                    __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v1 
                        = ((IData)(1U) + vlTOPp->Soc__DOT__CPU__DOT__brProfHit
                           [(0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot)]);
                    __Vdlyvset__Soc__DOT__CPU__DOT__brProfHit__v1 = 1U;
                    __Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v1 
                        = (0xffU & vlTOPp->Soc__DOT__CPU__DOT__stats_slot);
                }
            }
        }
    }
    if (vlTOPp->RESET) {
        if ((1U & (((IData)(vlTOPp->Soc__DOT__UART__DOT__cnt) 
                    >> 7U) & (~ (IData)((0U != (IData)(vlTOPp->Soc__DOT__UART__DOT__data))))))) {
            __Vdly__Soc__DOT__uart_ready = 1U;
        } else {
            if (((IData)(vlTOPp->Soc__DOT__uart_valid) 
                 & (IData)(vlTOPp->Soc__DOT__uart_ready))) {
                __Vdly__Soc__DOT__uart_ready = 0U;
            }
        }
        __Vdly__Soc__DOT__UART__DOT__cnt = ((1U & ((IData)(vlTOPp->Soc__DOT__uart_ready) 
                                                   | ((IData)(vlTOPp->Soc__DOT__UART__DOT__cnt) 
                                                      >> 7U)))
                                             ? 0x56U
                                             : (0xffU 
                                                & ((IData)(vlTOPp->Soc__DOT__UART__DOT__cnt) 
                                                   - (IData)(1U))));
        if ((0x80U & (IData)(vlTOPp->Soc__DOT__UART__DOT__cnt))) {
            vlTOPp->Soc__DOT__UART__DOT__data = (0x1ffU 
                                                 & ((IData)(vlTOPp->Soc__DOT__UART__DOT__data) 
                                                    >> 1U));
        } else {
            if (((IData)(vlTOPp->Soc__DOT__uart_valid) 
                 & (IData)(vlTOPp->Soc__DOT__uart_ready))) {
                vlTOPp->Soc__DOT__UART__DOT__data = 
                    (0x200U | (0x1feU & (vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                                         << 1U)));
            }
        }
    } else {
        __Vdly__Soc__DOT__uart_ready = 1U;
        __Vdly__Soc__DOT__UART__DOT__cnt = 0x56U;
        vlTOPp->Soc__DOT__UART__DOT__data = 0U;
    }
    if (((IData)(vlTOPp->Soc__DOT__IO_mem_wr) & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                                 >> 2U))) {
        vlTOPp->LEDS = (0x1fU & vlTOPp->Soc__DOT__CPU__DOT__EM_rs2);
    }
    if (vlTOPp->RESET) {
        if (VL_UNLIKELY(((IData)(vlTOPp->Soc__DOT__IO_mem_wr) 
                         & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                            >> 8U)))) {
            VL_WRITEF("COREMARK_TICKS=%0#\n",32,vlTOPp->Soc__DOT__MBOX0);
            VL_WRITEF("COREMARK_MHZ_X1000=%0#\n",32,
                      vlTOPp->Soc__DOT__MBOX1);
            VL_WRITEF("COREMARK_CPI_X1000=%0#\n",32,
                      vlTOPp->Soc__DOT__MBOX2);
            VL_FINISH_MT("src/Soc.v", 261, "");
        }
        if (((IData)(vlTOPp->Soc__DOT__IO_mem_wr) & 
             (vlTOPp->Soc__DOT__CPU__DOT__EM_addr >> 5U))) {
            vlTOPp->Soc__DOT__MBOX0 = vlTOPp->Soc__DOT__CPU__DOT__EM_rs2;
        }
        if (((IData)(vlTOPp->Soc__DOT__IO_mem_wr) & 
             (vlTOPp->Soc__DOT__CPU__DOT__EM_addr >> 6U))) {
            vlTOPp->Soc__DOT__MBOX1 = vlTOPp->Soc__DOT__CPU__DOT__EM_rs2;
        }
        if (((IData)(vlTOPp->Soc__DOT__IO_mem_wr) & 
             (vlTOPp->Soc__DOT__CPU__DOT__EM_addr >> 7U))) {
            vlTOPp->Soc__DOT__MBOX2 = vlTOPp->Soc__DOT__CPU__DOT__EM_rs2;
        }
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch = vlTOPp->Soc__DOT__CPU__DOT__D_isBranch;
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_IorSimm = ((8U 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)))
                                                   ? 
                                                  ((0xfffff800U 
                                                    & ((- (IData)(
                                                                  (1U 
                                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                                      >> 0x1fU)))) 
                                                       << 0xbU)) 
                                                   | ((0x7e0U 
                                                       & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 0x14U)) 
                                                      | (0x1fU 
                                                         & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 7U))))
                                                   : 
                                                  ((0xfffff800U 
                                                    & ((- (IData)(
                                                                  (1U 
                                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                                      >> 0x1fU)))) 
                                                       << 0xbU)) 
                                                   | (0x7ffU 
                                                      & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 0x14U))));
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r) {
            if (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divstep_do) {
                __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                    = (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                       - (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r));
                __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r 
                    = (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r 
                       | vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r);
            }
            vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                = (0x7fffffffffffffffULL & (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                                            >> 1U));
            __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
                = (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
                   >> 1U);
            vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r 
                = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                   & (~ vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r));
        } else {
            if (((IData)(vlTOPp->Soc__DOT__CPU__DOT____Vcellinp__DIVU__start) 
                 & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r)))) {
                __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r 
                    = ((1U & ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                              & (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                 >> 0x1fU))) ? (- vlTOPp->Soc__DOT__CPU__DOT__E_rs1)
                        : vlTOPp->Soc__DOT__CPU__DOT__E_rs1);
                __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r = 0U;
                __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = 0x80000000U;
                vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r = 1U;
                vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r 
                    = (0x7fffffffffffffffULL & ((QData)((IData)(
                                                                ((1U 
                                                                  & ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                                     & (vlTOPp->Soc__DOT__CPU__DOT__E_rs2 
                                                                        >> 0x1fU)))
                                                                  ? 
                                                                 (- vlTOPp->Soc__DOT__CPU__DOT__E_rs2)
                                                                  : vlTOPp->Soc__DOT__CPU__DOT__E_rs2))) 
                                                << 0x1fU));
                vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__sign_r 
                    = (1U & ((2U & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3))
                              ? ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                 & (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                    >> 0x1fU)) : ((
                                                   (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                   & ((1U 
                                                       & (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                          >> 0x1fU)) 
                                                      != 
                                                      (1U 
                                                       & (vlTOPp->Soc__DOT__CPU__DOT__E_rs2 
                                                          >> 0x1fU)))) 
                                                  & (0U 
                                                     != vlTOPp->Soc__DOT__CPU__DOT__E_rs2))));
            } else {
                vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r = 0U;
            }
        }
        vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r 
            = (1U & vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r);
    } else {
        __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r = 0U;
        __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r = 0ULL;
        __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__sign_r = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id = (0x1fU 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0x14U));
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) {
            __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0;
            __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2;
        } else {
            if ((1U & (((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)) 
                        & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC))) 
                       & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))))) {
                if (((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                      >> 3U) & (1U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                >> 7U))))) {
                    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1;
                    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                        = ((IData)(4U) + vlTOPp->Soc__DOT__CPU__DOT__FD_PC);
                }
                if ((((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isJALR) 
                      & (0U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                         >> 7U)))) 
                     & ((1U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                         >> 0xfU))) 
                        | (5U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                           >> 0xfU)))))) {
                    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1;
                    __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3;
                }
            }
        }
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJAL) 
                 & (1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId)))) {
                __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2;
                __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0;
            }
            if ((((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR) 
                  & (0U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId))) 
                 & ((1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)) 
                    | (5U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id))))) {
                __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2;
            }
        }
    } else {
        __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 = 0U;
        __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 = 0U;
        __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 = 0U;
        __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3 = 0U;
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable) 
         & (0U != (IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_rdId)))) {
        __Vdlyvval__Soc__DOT__CPU__DOT__RF__DOT__regs__v0 
            = vlTOPp->Soc__DOT__CPU__DOT__MW_wbData;
        __Vdlyvset__Soc__DOT__CPU__DOT__RF__DOT__regs__v0 = 1U;
        __Vdlyvdim0__Soc__DOT__CPU__DOT__RF__DOT__regs__v0 
            = vlTOPp->Soc__DOT__CPU__DOT__MW_rdId;
    }
    vlTOPp->Soc__DOT__AXI_DDR__DOT__latency_cnt = __Vdly__Soc__DOT__AXI_DDR__DOT__latency_cnt;
    vlTOPp->Soc__DOT__AXI_DDR__DOT__current_addr = __Vdly__Soc__DOT__AXI_DDR__DOT__current_addr;
    if (__Vdlyvset__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__BHT[__Vdlyvdim0__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__BP__DOT__BHT__v0;
    }
    vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 
        = __Vdly__Soc__DOT__CPU__DOT__nbPHTUniquePcOver1;
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfValid[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfValid__v0] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC2__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtPC2[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC2__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtPC2__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC3__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtPC3[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC3__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtPC3__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtAliasCount__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtAliasCount__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtAliasCount__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfPC[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfPC__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__brProfPC__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC0__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtPC0[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC0__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtPC0__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC1__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtPC1[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtPC1__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtPC1__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtUseCount__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUseCount__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__phtUseCount__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v0] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfTotal__v1) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfTotal__v1] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__brProfTotal__v1;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfValid__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfHit[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v0;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__brProfHit__v1) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfHit[__Vdlyvdim0__Soc__DOT__CPU__DOT__brProfHit__v1] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__brProfHit__v1;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC0__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v0] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC1__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v1] = 2U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC2__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v2] = 3U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtPC3__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v3] = 4U;
    }
    if (__Vdlyvset__Soc__DOT__CPU__DOT__phtUniqueCount__v4) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[__Vdlyvdim0__Soc__DOT__CPU__DOT__phtUniqueCount__v4] = 5U;
    }
    vlTOPp->Soc__DOT__UART__DOT__cnt = __Vdly__Soc__DOT__UART__DOT__cnt;
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r 
        = __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r;
    if (__Vdlyvset__Soc__DOT__CPU__DOT__RF__DOT__regs__v0) {
        vlTOPp->Soc__DOT__CPU__DOT__RF__DOT__regs[__Vdlyvdim0__Soc__DOT__CPU__DOT__RF__DOT__regs__v0] 
            = __Vdlyvval__Soc__DOT__CPU__DOT__RF__DOT__regs__v0;
    }
    vlTOPp->Soc__DOT__CPU__DOT__RF__DOT__regs[0U] = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__MW_nop = (1U & ((~ (IData)(vlTOPp->RESET)) 
                                                | (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_nop)));
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isStore = (8U 
                                                  == 
                                                  (0x1fU 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 2U)));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isStore = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__MW_instr = ((IData)(vlTOPp->RESET)
                                             ? vlTOPp->Soc__DOT__CPU__DOT__EM_instr
                                             : 0x33U);
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_predictRA = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_BHTindex = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__bht_index;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch 
            = vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
            vlTOPp->Soc__DOT__CPU__DOT__DE_PC = vlTOPp->Soc__DOT__CPU__DOT__FD_PC;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__DE_PC = 0U;
    }
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->Soc__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->Soc__DOT__UART__DOT__data))))));
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                    = vlTOPp->Soc__DOT__CPU__DOT__E_rs2;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) {
            vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
            vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3 
                = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
        } else {
            if ((1U & (((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)) 
                        & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC))) 
                       & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))))) {
                if (((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                      >> 3U) & (1U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                >> 7U))))) {
                    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
                    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
                }
                if ((((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isJALR) 
                      & (0U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                         >> 7U)))) 
                     & ((1U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                         >> 0xfU))) 
                        | (5U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                           >> 0xfU)))))) {
                    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1 
                        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
                }
            }
        }
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJAL) 
                 & (1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId)))) {
                vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
                vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_PCplus4orUimm;
            }
            if ((((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR) 
                  & (0U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId))) 
                 & ((1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)) 
                    | (5U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id))))) {
                vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
                vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2 
                    = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1 = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3 = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0 = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2 = 0U;
    }
    if (vlTOPp->RESET) {
        vlTOPp->Soc__DOT__CPU__DOT__MW_rdId = vlTOPp->Soc__DOT__CPU__DOT__EM_rdId;
        vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable) 
                                                   & 1U);
        vlTOPp->Soc__DOT__CPU__DOT__MW_wbData = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_isLoad)
                                                  ? 
                                                 ((0x400000U 
                                                   & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                   ? 
                                                  ((0x10U 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                    ? 
                                                   (0x200U 
                                                    & ((~ (IData)(vlTOPp->Soc__DOT__uart_ready)) 
                                                       << 9U))
                                                    : 0U)
                                                   : 
                                                  ((0U 
                                                    == 
                                                    (3U 
                                                     & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                                    ? 
                                                   ((0xffffff00U 
                                                     & ((- (IData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign))) 
                                                        << 8U)) 
                                                    | (IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b))
                                                    : 
                                                   ((1U 
                                                     == 
                                                     (3U 
                                                      & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                                     ? 
                                                    ((0xffff0000U 
                                                      & ((- (IData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign))) 
                                                         << 0x10U)) 
                                                     | (IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h))
                                                     : vlTOPp->Soc__DOT__data_rdata)))
                                                  : 
                                                 ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_isCSRRS)
                                                   ? 
                                                  (((((1U 
                                                       & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is))
                                                       ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle)
                                                       : 0U) 
                                                     | ((4U 
                                                         & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is))
                                                         ? (IData)(
                                                                   (vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle 
                                                                    >> 0x20U))
                                                         : 0U)) 
                                                    | ((2U 
                                                        & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is))
                                                        ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret)
                                                        : 0U)) 
                                                   | ((8U 
                                                       & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is))
                                                       ? (IData)(
                                                                 (vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret 
                                                                  >> 0x20U))
                                                       : 0U))
                                                   : vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult));
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__MW_rdId = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__MW_wbData = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 
        = __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1;
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3 
        = __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3;
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 
        = __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2;
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 
        = __Vdly__Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0;
    vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__cycle = __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__cycle;
    vlTOPp->Soc__DOT__CPU__DOT__CSR__DOT__instret = __Vdly__Soc__DOT__CPU__DOT__CSR__DOT__instret;
    vlTOPp->Soc__DOT__uart_ready = __Vdly__Soc__DOT__uart_ready;
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_nop = 1U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_nop 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_nop;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_nop = 1U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_instr = vlTOPp->Soc__DOT__CPU__DOT__DE_instr;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_instr = 0x33U;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_instr = 0x33U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->Soc__DOT__CPU__DOT__EM_rs2));
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR = vlTOPp->Soc__DOT__CPU__DOT__D_isJALR;
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJAL = (1U 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 3U));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJAL = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id = (0x1fU 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0xfU));
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_rdId 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_rdId;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_rdId = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable 
                    = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_wbEnable) 
                       & (0U != (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId)));
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable = 0U;
    }
    vlTOPp->Soc__DOT__data_rdata = vlTOPp->Soc__DOT__DATARAM__DOT__mem
        [(0x3fffU & (vlTOPp->Soc__DOT__CPU__DOT__E_addr 
                     >> 2U))];
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                    = vlTOPp->Soc__DOT__CPU__DOT__E_addr;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_addr = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is 
                    = (0xfU & ((IData)(1U) << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_csrId)));
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_csrId_is = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_funct3 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_funct3;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_funct3 = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_isLoad = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_isLoad 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_isLoad;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_isLoad = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__aluBusy) {
            vlTOPp->Soc__DOT__CPU__DOT__EM_isCSRRS = 0U;
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_isCSRRS 
                    = vlTOPp->Soc__DOT__CPU__DOT__DE_isCSRRS;
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_isCSRRS = 0U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)))) {
                vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult 
                    = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC)
                        ? vlTOPp->Soc__DOT__CPU__DOT__DE_PCplus4orUimm
                        : ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isRV32M)
                            ? ((((1U & ((IData)(1U) 
                                        << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                  ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply)
                                  : 0U) | ((0U != (7U 
                                                   & (((IData)(1U) 
                                                       << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                      >> 1U)))
                                            ? (IData)(
                                                      (vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply 
                                                       >> 0x20U))
                                            : 0U)) 
                               | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV)
                                   ? ((4U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divsel))
                                       ? vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r
                                       : ((5U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divsel))
                                           ? (- vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r)
                                           : ((6U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divsel))
                                               ? vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r
                                               : (- vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r))))
                                   : 0U)) : (((((((
                                                   ((1U 
                                                     & ((IData)(1U) 
                                                        << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                     ? 
                                                    (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct7) 
                                                      & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isALUreg))
                                                      ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus)
                                                      : vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus)
                                                     : 0U) 
                                                   | ((2U 
                                                       & ((IData)(1U) 
                                                          << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                       ? vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__leftshift
                                                       : 0U)) 
                                                  | ((4U 
                                                      & ((IData)(1U) 
                                                         << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                      ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_LT)
                                                      : 0U)) 
                                                 | ((8U 
                                                     & ((IData)(1U) 
                                                        << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                     ? 
                                                    (1U 
                                                     & (IData)(
                                                               (vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                >> 0x20U)))
                                                     : 0U)) 
                                                | ((0x10U 
                                                    & ((IData)(1U) 
                                                       << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                    ? 
                                                   (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                    ^ vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2)
                                                    : 0U)) 
                                               | ((0x20U 
                                                   & ((IData)(1U) 
                                                      << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                   ? vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__shifter
                                                   : 0U)) 
                                              | ((0x40U 
                                                  & ((IData)(1U) 
                                                     << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                  ? 
                                                 (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                  | vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2)
                                                  : 0U)) 
                                             | ((0x80U 
                                                 & ((IData)(1U) 
                                                    << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)))
                                                 ? 
                                                (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2)
                                                 : 0U))));
            }
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult = 0U;
    }
    if (__Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v0) {
        vlTOPp->Soc__DOT__DATARAM__DOT__mem[__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v0] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v0))) 
                & vlTOPp->Soc__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v0]) 
               | ((IData)(__Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v0) 
                  << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v0)));
    }
    if (__Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v1) {
        vlTOPp->Soc__DOT__DATARAM__DOT__mem[__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v1] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v1))) 
                & vlTOPp->Soc__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v1]) 
               | ((IData)(__Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v1) 
                  << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v1)));
    }
    if (__Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v2) {
        vlTOPp->Soc__DOT__DATARAM__DOT__mem[__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v2] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v2))) 
                & vlTOPp->Soc__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v2]) 
               | ((IData)(__Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v2) 
                  << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v2)));
    }
    if (__Vdlyvset__Soc__DOT__DATARAM__DOT__mem__v3) {
        vlTOPp->Soc__DOT__DATARAM__DOT__mem[__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v3] 
            = (((~ ((IData)(0xffU) << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v3))) 
                & vlTOPp->Soc__DOT__DATARAM__DOT__mem
                [__Vdlyvdim0__Soc__DOT__DATARAM__DOT__mem__v3]) 
               | ((IData)(__Vdlyvval__Soc__DOT__DATARAM__DOT__mem__v3) 
                  << (IData)(__Vdlyvlsb__Soc__DOT__DATARAM__DOT__mem__v3)));
    }
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r 
        = __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r;
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r 
        = __Vdly__Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r;
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_nop = 0U;
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_nop = 1U;
    }
    if (vlTOPp->RESET) {
        if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
            vlTOPp->Soc__DOT__CPU__DOT__DE_instr = 
                ((IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop)
                  ? 0x33U : vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg);
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__E_flush) {
            vlTOPp->Soc__DOT__CPU__DOT__DE_instr = 0x33U;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__DE_instr = 0x33U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_rdId = (0x1fU 
                                               & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                  >> 7U));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_wbEnable = (8U 
                                                   != 
                                                   (0xfU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_wbEnable = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data = ((0xffff00ffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff00U 
                                                   & (((1U 
                                                        & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->Soc__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       (vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                                                        >> 8U)) 
                                                      << 8U)));
    vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data = ((0xff00ffffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff0000U 
                                                   & (((2U 
                                                        & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->Soc__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       (vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                                                        >> 0x10U)) 
                                                      << 0x10U)));
    vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data = ((0xffffffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xff000000U 
                                                   & (((1U 
                                                        & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                        ? vlTOPp->Soc__DOT__CPU__DOT__EM_rs2
                                                        : 
                                                       ((2U 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                         ? 
                                                        (vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                                                         >> 8U)
                                                         : 
                                                        (vlTOPp->Soc__DOT__CPU__DOT__EM_rs2 
                                                         >> 0x18U))) 
                                                      << 0x18U)));
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h 
        = (0xffffU & ((2U & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                       ? (vlTOPp->Soc__DOT__data_rdata 
                          >> 0x10U) : vlTOPp->Soc__DOT__data_rdata));
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_csrId = ((2U 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x1aU)) 
                                                | (1U 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 0x15U)));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isLoad = (0U 
                                                 == 
                                                 (0x1fU 
                                                  & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 2U)));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isLoad = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isCSRRS = ((0x1cU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U))) 
                                                  & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 0xdU));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isCSRRS = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divstep_do 
        = (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r 
           <= (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r)));
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_funct7 = (1U 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x1eU));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_funct3 = (7U 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0xcU));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC 
            = (1U & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                     >> 2U));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isRV32M = ((0xcU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U))) 
                                                  & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 0x19U));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isRV32M = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV = (((0xcU 
                                                  == 
                                                  (0x1fU 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 2U))) 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x19U)) 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0xeU));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV = 0U;
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_PCplus4orUimm 
            = (((- (IData)((1U != (3U & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                         >> 5U))))) 
                & vlTOPp->Soc__DOT__CPU__DOT__FD_PC) 
               + ((1U & ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                          >> 2U) & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                    >> 6U))) ? 4U : 
                  (0xfffff000U & vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg)));
    }
    if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isALUreg = (0xcU 
                                                   == 
                                                   (0x1fU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)));
    }
    if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_flush) 
         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) {
        vlTOPp->Soc__DOT__CPU__DOT__DE_isALUreg = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__E_rs1 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)))
                                          ? vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)))
                                              ? vlTOPp->Soc__DOT__CPU__DOT__MW_wbData
                                              : ((0U 
                                                  == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id))
                                                  ? 0U
                                                  : 
                                                 vlTOPp->Soc__DOT__CPU__DOT__RF__DOT__regs
                                                 [vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id])));
    vlTOPp->Soc__DOT__CPU__DOT__E_rs2 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id)))
                                          ? vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id)))
                                              ? vlTOPp->Soc__DOT__CPU__DOT__MW_wbData
                                              : ((0U 
                                                  == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id))
                                                  ? 0U
                                                  : 
                                                 vlTOPp->Soc__DOT__CPU__DOT__RF__DOT__regs
                                                 [vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id])));
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b 
        = (0xffU & ((1U & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                     ? ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                        >> 8U) : (IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h)));
    vlTOPp->Soc__DOT__CPU__DOT__E_addr = (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                          + vlTOPp->Soc__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign 
        = (1U & ((~ ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3) 
                     >> 2U)) & ((0U == (3U & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                 ? ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b) 
                                    >> 7U) : ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                                              >> 0xfU))));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
        = vlTOPp->Soc__DOT__CPU__DOT__E_rs1;
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__shifter_in 
        = ((1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3))
            ? vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout
            : vlTOPp->Soc__DOT__CPU__DOT__E_rs1);
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divsel = 
        (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
          << 2U) | ((2U & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                    | (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__sign_r)));
    vlTOPp->Soc__DOT__CPU__DOT__aluBusy = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                                           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
                                              & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r))));
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC) 
             & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
            __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
            __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__F_redirect) {
            __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) {
            __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
            __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
        }
        if (((IData)(vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r) 
             & (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_pendingValid))) {
            if ((((IData)(vlTOPp->Soc__DOT__CPU__DOT__F_pendingEpoch) 
                  != (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_epochNext)) 
                 | (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC))) {
                if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
                    __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
                }
            } else {
                if ((1U & ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)) 
                           | (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop)))) {
                    vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                        = vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC;
                    vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                        = vlTOPp->Soc__DOT__ICACHE__DOT__resp_data_r;
                    __Vdly__Soc__DOT__CPU__DOT__FD_nop = 0U;
                } else {
                    __Vdly__Soc__DOT__CPU__DOT__FB_PC 
                        = vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC;
                    __Vdly__Soc__DOT__CPU__DOT__FB_instr 
                        = vlTOPp->Soc__DOT__ICACHE__DOT__resp_data_r;
                    __Vdly__Soc__DOT__CPU__DOT__FB_epoch 
                        = vlTOPp->Soc__DOT__CPU__DOT__F_pendingEpoch;
                    __Vdly__Soc__DOT__CPU__DOT__FB_valid = 1U;
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall)))) {
                if (((IData)(vlTOPp->Soc__DOT__CPU__DOT__FB_valid) 
                     & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__FB_epoch) 
                        == (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_epochNext)))) {
                    vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                        = vlTOPp->Soc__DOT__CPU__DOT__FB_PC;
                    vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                        = vlTOPp->Soc__DOT__CPU__DOT__FB_instr;
                    __Vdly__Soc__DOT__CPU__DOT__FD_nop = 0U;
                    __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
                } else {
                    if (vlTOPp->Soc__DOT__CPU__DOT__FB_valid) {
                        __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
                        __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
                    } else {
                        __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
                    }
                }
            }
        }
    } else {
        __Vdly__Soc__DOT__CPU__DOT__FB_valid = 0U;
        __Vdly__Soc__DOT__CPU__DOT__FD_nop = 1U;
        __Vdly__Soc__DOT__CPU__DOT__FB_PC = 0U;
        __Vdly__Soc__DOT__CPU__DOT__FB_instr = 0x33U;
        __Vdly__Soc__DOT__CPU__DOT__FB_epoch = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg = 0x33U;
        vlTOPp->Soc__DOT__CPU__DOT__FD_PC = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isALUreg) 
                                             | (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch))
                                             ? vlTOPp->Soc__DOT__CPU__DOT__E_rs2
                                             : vlTOPp->Soc__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__gshare_commit 
        = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch) 
           & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy)));
    vlTOPp->Soc__DOT__CPU__DOT__FB_PC = __Vdly__Soc__DOT__CPU__DOT__FB_PC;
    vlTOPp->Soc__DOT__CPU__DOT__FB_instr = __Vdly__Soc__DOT__CPU__DOT__FB_instr;
    vlTOPp->Soc__DOT__CPU__DOT__FB_epoch = __Vdly__Soc__DOT__CPU__DOT__FB_epoch;
    vlTOPp->Soc__DOT__CPU__DOT__FB_valid = __Vdly__Soc__DOT__CPU__DOT__FB_valid;
    vlTOPp->Soc__DOT__CPU__DOT__FD_nop = __Vdly__Soc__DOT__CPU__DOT__FD_nop;
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply 
        = VL_MULS_QQQ(64,64,64, VL_EXTENDS_QQ(64,33, 
                                              (((QData)((IData)(
                                                                (1U 
                                                                 & ((vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                                     >> 0x1fU) 
                                                                    & (((IData)(1U) 
                                                                        << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                                       >> 1U))))) 
                                                << 0x20U) 
                                               | (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_rs1)))), 
                      VL_EXTENDS_QQ(64,33, (((QData)((IData)(
                                                             (1U 
                                                              & ((vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2 
                                                                  >> 0x1fU) 
                                                                 & ((0x7fU 
                                                                     & (((IData)(1U) 
                                                                         << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                                        >> 1U)) 
                                                                    | (0x3fU 
                                                                       & (((IData)(1U) 
                                                                           << (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                                                                          >> 2U))))))) 
                                             << 0x20U) 
                                            | (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2)))));
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__shifter = (IData)(
                                                            (0x1ffffffffULL 
                                                             & VL_SHIFTRS_QQI(33,33,5, 
                                                                              (((QData)((IData)(
                                                                                ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct7) 
                                                                                & (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                                                >> 0x1fU)))) 
                                                                                << 0x20U) 
                                                                               | (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__shifter_in))), 
                                                                              (0x1fU 
                                                                               & vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2))));
    vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus = (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                             + vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2);
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus 
        = (0x1ffffffffULL & (1ULL + ((0x100000000ULL 
                                      | (QData)((IData)(
                                                        (~ vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2)))) 
                                     + (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_rs1)))));
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__F_reqFire) {
            vlTOPp->Soc__DOT__CPU__DOT__F_pendingEpoch 
                = vlTOPp->Soc__DOT__CPU__DOT__F_epochNext;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__F_pendingEpoch = 0U;
    }
    if (vlTOPp->RESET) {
        if (vlTOPp->Soc__DOT__CPU__DOT__F_reqFire) {
            vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC 
                = vlTOPp->Soc__DOT__CPU__DOT__F_PC;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC = 0U;
    }
    if (vlTOPp->RESET) {
        if (((IData)(vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r) 
             & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_reqFire)))) {
            vlTOPp->Soc__DOT__CPU__DOT__F_pendingValid = 0U;
        }
        if (vlTOPp->Soc__DOT__CPU__DOT__F_reqFire) {
            vlTOPp->Soc__DOT__CPU__DOT__F_pendingValid = 1U;
        }
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__F_pendingValid = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__D_isJALR = (5U == (
                                                   (4U 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 4U)) 
                                                   | (3U 
                                                      & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 2U))));
    vlTOPp->Soc__DOT__CPU__DOT__D_isBranch = (4U == 
                                              ((4U 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 4U)) 
                                               | ((2U 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 3U)) 
                                                  | (1U 
                                                     & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                        >> 2U)))));
    vlTOPp->Soc__DOT__CPU__DOT__dataHazard = ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop)) 
                                              & (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isLoad) 
                                                  | (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isCSRRS)) 
                                                 & ((((~ 
                                                       ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 3U) 
                                                        | ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 4U) 
                                                           & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                              >> 6U)))) 
                                                      & ((0x1fU 
                                                          & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                             >> 0xfU)) 
                                                         == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId))) 
                                                     & (0U 
                                                        != 
                                                        (0x1fU 
                                                         & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                            >> 0xfU)))) 
                                                    | ((((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 5U) 
                                                         & (0U 
                                                            == 
                                                            (3U 
                                                             & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                                >> 2U)))) 
                                                        & ((0x1fU 
                                                            & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                               >> 0x14U)) 
                                                           == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId))) 
                                                       & (0U 
                                                          != 
                                                          (0x1fU 
                                                           & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                              >> 0x14U)))))));
    vlTOPp->Soc__DOT__CPU__DOT__D_Bimm = ((0xfffff000U 
                                           & ((- (IData)(
                                                         (1U 
                                                          & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                             >> 0x1fU)))) 
                                              << 0xcU)) 
                                          | ((0x800U 
                                              & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                 << 4U)) 
                                             | ((0x7e0U 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 0x14U)) 
                                                | (0x1eU 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                      >> 7U)))));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history;
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xffeU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 1U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xffdU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 1U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 2U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xffbU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 2U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 3U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xff7U & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 3U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 4U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xfefU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 4U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 5U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xfdfU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 5U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 6U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xfbfU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 6U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 7U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xf7fU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 7U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 8U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xeffU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 8U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 9U));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xdffU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 9U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 0xaU));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0xbffU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 0xaU));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 
        = (1U & ((IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history) 
                 >> 0xbU));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout 
        = ((0x7ffU & (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)) 
           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1) 
              << 0xbU));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__bht_index 
        = (0xfffU & ((vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                      >> 2U) ^ (IData)(vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout)));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
        = vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__shifter;
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__Vfuncout 
        = ((0x80000000U & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                           << 0x1fU)) | ((0x40000000U 
                                          & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                             << 0x1dU)) 
                                         | ((0x20000000U 
                                             & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                << 0x1bU)) 
                                            | ((0x10000000U 
                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                   << 0x19U)) 
                                               | ((0x8000000U 
                                                   & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                      << 0x17U)) 
                                                  | ((0x4000000U 
                                                      & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                         << 0x15U)) 
                                                     | ((0x2000000U 
                                                         & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                            << 0x13U)) 
                                                        | ((0x1000000U 
                                                            & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                               << 0x11U)) 
                                                           | ((0x800000U 
                                                               & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                  << 0xfU)) 
                                                              | ((0x400000U 
                                                                  & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                     << 0xdU)) 
                                                                 | ((0x200000U 
                                                                     & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                        << 0xbU)) 
                                                                    | ((0x100000U 
                                                                        & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                           << 9U)) 
                                                                       | ((0x80000U 
                                                                           & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                              << 7U)) 
                                                                          | ((0x40000U 
                                                                              & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                << 5U)) 
                                                                             | ((0x20000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                << 3U)) 
                                                                                | ((0x10000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                << 1U)) 
                                                                                | ((0x8000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 1U)) 
                                                                                | ((0x4000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 3U)) 
                                                                                | ((0x2000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 5U)) 
                                                                                | ((0x1000U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 7U)) 
                                                                                | ((0x800U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 9U)) 
                                                                                | ((0x400U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0xbU)) 
                                                                                | ((0x200U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0xdU)) 
                                                                                | ((0x100U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0xfU)) 
                                                                                | ((0x80U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x11U)) 
                                                                                | ((0x40U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x13U)) 
                                                                                | ((0x20U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x15U)) 
                                                                                | ((0x10U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x17U)) 
                                                                                | ((8U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x19U)) 
                                                                                | ((4U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x1bU)) 
                                                                                | ((2U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x1dU)) 
                                                                                | (1U 
                                                                                & (vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x 
                                                                                >> 0x1fU)))))))))))))))))))))))))))))))));
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__leftshift 
        = vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__Vfuncout;
    vlTOPp->Soc__DOT__CPU__DOT__E_PCcorrection = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch)
                                                   ? vlTOPp->Soc__DOT__CPU__DOT__DE_PCplus4orBimm
                                                   : 
                                                  (0xfffffffeU 
                                                   & vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus));
    vlTOPp->Soc__DOT__CPU__DOT__E_LT = (1U & ((0x80000000U 
                                               & (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                  ^ vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2))
                                               ? (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                                  >> 0x1fU)
                                               : (IData)(
                                                         (vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                          >> 0x20U))));
    vlTOPp->Soc__DOT__CPU__DOT____Vcellinp__DIVU__start 
        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
            & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard))) 
           & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r)));
    vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction = ((1U 
                                                   == 
                                                   (3U 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 2U)))
                                                   ? vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0
                                                   : 
                                                  (vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                                                   + 
                                                   ((8U 
                                                     & vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg)
                                                     ? 
                                                    ((0xfff00000U 
                                                      & ((- (IData)(
                                                                    (1U 
                                                                     & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                                        >> 0x1fU)))) 
                                                         << 0x14U)) 
                                                     | ((0xff000U 
                                                         & vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg) 
                                                        | ((0x800U 
                                                            & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                               >> 9U)) 
                                                           | (0x7feU 
                                                              & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                                 >> 0x14U)))))
                                                     : vlTOPp->Soc__DOT__CPU__DOT__D_Bimm)));
    vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch = (1U 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__BHT
                                                      [vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__bht_index] 
                                                      >> 1U));
    vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch) 
                                                & ((1U 
                                                    & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is))
                                                    ? 
                                                   (0U 
                                                    == (IData)(vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is))
                                                     ? 
                                                    (0U 
                                                     != (IData)(vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus))
                                                     : 
                                                    ((0x10U 
                                                      & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is))
                                                      ? (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_LT)
                                                      : 
                                                     ((0x20U 
                                                       & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is))
                                                       ? 
                                                      (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_LT))
                                                       : 
                                                      ((0x40U 
                                                        & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is))
                                                        ? (IData)(
                                                                  (vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                   >> 0x20U))
                                                        : 
                                                       (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3_is) 
                                                         >> 7U) 
                                                        & (~ (IData)(
                                                                     (vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                      >> 0x20U))))))))));
    if (vlTOPp->RESET) {
        vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r = 0U;
        if (((0U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
             & (IData)(vlTOPp->Soc__DOT__inst_en))) {
            if (vlTOPp->Soc__DOT__ICACHE__DOT__is_hit) {
                vlTOPp->Soc__DOT__ICACHE__DOT__hit_count 
                    = ((IData)(1U) + vlTOPp->Soc__DOT__ICACHE__DOT__hit_count);
            } else {
                if (vlTOPp->Soc__DOT__ICACHE__DOT__is_miss) {
                    vlTOPp->Soc__DOT__ICACHE__DOT__miss_count 
                        = ((IData)(1U) + vlTOPp->Soc__DOT__ICACHE__DOT__miss_count);
                }
            }
        }
        if ((4U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state))) {
            __Vdly__Soc__DOT__ICACHE__DOT__state = 0U;
        } else {
            if ((2U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state))) {
                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state))) {
                    if (vlTOPp->Soc__DOT__ICACHE__DOT__replace_way) {
                        __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0 = 1U;
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way1__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way1__v0 
                            = (0xfffffU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                           >> 0xcU));
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way1__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[0U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[0U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[1U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[1U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[2U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[2U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[3U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[3U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[4U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[4U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[5U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[5U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[6U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[6U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[7U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[7U];
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v2 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                    } else {
                        __Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0 = 1U;
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way0__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way0__v0 
                            = (0xfffffU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                           >> 0xcU));
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way0__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[0U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[0U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[1U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[1U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[2U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[2U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[3U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[3U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[4U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[4U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[5U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[5U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[6U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[6U];
                        __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[7U] 
                            = vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[7U];
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                        __Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v3 
                            = (0x7fU & (vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                                        >> 5U));
                    }
                    vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r = 1U;
                    vlTOPp->Soc__DOT__ICACHE__DOT__resp_data_r 
                        = ((4U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                            ? ((2U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                ? ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                    ? vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[7U]
                                    : vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[6U])
                                : ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                    ? vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[5U]
                                    : vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[4U]))
                            : ((2U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                ? ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                    ? vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[3U]
                                    : vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[2U])
                                : ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset))
                                    ? vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[1U]
                                    : vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[0U])));
                    __Vdly__Soc__DOT__ICACHE__DOT__state = 0U;
                } else {
                    if (((2U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state)) 
                         & (IData)(vlTOPp->Soc__DOT__axi_rready))) {
                        if ((4U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                            if ((2U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[7U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                } else {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[6U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                }
                            } else {
                                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[5U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                } else {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[4U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                }
                            }
                        } else {
                            if ((2U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[3U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                } else {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[2U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                }
                            } else {
                                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt))) {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[1U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                } else {
                                    vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[0U] 
                                        = vlTOPp->Soc__DOT__axi_rdata;
                                }
                            }
                        }
                        if (((2U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state)) 
                             & (0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__burst_left)))) {
                            __Vdly__Soc__DOT__ICACHE__DOT__state = 3U;
                        }
                        vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt 
                            = (7U & ((IData)(1U) + (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt)));
                    }
                }
            } else {
                if ((1U & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state))) {
                    if (((0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state)) 
                         & (IData)(vlTOPp->Soc__DOT__axi_arvalid))) {
                        __Vdly__Soc__DOT__ICACHE__DOT__state = 2U;
                    }
                } else {
                    if (vlTOPp->Soc__DOT__ICACHE__DOT__is_hit) {
                        vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r = 1U;
                        vlTOPp->Soc__DOT__ICACHE__DOT__resp_data_r 
                            = ((0x10U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                ? ((8U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                    ? ((4U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                        ? vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[7U]
                                        : vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[6U])
                                    : ((4U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                        ? vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[5U]
                                        : vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[4U]))
                                : ((8U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                    ? ((4U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                        ? vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[3U]
                                        : vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[2U])
                                    : ((4U & vlTOPp->Soc__DOT__CPU__DOT__F_PC)
                                        ? vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[1U]
                                        : vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[0U])));
                    }
                    if (vlTOPp->Soc__DOT__ICACHE__DOT__is_miss) {
                        vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt = 0U;
                        __Vdly__Soc__DOT__ICACHE__DOT__state = 1U;
                        vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr 
                            = (0xffffffe0U & vlTOPp->Soc__DOT__CPU__DOT__F_PC);
                        vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset 
                            = (7U & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                     >> 2U));
                        vlTOPp->Soc__DOT__ICACHE__DOT__replace_way 
                            = (vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way0
                               [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                          >> 5U))] 
                               & ((~ vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way1
                                   [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                              >> 5U))]) 
                                  | vlTOPp->Soc__DOT__ICACHE__DOT__lru_array
                                  [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                             >> 5U))]));
                    }
                }
            }
        }
    } else {
        vlTOPp->Soc__DOT__ICACHE__DOT__burst_cnt = 0U;
        __Vdly__Soc__DOT__ICACHE__DOT__state = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[0U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[1U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[2U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[3U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[4U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[5U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[6U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__refill_line[7U] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__miss_addr = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__replace_way = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__miss_word_offset = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__resp_data_r = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__resp_valid_r = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__hit_count = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__miss_count = 0U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__D_predictPC = (1U & 
                                               ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop)) 
                                                & ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                    >> 3U) 
                                                   | ((6U 
                                                       == 
                                                       (7U 
                                                        & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                           >> 4U))) 
                                                      & ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                          >> 2U) 
                                                         | (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch))))));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch_next 
        = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__gshare_commit)
            ? (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                << 0xbU) | (0x7ffU & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch) 
                                      >> 1U))) : (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_arch));
    vlTOPp->Soc__DOT__CPU__DOT__E_correctPC = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR) 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__DE_predictRA 
                                                   != 
                                                   (0xfffffffeU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus))) 
                                               | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch) 
                                                  & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                                                     ^ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))));
    vlTOPp->Soc__DOT__AXI_DDR__DOT__burst_left = __Vdly__Soc__DOT__AXI_DDR__DOT__burst_left;
    vlTOPp->Soc__DOT__axi_rdata = vlTOPp->Soc__DOT__AXI_DDR__DOT__memory
        [(0x3ffffU & (vlTOPp->Soc__DOT__AXI_DDR__DOT__current_addr 
                      >> 2U))];
    vlTOPp->Soc__DOT__AXI_DDR__DOT__state = __Vdly__Soc__DOT__AXI_DDR__DOT__state;
    vlTOPp->Soc__DOT__ICACHE__DOT__state = __Vdly__Soc__DOT__ICACHE__DOT__state;
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][0U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[0U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][1U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[1U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][2U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[2U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][3U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[3U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][4U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[4U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][5U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[5U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][6U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[6U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way1__v0][7U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way1__v0[7U];
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__tag_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way1__v0] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way1__v0;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__lru_array[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v0] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__lru_array__v1) {
        vlTOPp->Soc__DOT__ICACHE__DOT__lru_array[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v1] = 0U;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__lru_array[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v2] = 0U;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__lru_array[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__lru_array__v3] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][0U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[0U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][1U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[1U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][2U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[2U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][3U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[3U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][4U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[4U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][5U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[5U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][6U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[6U];
        vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__data_array_way0__v0][7U] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__data_array_way0__v0[7U];
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__tag_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__tag_array_way0__v0] 
            = __Vdlyvval__Soc__DOT__ICACHE__DOT__tag_array_way0__v0;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way1__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way1[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way1__v0] = 1U;
    }
    if (__Vdlyvset__Soc__DOT__ICACHE__DOT__valid_array_way0__v0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way0[__Vdlyvdim0__Soc__DOT__ICACHE__DOT__valid_array_way0__v0] = 1U;
    }
    vlTOPp->Soc__DOT__CPU__DOT__E_flush = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) 
                                           | (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard));
    vlTOPp->Soc__DOT__CPU__DOT__F_redirect = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) 
                                              | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC) 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC 
                                                    != vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction)));
    vlTOPp->Soc__DOT__CPU__DOT__F_PC = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC)
                                         ? vlTOPp->Soc__DOT__CPU__DOT__E_PCcorrection
                                         : ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC)
                                             ? vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction
                                             : vlTOPp->Soc__DOT__CPU__DOT__PC));
    vlTOPp->Soc__DOT__axi_arready = (0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state));
    vlTOPp->Soc__DOT__axi_rvalid = (2U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state));
    vlTOPp->Soc__DOT__axi_arvalid = (1U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state));
    vlTOPp->Soc__DOT__axi_rready = (2U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state));
    vlTOPp->Soc__DOT__CPU__DOT__F_epochNext = (3U & 
                                               ((IData)(vlTOPp->Soc__DOT__CPU__DOT__F_epoch) 
                                                + ((IData)(vlTOPp->Soc__DOT__CPU__DOT__F_redirect)
                                                    ? 1U
                                                    : 0U)));
    vlTOPp->Soc__DOT__ICACHE__DOT__hit_way1_raw = (
                                                   vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way1
                                                   [
                                                   (0x7fU 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                       >> 5U))] 
                                                   & (vlTOPp->Soc__DOT__ICACHE__DOT__tag_array_way1
                                                      [
                                                      (0x7fU 
                                                       & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                          >> 5U))] 
                                                      == 
                                                      (0xfffffU 
                                                       & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                          >> 0xcU))));
    vlTOPp->Soc__DOT__ICACHE__DOT__hit_way0 = (vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way0
                                               [(0x7fU 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                    >> 5U))] 
                                               & (vlTOPp->Soc__DOT__ICACHE__DOT__tag_array_way0
                                                  [
                                                  (0x7fU 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                      >> 5U))] 
                                                  == 
                                                  (0xfffffU 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                                                      >> 0xcU))));
    if (vlTOPp->Soc__DOT__ICACHE__DOT__hit_way0) {
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[0U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][0U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[1U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][1U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[2U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][2U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[3U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][3U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[4U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][4U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[5U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][5U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[6U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][6U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[7U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way0
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][7U];
    } else {
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[0U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][0U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[1U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][1U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[2U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][2U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[3U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][3U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[4U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][4U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[5U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][5U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[6U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][6U];
        vlTOPp->Soc__DOT__ICACHE__DOT__cache_line[7U] 
            = vlTOPp->Soc__DOT__ICACHE__DOT__data_array_way1
            [(0x7fU & (vlTOPp->Soc__DOT__CPU__DOT__F_PC 
                       >> 5U))][7U];
    }
    vlTOPp->Soc__DOT__ICACHE__DOT__cache_hit = ((IData)(vlTOPp->Soc__DOT__ICACHE__DOT__hit_way0) 
                                                | (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__hit_way1_raw));
}

VL_INLINE_OPT void VSoc::_combo__TOP__4(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_combo__TOP__4\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->HALT = ((IData)(vlTOPp->RESET) & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isEBREAK));
    vlTOPp->Soc__DOT__IO_mem_wr = ((IData)(vlTOPp->RESET) 
                                   & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_isStore) 
                                      & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                         >> 0x16U)));
    vlTOPp->Soc__DOT__data_wmask = ((IData)(vlTOPp->RESET)
                                     ? ((- (IData)(
                                                   ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_isStore) 
                                                    & (~ 
                                                       (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                                        >> 0x16U))))) 
                                        & ((0U == (3U 
                                                   & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                            ? ((2U 
                                                & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                ? (
                                                   (1U 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                    ? 8U
                                                    : 4U)
                                                : (
                                                   (1U 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                    ? 2U
                                                    : 1U))
                                            : ((1U 
                                                == 
                                                (3U 
                                                 & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                                ? (
                                                   (2U 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                                                    ? 0xcU
                                                    : 3U)
                                                : 0xfU)))
                                     : 0U);
    vlTOPp->Soc__DOT__CPU__DOT__D_stall = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy) 
                                            | (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard)) 
                                           | (IData)(vlTOPp->HALT));
    vlTOPp->Soc__DOT__uart_valid = ((IData)(vlTOPp->Soc__DOT__IO_mem_wr) 
                                    & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                       >> 3U));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_spec_next 
        = ((((((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isBranch) 
               & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall))) 
              & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC))) 
             & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop))) 
            & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC)))
            ? (((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch) 
                << 0xbU) | (0x7ffU & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history) 
                                      >> 1U))) : (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history));
    vlTOPp->Soc__DOT__inst_en = ((((IData)(vlTOPp->RESET) 
                                   & (~ (IData)(vlTOPp->HALT))) 
                                  & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall))) 
                                 & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FB_valid)));
    vlTOPp->Soc__DOT__CPU__DOT__F_reqFire = ((IData)(vlTOPp->Soc__DOT__inst_en) 
                                             & (0U 
                                                == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)));
    vlTOPp->Soc__DOT__ICACHE__DOT__is_hit = (((0U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
                                              & (IData)(vlTOPp->Soc__DOT__inst_en)) 
                                             & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__cache_hit));
    vlTOPp->Soc__DOT__ICACHE__DOT__is_miss = (((0U 
                                                == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
                                               & (IData)(vlTOPp->Soc__DOT__inst_en)) 
                                              & (~ (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__cache_hit)));
}

void VSoc::_eval(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_eval\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->CLK) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLK)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    vlTOPp->_combo__TOP__4(vlSymsp);
    // Final
    vlTOPp->__Vclklast__TOP__CLK = vlTOPp->CLK;
}

VL_INLINE_OPT QData VSoc::_change_request(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_change_request\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData VSoc::_change_request_1(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_change_request_1\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VSoc::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((CLK & 0xfeU))) {
        Verilated::overWidthError("CLK");}
    if (VL_UNLIKELY((RESET & 0xfeU))) {
        Verilated::overWidthError("RESET");}
    if (VL_UNLIKELY((RXD & 0xfeU))) {
        Verilated::overWidthError("RXD");}
}
#endif  // VL_DEBUG
