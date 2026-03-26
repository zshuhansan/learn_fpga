// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSOC.h for the primary calling header

#include "VSOC.h"
#include "VSOC__Syms.h"

//==========

VL_CTOR_IMP(VSOC) {
    VSOC__Syms* __restrict vlSymsp = __VlSymsp = new VSOC__Syms(this, name());
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VSOC::__Vconfigure(VSOC__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

VSOC::~VSOC() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void VSOC::_initial__TOP__3(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_initial__TOP__3\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp16[3];
    WData/*95:0*/ __Vtemp17[3];
    // Body
    vlTOPp->SOC__DOT__CPU__DOT__nbBranch = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbBranchHit = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbJAL = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbJALR = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbJALRhit = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbLoad = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbStore = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbLoadHazard = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbMUL = 0U;
    vlTOPp->SOC__DOT__CPU__DOT__nbDIV = 0U;
    vlTOPp->SOC__DOT__UART__DOT__cnt = 0U;
    __Vtemp16[0U] = 0x2e686578U;
    __Vtemp16[1U] = 0x4152414dU;
    __Vtemp16[2U] = 0x444154U;
    VL_READMEM_N(true, 32, 16384, 0, VL_CVT_PACK_STR_NW(3, __Vtemp16)
                 , vlTOPp->SOC__DOT__DATARAM__DOT__mem
                 , 0, ~0ULL);
    __Vtemp17[0U] = 0x2e686578U;
    __Vtemp17[1U] = 0x47524f4dU;
    __Vtemp17[2U] = 0x50524fU;
    VL_READMEM_N(true, 32, 16384, 0, VL_CVT_PACK_STR_NW(3, __Vtemp17)
                 , vlTOPp->SOC__DOT__PROGROM__DOT__mem
                 , 0, ~0ULL);
    vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt = 0U;
}

void VSOC::_settle__TOP__4(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_settle__TOP__4\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->SOC__DOT__UART__DOT__data))))));
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divstep_do 
        = (vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r 
           <= (QData)((IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r)));
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
    vlTOPp->SOC__DOT__IO_mem_wr = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_isStore) 
                                   & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                      >> 0x16U));
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h 
        = (0xffffU & ((2U & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                       ? (vlTOPp->SOC__DOT__data_rdata 
                          >> 0x10U) : vlTOPp->SOC__DOT__data_rdata));
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
    vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__divsel = 
        (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
          << 2U) | ((2U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3)) 
                    | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__sign_r)));
    vlTOPp->SOC__DOT__CPU__DOT__aluBusy = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                                           | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
                                              & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r))));
    vlTOPp->SOC__DOT__CPU__DOT__D_isJALR = (5U == (
                                                   (4U 
                                                    & (vlTOPp->SOC__DOT__inst_rdata 
                                                       >> 4U)) 
                                                   | (3U 
                                                      & (vlTOPp->SOC__DOT__inst_rdata 
                                                         >> 2U))));
    vlTOPp->SOC__DOT__CPU__DOT__D_Bimm = ((0xfffff000U 
                                           & ((- (IData)(
                                                         (1U 
                                                          & (vlTOPp->SOC__DOT__inst_rdata 
                                                             >> 0x1fU)))) 
                                              << 0xcU)) 
                                          | ((0x800U 
                                              & (vlTOPp->SOC__DOT__inst_rdata 
                                                 << 4U)) 
                                             | ((0x7e0U 
                                                 & (vlTOPp->SOC__DOT__inst_rdata 
                                                    >> 0x14U)) 
                                                | (0x1eU 
                                                   & (vlTOPp->SOC__DOT__inst_rdata 
                                                      >> 7U)))));
    vlTOPp->SOC__DOT__CPU__DOT__dataHazard = ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                              & ((((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isLoad) 
                                                   | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isCSRRS)) 
                                                  & (((~ 
                                                       ((vlTOPp->SOC__DOT__inst_rdata 
                                                         >> 3U) 
                                                        | ((vlTOPp->SOC__DOT__inst_rdata 
                                                            >> 4U) 
                                                           & (vlTOPp->SOC__DOT__inst_rdata 
                                                              >> 6U)))) 
                                                      & ((0x1fU 
                                                          & (vlTOPp->SOC__DOT__inst_rdata 
                                                             >> 0xfU)) 
                                                         == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))) 
                                                     | (((vlTOPp->SOC__DOT__inst_rdata 
                                                          >> 5U) 
                                                         & (0U 
                                                            == 
                                                            (3U 
                                                             & (vlTOPp->SOC__DOT__inst_rdata 
                                                                >> 2U)))) 
                                                        & ((0x1fU 
                                                            & (vlTOPp->SOC__DOT__inst_rdata 
                                                               >> 0x14U)) 
                                                           == (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_rdId))))) 
                                                 | ((0U 
                                                     == 
                                                     (0x1fU 
                                                      & (vlTOPp->SOC__DOT__inst_rdata 
                                                         >> 2U))) 
                                                    & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isStore))));
    vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__bht_index 
        = (0xfffU & ((vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                      >> 2U) ^ ((IData)(vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__branch_history) 
                                << 3U)));
    vlTOPp->SOC__DOT__resetn = (0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt));
    vlTOPp->SOC__DOT__CPU__DOT__halt = ((0xffffU == (IData)(vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt)) 
                                        & (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isEBREAK));
    vlTOPp->SOC__DOT__uart_valid = ((IData)(vlTOPp->SOC__DOT__IO_mem_wr) 
                                    & (vlTOPp->SOC__DOT__CPU__DOT__EM_addr 
                                       >> 3U));
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b 
        = (0xffU & ((1U & vlTOPp->SOC__DOT__CPU__DOT__EM_addr)
                     ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                        >> 8U) : (IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h)));
    vlTOPp->SOC__DOT__CPU__DOT__E_addr = (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                          + vlTOPp->SOC__DOT__CPU__DOT__DE_IorSimm);
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
    vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2 = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isALUreg) 
                                             | (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch))
                                             ? vlTOPp->SOC__DOT__CPU__DOT__E_rs2
                                             : vlTOPp->SOC__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->SOC__DOT__CPU__DOT____Vcellinp__DIVU__start 
        = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isDIV) 
            & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard))) 
           & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DIVU__DOT__finished_r)));
    vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch = (1U 
                                                   & (vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__BHT
                                                      [vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__bht_index] 
                                                      >> 1U));
    vlTOPp->SOC__DOT__CPU__DOT__F_stall = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__aluBusy) 
                                            | (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard)) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__halt));
    vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign 
        = (1U & ((~ ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3) 
                     >> 2U)) & ((0U == (3U & (IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_funct3)))
                                 ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b) 
                                    >> 7U) : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                                              >> 0xfU))));
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
    vlTOPp->SOC__DOT__CPU__DOT__F_PC = ((1U & ((~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__FD_nop)) 
                                               & ((vlTOPp->SOC__DOT__inst_rdata 
                                                   >> 3U) 
                                                  | ((6U 
                                                      == 
                                                      (7U 
                                                       & (vlTOPp->SOC__DOT__inst_rdata 
                                                          >> 4U))) 
                                                     & ((vlTOPp->SOC__DOT__inst_rdata 
                                                         >> 2U) 
                                                        | (IData)(vlTOPp->SOC__DOT__CPU__DOT__D_predictBranch))))))
                                         ? ((1U == 
                                             (3U & 
                                              (vlTOPp->SOC__DOT__inst_rdata 
                                               >> 2U)))
                                             ? vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_0
                                             : (vlTOPp->SOC__DOT__CPU__DOT__FD_PC 
                                                + (
                                                   (8U 
                                                    & vlTOPp->SOC__DOT__inst_rdata)
                                                    ? 
                                                   ((0xfff00000U 
                                                     & ((- (IData)(
                                                                   (1U 
                                                                    & (vlTOPp->SOC__DOT__inst_rdata 
                                                                       >> 0x1fU)))) 
                                                        << 0x14U)) 
                                                    | ((0xff000U 
                                                        & vlTOPp->SOC__DOT__inst_rdata) 
                                                       | ((0x800U 
                                                           & (vlTOPp->SOC__DOT__inst_rdata 
                                                              >> 9U)) 
                                                          | (0x7feU 
                                                             & (vlTOPp->SOC__DOT__inst_rdata 
                                                                >> 0x14U)))))
                                                    : vlTOPp->SOC__DOT__CPU__DOT__D_Bimm)))
                                         : ((IData)(vlTOPp->SOC__DOT__CPU__DOT__EM_correctPC)
                                             ? vlTOPp->SOC__DOT__CPU__DOT__EM_PCcorrection
                                             : vlTOPp->SOC__DOT__CPU__DOT__PC));
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
    vlTOPp->SOC__DOT__CPU__DOT__E_LT = (1U & ((0x80000000U 
                                               & (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                  ^ vlTOPp->SOC__DOT__CPU__DOT__E_aluIn2))
                                               ? (vlTOPp->SOC__DOT__CPU__DOT__E_rs1 
                                                  >> 0x1fU)
                                               : (IData)(
                                                         (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                          >> 0x20U))));
    vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch = (1U 
                                                & (((((((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                        & (0U 
                                                           == (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus))) 
                                                       | (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                           >> 1U) 
                                                          & (0U 
                                                             != (IData)(vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus)))) 
                                                      | (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                          >> 4U) 
                                                         & (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT))) 
                                                     | (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                         >> 5U) 
                                                        & (~ (IData)(vlTOPp->SOC__DOT__CPU__DOT__E_LT)))) 
                                                    | (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                        >> 6U) 
                                                       & (IData)(
                                                                 (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                  >> 0x20U)))) 
                                                   | (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_funct3_is) 
                                                       >> 7U) 
                                                      & (~ (IData)(
                                                                   (vlTOPp->SOC__DOT__CPU__DOT__ALU__DOT__alu_minus 
                                                                    >> 0x20U))))));
    vlTOPp->SOC__DOT__CPU__DOT__E_correctPC = (((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isJALR) 
                                                & (vlTOPp->SOC__DOT__CPU__DOT__BP__DOT__ras_0 
                                                   != 
                                                   (0xfffffffeU 
                                                    & vlTOPp->SOC__DOT__CPU__DOT__E_aluPlus))) 
                                               | ((IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_isBranch) 
                                                  & ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_takeBranch) 
                                                     ^ (IData)(vlTOPp->SOC__DOT__CPU__DOT__DE_predictBranch))));
    vlTOPp->SOC__DOT__CPU__DOT__E_flush = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__E_correctPC) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard));
}

void VSOC::_eval_initial(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_eval_initial\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__CLK = vlTOPp->CLK;
    vlTOPp->__Vclklast__TOP__RESET = vlTOPp->RESET;
    vlTOPp->_initial__TOP__3(vlSymsp);
}

void VSOC::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::final\n"); );
    // Variables
    VSOC__Syms* __restrict vlSymsp = this->__VlSymsp;
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VSOC::_eval_settle(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_eval_settle\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__4(vlSymsp);
}

void VSOC::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_ctor_var_reset\n"); );
    // Body
    CLK = VL_RAND_RESET_I(1);
    RESET = VL_RAND_RESET_I(1);
    LEDS = VL_RAND_RESET_I(5);
    RXD = VL_RAND_RESET_I(1);
    TXD = VL_RAND_RESET_I(1);
    SOC__DOT__resetn = VL_RAND_RESET_I(1);
    SOC__DOT__inst_rdata = VL_RAND_RESET_I(32);
    SOC__DOT__data_rdata = VL_RAND_RESET_I(32);
    SOC__DOT__IO_mem_wr = VL_RAND_RESET_I(1);
    SOC__DOT__uart_valid = VL_RAND_RESET_I(1);
    SOC__DOT__uart_ready = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__F_stall = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_flush = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__halt = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__F_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__FD_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__FD_nop = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__D_isJALR = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__D_Bimm = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__D_predictBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_BHTindex = VL_RAND_RESET_I(12);
    SOC__DOT__CPU__DOT__DE_nop = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_rdId = VL_RAND_RESET_I(5);
    SOC__DOT__CPU__DOT__DE_rs1Id = VL_RAND_RESET_I(5);
    SOC__DOT__CPU__DOT__DE_rs2Id = VL_RAND_RESET_I(5);
    SOC__DOT__CPU__DOT__DE_csrId = VL_RAND_RESET_I(2);
    SOC__DOT__CPU__DOT__DE_funct3 = VL_RAND_RESET_I(3);
    SOC__DOT__CPU__DOT__DE_funct3_is = VL_RAND_RESET_I(8);
    SOC__DOT__CPU__DOT__DE_funct7 = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_IorSimm = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DE_isALUreg = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isJALR = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isLoad = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isStore = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isCSRRS = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isEBREAK = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isRV32M = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isDIV = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_wbEnable = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_PCplus4orBimm = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DE_predictBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_PCplus4orUimm = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_rs1 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_rs2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_aluIn2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_aluPlus = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_LT = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT____Vcellinp__DIVU__start = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__aluBusy = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_takeBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_correctPC = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_addr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_nop = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_rdId = VL_RAND_RESET_I(5);
    SOC__DOT__CPU__DOT__EM_csrId_is = VL_RAND_RESET_I(4);
    SOC__DOT__CPU__DOT__EM_funct3 = VL_RAND_RESET_I(3);
    SOC__DOT__CPU__DOT__EM_rs2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_Eresult = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_addr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_isStore = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_isLoad = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_isCSRRS = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_wbEnable = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_correctPC = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__EM_PCcorrection = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__M_STORE_data = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_nop = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__MW_rdId = VL_RAND_RESET_I(5);
    SOC__DOT__CPU__DOT__MW_wbData = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_wbEnable = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__dataHazard = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DE_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbBranch = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbBranchHit = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbJAL = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbJALR = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbJALRhit = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbLoad = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbStore = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbLoadHazard = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbMUL = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__nbDIV = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__CSR__DOT__cycle = VL_RAND_RESET_Q(64);
    SOC__DOT__CPU__DOT__CSR__DOT__instret = VL_RAND_RESET_Q(64);
    SOC__DOT__CPU__DOT__BP__DOT__branch_history = VL_RAND_RESET_I(9);
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            SOC__DOT__CPU__DOT__BP__DOT__BHT[__Vi0] = VL_RAND_RESET_I(2);
    }}
    SOC__DOT__CPU__DOT__BP__DOT__ras_0 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__BP__DOT__ras_1 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__BP__DOT__ras_2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__BP__DOT__ras_3 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__BP__DOT__bht_index = VL_RAND_RESET_I(12);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            SOC__DOT__CPU__DOT__RF__DOT__regs[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SOC__DOT__CPU__DOT__DIVU__DOT__dividend_r = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DIVU__DOT__divisor_r = VL_RAND_RESET_Q(63);
    SOC__DOT__CPU__DOT__DIVU__DOT__quotient_r = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DIVU__DOT__sign_r = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DIVU__DOT__busy_r = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DIVU__DOT__finished_r = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DIVU__DOT__divstep_do = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__DIVU__DOT__divsel = VL_RAND_RESET_I(3);
    SOC__DOT__CPU__DOT__ALU__DOT__alu_minus = VL_RAND_RESET_Q(33);
    SOC__DOT__CPU__DOT__ALU__DOT__shifter_in = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__ALU__DOT__shifter = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__ALU__DOT__leftshift = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__ALU__DOT__genblk1__DOT__multiply = VL_RAND_RESET_Q(64);
    SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h = VL_RAND_RESET_I(16);
    SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b = VL_RAND_RESET_I(8);
    SOC__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            SOC__DOT__PROGROM__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            SOC__DOT__DATARAM__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SOC__DOT__UART__DOT__cnt = VL_RAND_RESET_I(8);
    SOC__DOT__UART__DOT__data = VL_RAND_RESET_I(10);
    SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt = VL_RAND_RESET_I(16);
    __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__8__x = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__ALU__DOT__flip32__9__x = VL_RAND_RESET_I(32);
}
