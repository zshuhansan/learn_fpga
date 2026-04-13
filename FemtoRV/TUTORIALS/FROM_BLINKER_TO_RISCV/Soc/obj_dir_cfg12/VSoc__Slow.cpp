// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSoc.h for the primary calling header

#include "VSoc.h"
#include "VSoc__Syms.h"

//==========

VL_CTOR_IMP(VSoc) {
    VSoc__Syms* __restrict vlSymsp = __VlSymsp = new VSoc__Syms(this, name());
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VSoc::__Vconfigure(VSoc__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

VSoc::~VSoc() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void VSoc::_initial__TOP__1(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_initial__TOP__1\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*95:0*/ __Vtemp2[3];
    WData/*95:0*/ __Vtemp6[3];
    // Body
    vlTOPp->Soc__DOT__AXI_DDR__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x40000U, vlTOPp->Soc__DOT__AXI_DDR__DOT__i)) {
        vlTOPp->Soc__DOT__AXI_DDR__DOT__memory[(0x3ffffU 
                                                & vlTOPp->Soc__DOT__AXI_DDR__DOT__i)] = 0U;
        vlTOPp->Soc__DOT__AXI_DDR__DOT__i = ((IData)(1U) 
                                             + vlTOPp->Soc__DOT__AXI_DDR__DOT__i);
    }
    __Vtemp2[0U] = 0x2e686578U;
    __Vtemp2[1U] = 0x47524f4dU;
    __Vtemp2[2U] = 0x50524fU;
    VL_READMEM_N(true, 32, 262144, 0, VL_CVT_PACK_STR_NW(3, __Vtemp2)
                 , vlTOPp->Soc__DOT__AXI_DDR__DOT__memory
                 , 0, ~0ULL);
    vlTOPp->Soc__DOT__CPU__DOT__nbBranch = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbBranchHit = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackward = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbBranchBackwardHit = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbBranchForward = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbBranchForwardHit = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbJAL = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbJALR = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbJALRhit = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbLoad = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbStore = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbLoadHazard = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbMUL = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbDIV = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbPHTSharedEntries = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 = 0U;
    vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0U;
    while (VL_GTS_III(1,32,32, 0x100U, vlTOPp->Soc__DOT__CPU__DOT__stats_i)) {
        vlTOPp->Soc__DOT__CPU__DOT__brProfTotal[(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__brProfHit[(0xffU 
                                               & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__brProfPC[(0xffU 
                                              & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__brProfValid[(0xffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__reportUsed[(0xffU 
                                                & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__stats_i = ((IData)(1U) 
                                               + vlTOPp->Soc__DOT__CPU__DOT__stats_i);
    }
    vlTOPp->Soc__DOT__CPU__DOT__stats_i = 0U;
    while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__stats_i)) {
        vlTOPp->Soc__DOT__CPU__DOT__phtUseCount[(0xfffU 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtAliasCount[(0xfffU 
                                                   & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtUniqueCount[(0xfffU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtPC0[(0xfffU 
                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtPC1[(0xfffU 
                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtPC2[(0xfffU 
                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__phtPC3[(0xfffU 
                                            & vlTOPp->Soc__DOT__CPU__DOT__stats_i)] = 0U;
        vlTOPp->Soc__DOT__CPU__DOT__stats_i = ((IData)(1U) 
                                               + vlTOPp->Soc__DOT__CPU__DOT__stats_i);
    }
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x1000U, vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__i)) {
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__BHT[(0xfffU 
                                                  & vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__i)] = 1U;
        vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__i = ((IData)(1U) 
                                                  + vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__i);
    }
    vlTOPp->Soc__DOT__uart_ready = 1U;
    vlTOPp->Soc__DOT__UART__DOT__cnt = 0x56U;
    vlTOPp->Soc__DOT__UART__DOT__data = 0U;
    __Vtemp6[0U] = 0x2e686578U;
    __Vtemp6[1U] = 0x4152414dU;
    __Vtemp6[2U] = 0x444154U;
    VL_READMEM_N(true, 32, 16384, 0, VL_CVT_PACK_STR_NW(3, __Vtemp6)
                 , vlTOPp->Soc__DOT__DATARAM__DOT__mem
                 , 0, ~0ULL);
    vlTOPp->Soc__DOT__ICACHE__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x80U, vlTOPp->Soc__DOT__ICACHE__DOT__i)) {
        vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way0[(0x7fU 
                                                         & vlTOPp->Soc__DOT__ICACHE__DOT__i)] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__valid_array_way1[(0x7fU 
                                                         & vlTOPp->Soc__DOT__ICACHE__DOT__i)] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__lru_array[(0x7fU 
                                                  & vlTOPp->Soc__DOT__ICACHE__DOT__i)] = 0U;
        vlTOPp->Soc__DOT__ICACHE__DOT__i = ((IData)(1U) 
                                            + vlTOPp->Soc__DOT__ICACHE__DOT__i);
    }
}

void VSoc::_settle__TOP__3(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_settle__TOP__3\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->HALT = ((IData)(vlTOPp->RESET) & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isEBREAK));
    vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->Soc__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->Soc__DOT__CPU__DOT__EM_rs2));
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divstep_do 
        = (vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r 
           <= (QData)((IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r)));
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
    vlTOPp->Soc__DOT__IO_mem_wr = ((IData)(vlTOPp->RESET) 
                                   & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_isStore) 
                                      & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                         >> 0x16U)));
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h 
        = (0xffffU & ((2U & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                       ? (vlTOPp->Soc__DOT__data_rdata 
                          >> 0x10U) : vlTOPp->Soc__DOT__data_rdata));
    vlTOPp->Soc__DOT__CPU__DOT__E_rs1 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)))
                                          ? vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)))
                                              ? vlTOPp->Soc__DOT__CPU__DOT__MW_wbData
                                              : vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Data));
    vlTOPp->Soc__DOT__CPU__DOT__E_rs2 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_wbEnable) 
                                          & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_rdId) 
                                             == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id)))
                                          ? vlTOPp->Soc__DOT__CPU__DOT__EM_Eresult
                                          : (((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_wbEnable) 
                                              & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__MW_rdId) 
                                                 == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Id)))
                                              ? vlTOPp->Soc__DOT__CPU__DOT__MW_wbData
                                              : vlTOPp->Soc__DOT__CPU__DOT__DE_rs2Data));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__de_is_call 
        = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJAL) 
           & (1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId)));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__de_is_return 
        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR) 
            & (0U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rdId))) 
           & ((1U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id)) 
              | (5U == (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_rs1Id))));
    vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__divsel = 
        (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
          << 2U) | ((2U & (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_funct3)) 
                    | (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__sign_r)));
    vlTOPp->Soc__DOT__CPU__DOT__aluBusy = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__busy_r) 
                                           | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
                                              & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r))));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__d_is_call 
        = ((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
            >> 3U) & (1U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                      >> 7U))));
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
    vlTOPp->Soc__DOT__CPU__DOT__D_Jimm = ((0xfff00000U 
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
                                                      >> 0x14U)))));
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
    vlTOPp->Soc__DOT__CPU__DOT__D_isJALR = (5U == (
                                                   (4U 
                                                    & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                       >> 4U)) 
                                                   | (3U 
                                                      & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                         >> 2U))));
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__bht_index 
        = (0xfffU & ((vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                      >> 2U) ^ (IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_hash_q)));
    vlTOPp->Soc__DOT__axi_arvalid = (1U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state));
    vlTOPp->Soc__DOT__axi_rready = (2U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state));
    vlTOPp->Soc__DOT__axi_arready = (0U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state));
    vlTOPp->Soc__DOT__axi_rvalid = (2U == (IData)(vlTOPp->Soc__DOT__AXI_DDR__DOT__state));
    vlTOPp->Soc__DOT__axi_rdata = vlTOPp->Soc__DOT__AXI_DDR__DOT__memory
        [(0x3ffffU & (vlTOPp->Soc__DOT__AXI_DDR__DOT__current_addr 
                      >> 2U))];
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->Soc__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->Soc__DOT__UART__DOT__data))))));
    vlTOPp->Soc__DOT__uart_valid = ((IData)(vlTOPp->Soc__DOT__IO_mem_wr) 
                                    & (vlTOPp->Soc__DOT__CPU__DOT__EM_addr 
                                       >> 3U));
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b 
        = (0xffU & ((1U & vlTOPp->Soc__DOT__CPU__DOT__EM_addr)
                     ? ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                        >> 8U) : (IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h)));
    vlTOPp->Soc__DOT__CPU__DOT__E_addr = (vlTOPp->Soc__DOT__CPU__DOT__E_rs1 
                                          + vlTOPp->Soc__DOT__CPU__DOT__DE_IorSimm);
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
    vlTOPp->Soc__DOT__CPU__DOT__E_aluIn2 = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isALUreg) 
                                             | (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch))
                                             ? vlTOPp->Soc__DOT__CPU__DOT__E_rs2
                                             : vlTOPp->Soc__DOT__CPU__DOT__DE_IorSimm);
    vlTOPp->Soc__DOT__CPU__DOT____Vcellinp__DIVU__start 
        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isDIV) 
            & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard))) 
           & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DIVU__DOT__finished_r)));
    vlTOPp->Soc__DOT__CPU__DOT__D_stall = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__aluBusy) 
                                            | (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard)) 
                                           | (IData)(vlTOPp->HALT));
    vlTOPp->Soc__DOT__CPU__DOT__D_PCplusBimm = (vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                                                + vlTOPp->Soc__DOT__CPU__DOT__D_Bimm);
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__d_is_return 
        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isJALR) 
            & (0U == (0x1fU & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                               >> 7U)))) & ((1U == 
                                             (0x1fU 
                                              & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                 >> 0xfU))) 
                                            | (5U == 
                                               (0x1fU 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                   >> 0xfU)))));
    vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isJALR)
                                                   ? vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0
                                                   : 
                                                  ((8U 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg)
                                                    ? 
                                                   (vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                                                    + vlTOPp->Soc__DOT__CPU__DOT__D_Jimm)
                                                    : 
                                                   (vlTOPp->Soc__DOT__CPU__DOT__FD_PC 
                                                    + vlTOPp->Soc__DOT__CPU__DOT__D_Bimm)));
    vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch = (1U 
                                                   & (vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__BHT
                                                      [vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__bht_index] 
                                                      >> 1U));
    vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign 
        = (1U & ((~ ((IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3) 
                     >> 2U)) & ((0U == (3U & (IData)(vlTOPp->Soc__DOT__CPU__DOT__EM_funct3)))
                                 ? ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b) 
                                    >> 7U) : ((IData)(vlTOPp->Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h) 
                                              >> 0xfU))));
    vlTOPp->Soc__DOT__CPU__DOT__ALU__DOT__multiply 
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
    vlTOPp->Soc__DOT__inst_en = ((((IData)(vlTOPp->RESET) 
                                   & (~ (IData)(vlTOPp->HALT))) 
                                  & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_stall))) 
                                 & (~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FB_valid)));
    vlTOPp->Soc__DOT__CPU__DOT__D_predictPC = (1U & 
                                               ((~ (IData)(vlTOPp->Soc__DOT__CPU__DOT__FD_nop)) 
                                                & (((vlTOPp->Soc__DOT__CPU__DOT__FD_instr_reg 
                                                     >> 3U) 
                                                    | (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isJALR)) 
                                                   | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_isBranch) 
                                                      & (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictBranch)))));
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
    vlTOPp->Soc__DOT__CPU__DOT__F_reqFire = ((IData)(vlTOPp->Soc__DOT__inst_en) 
                                             & (0U 
                                                == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)));
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
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_next 
        = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
            << 0xbU) | (0x7ffU & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history) 
                                  >> 1U)));
    vlTOPp->Soc__DOT__CPU__DOT__E_correctPC = (((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isJALR) 
                                                & (vlTOPp->Soc__DOT__CPU__DOT__DE_predictRA 
                                                   != 
                                                   (0xfffffffeU 
                                                    & vlTOPp->Soc__DOT__CPU__DOT__E_aluPlus))) 
                                               | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_isBranch) 
                                                  & ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_takeBranch) 
                                                     ^ (IData)(vlTOPp->Soc__DOT__CPU__DOT__DE_predictBranch))));
    vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history 
        = vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_next;
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
    vlTOPp->Soc__DOT__CPU__DOT__BP__DOT__branch_history_hash_next 
        = vlTOPp->__Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout;
    vlTOPp->Soc__DOT__CPU__DOT__E_flush = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) 
                                           | (IData)(vlTOPp->Soc__DOT__CPU__DOT__dataHazard));
    vlTOPp->Soc__DOT__CPU__DOT__F_redirect = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) 
                                              | ((IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC) 
                                                 & (vlTOPp->Soc__DOT__CPU__DOT__F_pendingPC 
                                                    != vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction)));
    vlTOPp->Soc__DOT__CPU__DOT__F_useRedirectPC = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC) 
                                                   | (IData)(vlTOPp->Soc__DOT__CPU__DOT__D_predictPC));
    vlTOPp->Soc__DOT__CPU__DOT__F_epochNext = (3U & 
                                               ((IData)(vlTOPp->Soc__DOT__CPU__DOT__F_redirect)
                                                 ? 
                                                ((IData)(1U) 
                                                 + (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_epoch))
                                                 : (IData)(vlTOPp->Soc__DOT__CPU__DOT__F_epoch)));
    if (vlTOPp->Soc__DOT__CPU__DOT__F_useRedirectPC) {
        vlTOPp->Soc__DOT__CPU__DOT__F_nextPC = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC)
                                                 ? 
                                                ((IData)(4U) 
                                                 + vlTOPp->Soc__DOT__CPU__DOT__E_PCcorrection)
                                                 : 
                                                ((IData)(4U) 
                                                 + vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction));
        vlTOPp->Soc__DOT__CPU__DOT__F_PC = ((IData)(vlTOPp->Soc__DOT__CPU__DOT__E_correctPC)
                                             ? vlTOPp->Soc__DOT__CPU__DOT__E_PCcorrection
                                             : vlTOPp->Soc__DOT__CPU__DOT__D_PCprediction);
    } else {
        vlTOPp->Soc__DOT__CPU__DOT__F_nextPC = ((IData)(4U) 
                                                + vlTOPp->Soc__DOT__CPU__DOT__PC);
        vlTOPp->Soc__DOT__CPU__DOT__F_PC = vlTOPp->Soc__DOT__CPU__DOT__PC;
    }
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
    vlTOPp->Soc__DOT__ICACHE__DOT__is_hit = (((0U == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
                                              & (IData)(vlTOPp->Soc__DOT__inst_en)) 
                                             & (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__cache_hit));
    vlTOPp->Soc__DOT__ICACHE__DOT__is_miss = (((0U 
                                                == (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__state)) 
                                               & (IData)(vlTOPp->Soc__DOT__inst_en)) 
                                              & (~ (IData)(vlTOPp->Soc__DOT__ICACHE__DOT__cache_hit)));
}

void VSoc::_eval_initial(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_eval_initial\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
    vlTOPp->__Vclklast__TOP__CLK = vlTOPp->CLK;
}

void VSoc::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::final\n"); );
    // Variables
    VSoc__Syms* __restrict vlSymsp = this->__VlSymsp;
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VSoc::_eval_settle(VSoc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_eval_settle\n"); );
    VSoc* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

void VSoc::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoc::_ctor_var_reset\n"); );
    // Body
    CLK = VL_RAND_RESET_I(1);
    RESET = VL_RAND_RESET_I(1);
    LEDS = VL_RAND_RESET_I(5);
    RXD = VL_RAND_RESET_I(1);
    TXD = VL_RAND_RESET_I(1);
    HALT = VL_RAND_RESET_I(1);
    Soc__DOT__inst_en = VL_RAND_RESET_I(1);
    Soc__DOT__data_rdata = VL_RAND_RESET_I(32);
    Soc__DOT__data_wmask = VL_RAND_RESET_I(4);
    Soc__DOT__IO_mem_wr = VL_RAND_RESET_I(1);
    Soc__DOT__axi_arvalid = VL_RAND_RESET_I(1);
    Soc__DOT__axi_arready = VL_RAND_RESET_I(1);
    Soc__DOT__axi_rdata = VL_RAND_RESET_I(32);
    Soc__DOT__axi_rvalid = VL_RAND_RESET_I(1);
    Soc__DOT__axi_rready = VL_RAND_RESET_I(1);
    Soc__DOT__uart_valid = VL_RAND_RESET_I(1);
    Soc__DOT__uart_ready = VL_RAND_RESET_I(1);
    Soc__DOT__MBOX0 = VL_RAND_RESET_I(32);
    Soc__DOT__MBOX1 = VL_RAND_RESET_I(32);
    Soc__DOT__MBOX2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__D_stall = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__E_flush = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__dataHazard = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__aluBusy = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__E_correctPC = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__E_takeBranch = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__PC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__F_pendingPC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__F_pendingEpoch = VL_RAND_RESET_I(2);
    Soc__DOT__CPU__DOT__F_pendingValid = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__F_epoch = VL_RAND_RESET_I(2);
    Soc__DOT__CPU__DOT__FB_PC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__FB_instr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__FB_epoch = VL_RAND_RESET_I(2);
    Soc__DOT__CPU__DOT__FB_valid = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__E_PCcorrection = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__FD_PC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__FD_instr_reg = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__FD_nop = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__D_isBranch = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__D_isJALR = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__D_Bimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__D_Jimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__D_predictPC = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__D_PCprediction = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__D_predictBranch = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__D_PCplusBimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_nop = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_rdId = VL_RAND_RESET_I(5);
    Soc__DOT__CPU__DOT__DE_rs1Id = VL_RAND_RESET_I(5);
    Soc__DOT__CPU__DOT__DE_rs2Id = VL_RAND_RESET_I(5);
    Soc__DOT__CPU__DOT__DE_rs1Data = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_rs2Data = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_csrId = VL_RAND_RESET_I(2);
    Soc__DOT__CPU__DOT__DE_funct3 = VL_RAND_RESET_I(3);
    Soc__DOT__CPU__DOT__DE_funct3_is = VL_RAND_RESET_I(8);
    Soc__DOT__CPU__DOT__DE_funct7 = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_IorSimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_isALUreg = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isBranch = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isJALR = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isJAL = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isLoad = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isStore = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isCSRRS = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isEBREAK = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_wbEnable = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isJALorJALRorLUIorAUIPC = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_PCplus4orUimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_isRV32M = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_isDIV = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_PCplus4orBimm = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_predictBranch = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_predictRA = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_BHTindex = VL_RAND_RESET_I(12);
    Soc__DOT__CPU__DOT__E_rs1 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__E_rs2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__E_aluIn2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__E_aluPlus = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__E_LT = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__E_addr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__EM_nop = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__EM_rdId = VL_RAND_RESET_I(5);
    Soc__DOT__CPU__DOT__EM_csrId_is = VL_RAND_RESET_I(4);
    Soc__DOT__CPU__DOT__EM_funct3 = VL_RAND_RESET_I(3);
    Soc__DOT__CPU__DOT__EM_rs2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__EM_Eresult = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__EM_addr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__EM_isStore = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__EM_isLoad = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__EM_isCSRRS = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__EM_wbEnable = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__M_STORE_data = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__MW_nop = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__MW_rdId = VL_RAND_RESET_I(5);
    Soc__DOT__CPU__DOT__MW_wbData = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__MW_wbEnable = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__F_reqFire = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__F_redirect = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__F_epochNext = VL_RAND_RESET_I(2);
    Soc__DOT__CPU__DOT__F_useRedirectPC = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__F_PC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__F_nextPC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT____Vcellinp__DIVU__start = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DE_instr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DE_PC = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__EM_instr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__MW_instr = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranch = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranchHit = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranchBackward = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranchBackwardHit = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranchForward = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbBranchForwardHit = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbJAL = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbJALR = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbJALRhit = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbLoad = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbStore = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbLoadHazard = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbMUL = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbDIV = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbPHTSharedEntries = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__nbPHTUniquePcOver1 = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            Soc__DOT__CPU__DOT__brProfTotal[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            Soc__DOT__CPU__DOT__brProfHit[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            Soc__DOT__CPU__DOT__brProfPC[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            Soc__DOT__CPU__DOT__brProfValid[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            Soc__DOT__CPU__DOT__reportUsed[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtUseCount[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtAliasCount[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtUniqueCount[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtPC0[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtPC1[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtPC2[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__phtPC3[__Vi0] = VL_RAND_RESET_I(32);
    }}
    Soc__DOT__CPU__DOT__stats_i = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_slot = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_free = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_idx = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_best = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_unique = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__stats_alias = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__CSR__DOT__cycle = VL_RAND_RESET_Q(64);
    Soc__DOT__CPU__DOT__CSR__DOT__instret = VL_RAND_RESET_Q(64);
    Soc__DOT__CPU__DOT__BP__DOT__branch_history = VL_RAND_RESET_I(12);
    Soc__DOT__CPU__DOT__BP__DOT__branch_history_hash_q = VL_RAND_RESET_I(12);
    { int __Vi0=0; for (; __Vi0<4096; ++__Vi0) {
            Soc__DOT__CPU__DOT__BP__DOT__BHT[__Vi0] = VL_RAND_RESET_I(2);
    }}
    Soc__DOT__CPU__DOT__BP__DOT__ras_spec_0 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_spec_1 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_spec_2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_spec_3 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_arch_0 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_arch_1 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_arch_2 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__ras_arch_3 = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT__d_is_call = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__BP__DOT__d_is_return = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__BP__DOT__de_is_call = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__BP__DOT__de_is_return = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__BP__DOT__branch_history_next = VL_RAND_RESET_I(12);
    Soc__DOT__CPU__DOT__BP__DOT__branch_history_hash_next = VL_RAND_RESET_I(12);
    Soc__DOT__CPU__DOT__BP__DOT__bht_index = VL_RAND_RESET_I(12);
    Soc__DOT__CPU__DOT__BP__DOT__i = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__BP__DOT____Vlvbound1 = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            Soc__DOT__CPU__DOT__RF__DOT__regs[__Vi0] = VL_RAND_RESET_I(32);
    }}
    Soc__DOT__CPU__DOT__DIVU__DOT__dividend_r = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DIVU__DOT__divisor_r = VL_RAND_RESET_Q(63);
    Soc__DOT__CPU__DOT__DIVU__DOT__quotient_r = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DIVU__DOT__quotient_msk_r = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__DIVU__DOT__sign_r = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DIVU__DOT__busy_r = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DIVU__DOT__finished_r = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DIVU__DOT__divstep_do = VL_RAND_RESET_I(1);
    Soc__DOT__CPU__DOT__DIVU__DOT__divsel = VL_RAND_RESET_I(3);
    Soc__DOT__CPU__DOT__ALU__DOT__alu_minus = VL_RAND_RESET_Q(33);
    Soc__DOT__CPU__DOT__ALU__DOT__shifter_in = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__ALU__DOT__shifter = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__ALU__DOT__leftshift = VL_RAND_RESET_I(32);
    Soc__DOT__CPU__DOT__ALU__DOT__multiply = VL_RAND_RESET_Q(64);
    Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_h = VL_RAND_RESET_I(16);
    Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_b = VL_RAND_RESET_I(8);
    Soc__DOT__CPU__DOT__LSU_ALIGN__DOT__load_sign = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            VL_RAND_RESET_W(256, Soc__DOT__ICACHE__DOT__data_array_way0[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            Soc__DOT__ICACHE__DOT__tag_array_way0[__Vi0] = VL_RAND_RESET_I(20);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            Soc__DOT__ICACHE__DOT__valid_array_way0[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            VL_RAND_RESET_W(256, Soc__DOT__ICACHE__DOT__data_array_way1[__Vi0]);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            Soc__DOT__ICACHE__DOT__tag_array_way1[__Vi0] = VL_RAND_RESET_I(20);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            Soc__DOT__ICACHE__DOT__valid_array_way1[__Vi0] = VL_RAND_RESET_I(1);
    }}
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            Soc__DOT__ICACHE__DOT__lru_array[__Vi0] = VL_RAND_RESET_I(1);
    }}
    Soc__DOT__ICACHE__DOT__hit_way0 = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__hit_way1_raw = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__cache_hit = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__state = VL_RAND_RESET_I(3);
    Soc__DOT__ICACHE__DOT__burst_cnt = VL_RAND_RESET_I(3);
    VL_RAND_RESET_W(256, Soc__DOT__ICACHE__DOT__refill_line);
    Soc__DOT__ICACHE__DOT__miss_addr = VL_RAND_RESET_I(32);
    Soc__DOT__ICACHE__DOT__replace_way = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__miss_word_offset = VL_RAND_RESET_I(3);
    Soc__DOT__ICACHE__DOT__resp_data_r = VL_RAND_RESET_I(32);
    Soc__DOT__ICACHE__DOT__resp_valid_r = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__is_hit = VL_RAND_RESET_I(1);
    Soc__DOT__ICACHE__DOT__is_miss = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(256, Soc__DOT__ICACHE__DOT__cache_line);
    Soc__DOT__ICACHE__DOT__hit_count = VL_RAND_RESET_I(32);
    Soc__DOT__ICACHE__DOT__miss_count = VL_RAND_RESET_I(32);
    Soc__DOT__ICACHE__DOT__i = VL_RAND_RESET_I(32);
    Soc__DOT__ICACHE__DOT__stats_printed = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<262144; ++__Vi0) {
            Soc__DOT__AXI_DDR__DOT__memory[__Vi0] = VL_RAND_RESET_I(32);
    }}
    Soc__DOT__AXI_DDR__DOT__i = VL_RAND_RESET_I(32);
    Soc__DOT__AXI_DDR__DOT__state = VL_RAND_RESET_I(2);
    Soc__DOT__AXI_DDR__DOT__current_addr = VL_RAND_RESET_I(32);
    Soc__DOT__AXI_DDR__DOT__burst_left = VL_RAND_RESET_I(8);
    Soc__DOT__AXI_DDR__DOT__latency_cnt = VL_RAND_RESET_I(4);
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            Soc__DOT__DATARAM__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }}
    Soc__DOT__UART__DOT__cnt = VL_RAND_RESET_I(8);
    Soc__DOT__UART__DOT__data = VL_RAND_RESET_I(10);
    __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__Vfuncout = VL_RAND_RESET_I(12);
    __Vfunc_Soc__DOT__CPU__DOT__BP__DOT__history_hash__7__history = VL_RAND_RESET_I(12);
    __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__9__x = VL_RAND_RESET_I(32);
    __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_Soc__DOT__CPU__DOT__ALU__DOT__flip32__10__x = VL_RAND_RESET_I(32);
}
