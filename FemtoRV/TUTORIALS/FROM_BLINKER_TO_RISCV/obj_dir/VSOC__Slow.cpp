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
    WData/*95:0*/ __Vtemp5[3];
    WData/*95:0*/ __Vtemp6[3];
    // Body
    __Vtemp5[0U] = 0x2e686578U;
    __Vtemp5[1U] = 0x4152414dU;
    __Vtemp5[2U] = 0x444154U;
    VL_READMEM_N(true, 32, 16384, 0, VL_CVT_PACK_STR_NW(3, __Vtemp5)
                 , vlTOPp->SOC__DOT__CPU__DOT__DATARAM
                 , 0, ~0ULL);
    vlTOPp->SOC__DOT__UART__DOT__cnt = 0U;
    __Vtemp6[0U] = 0x2e686578U;
    __Vtemp6[1U] = 0x47524f4dU;
    __Vtemp6[2U] = 0x50524fU;
    VL_READMEM_N(true, 32, 16384, 0, VL_CVT_PACK_STR_NW(3, __Vtemp6)
                 , vlTOPp->SOC__DOT__CPU__DOT__PROGROM
                 , 0, ~0ULL);
    vlTOPp->SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt = 0U;
}

void VSOC::_settle__TOP__4(VSOC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSOC::_settle__TOP__4\n"); );
    VSOC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->TXD = (1U & ((IData)(vlTOPp->SOC__DOT__UART__DOT__data) 
                         | (~ (IData)((0U != (IData)(vlTOPp->SOC__DOT__UART__DOT__data))))));
    vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H = (0xffffU 
                                            & ((2U 
                                                & vlTOPp->SOC__DOT__CPU__DOT__MW_addr)
                                                ? (vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata 
                                                   >> 0x10U)
                                                : vlTOPp->SOC__DOT__CPU__DOT__MW_Mdata));
    vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data = ((0xffffff00U 
                                                 & vlTOPp->SOC__DOT__CPU__DOT__M_STORE_data) 
                                                | (0xffU 
                                                   & vlTOPp->SOC__DOT__CPU__DOT__EM_rs2));
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__I 
        = vlTOPp->SOC__DOT__CPU__DOT__MW_instr;
    vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout 
        = (0x1fU & (vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__I 
                    >> 7U));
    vlTOPp->SOC__DOT__CPU__DOT__wbRdId = vlTOPp->__Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout;
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
    vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_B = (0xffU & 
                                            ((1U & vlTOPp->SOC__DOT__CPU__DOT__MW_addr)
                                              ? ((IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H) 
                                                 >> 8U)
                                              : (IData)(vlTOPp->SOC__DOT__CPU__DOT__W_LOAD_H)));
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
    vlTOPp->SOC__DOT__CPU__DOT__dataHazard = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__rs1Hazard) 
                                              | (IData)(vlTOPp->SOC__DOT__CPU__DOT__rs2Hazard));
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
    vlTOPp->SOC__DOT__CPU__DOT__D_stall = ((IData)(vlTOPp->SOC__DOT__CPU__DOT__dataHazard) 
                                           | (IData)(vlTOPp->SOC__DOT__CPU__DOT__halt));
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
    SOC__DOT__IO_mem_wr = VL_RAND_RESET_I(1);
    SOC__DOT__uart_valid = VL_RAND_RESET_I(1);
    SOC__DOT__uart_ready = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__cycle = VL_RAND_RESET_Q(64);
    SOC__DOT__CPU__DOT__instret = VL_RAND_RESET_Q(64);
    SOC__DOT__CPU__DOT__E_flush = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__D_stall = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__halt = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__F_PC = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            SOC__DOT__CPU__DOT__PROGROM[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SOC__DOT__CPU__DOT__FD_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__FD_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__FD_nop = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__wbEnable = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__wbData = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__wbRdId = VL_RAND_RESET_I(5);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            SOC__DOT__CPU__DOT__RegisterBank[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SOC__DOT__CPU__DOT__DE_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DE_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DE_rs1 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__DE_rs2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_aluIn2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_minus = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_aluPlus = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_aluMinus = VL_RAND_RESET_Q(33);
    SOC__DOT__CPU__DOT__E_LT = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_shifter_in = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_shifter = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_leftshift = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_aluOut = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_takeBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_JumpOrBranch = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__E_JumpOrBranchAddr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__E_result = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_rs2 = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_Eresult = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__EM_addr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__M_funct3 = VL_RAND_RESET_I(3);
    SOC__DOT__CPU__DOT__M_STORE_data = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__M_wmask = VL_RAND_RESET_I(4);
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            SOC__DOT__CPU__DOT__DATARAM[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SOC__DOT__CPU__DOT__MW_PC = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_instr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_Eresult = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_addr = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_Mdata = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_IOresult = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__MW_CSRresult = VL_RAND_RESET_I(32);
    SOC__DOT__CPU__DOT__W_funct3 = VL_RAND_RESET_I(3);
    SOC__DOT__CPU__DOT__W_LOAD_H = VL_RAND_RESET_I(16);
    SOC__DOT__CPU__DOT__W_LOAD_B = VL_RAND_RESET_I(8);
    SOC__DOT__CPU__DOT__W_LOAD_sign = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__rs1Hazard = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__rs2Hazard = VL_RAND_RESET_I(1);
    SOC__DOT__CPU__DOT__dataHazard = VL_RAND_RESET_I(1);
    SOC__DOT__UART__DOT__cnt = VL_RAND_RESET_I(8);
    SOC__DOT__UART__DOT__data = VL_RAND_RESET_I(10);
    SOC__DOT__CW__DOT__genblk2__DOT__reset_cnt = VL_RAND_RESET_I(16);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__2__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__3__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__3__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Iimm__4__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Iimm__4__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__7__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__8__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__8__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__flip32__9__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__flip32__9__x = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__flip32__10__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__flip32__10__x = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__11__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__11__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__12__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__12__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__13__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__13__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__14__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__14__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__15__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__15__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__16__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__16__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__17__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__17__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__18__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__18__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__19__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__19__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__20__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__20__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__21__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__21__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__22__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__22__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__23__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__23__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__24__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__24__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJALR__25__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJALR__25__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__26__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__26__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__27__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__27__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Bimm__28__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Bimm__28__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__29__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__29__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Jimm__30__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Jimm__30__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__31__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__31__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJALR__32__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJALR__32__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isLUI__33__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isLUI__33__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Uimm__34__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Uimm__34__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__35__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Uimm__36__Vfuncout = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__Uimm__36__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isEBREAK__40__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__41__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__42__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__42__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__43__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__43__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__44__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__44__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__45__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__45__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__49__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__49__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isLoad__50__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isLoad__50__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isCSRRS__51__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isSYSTEM__52__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__53__Vfuncout = VL_RAND_RESET_I(3);
    __Vfunc_SOC__DOT__CPU__DOT__funct3__53__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__54__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__54__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__55__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__55__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__56__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__56__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__57__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__57__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__58__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__58__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__readsRs1__59__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__60__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__61__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__61__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__62__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__63__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__63__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__64__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__64__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__65__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__66__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__66__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__67__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__67__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs1Id__68__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__69__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__69__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__70__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__70__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__71__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__71__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__72__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__72__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__73__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__73__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__74__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__74__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__75__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__75__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__76__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isJAL__76__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isAUIPC__77__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isLUI__78__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isLUI__78__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__readsRs2__79__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__80__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__81__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__81__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__82__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__83__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__83__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__84__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__84__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__85__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__86__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__86__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__87__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__writesRd__87__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rs2Id__88__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__89__Vfuncout = VL_RAND_RESET_I(5);
    __Vfunc_SOC__DOT__CPU__DOT__rdId__89__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__90__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__90__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__91__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__91__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__92__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__92__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__93__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__93__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__94__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__94__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__95__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__95__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isALUreg__96__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__97__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isBranch__97__I = VL_RAND_RESET_I(32);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__98__Vfuncout = VL_RAND_RESET_I(1);
    __Vfunc_SOC__DOT__CPU__DOT__isStore__98__I = VL_RAND_RESET_I(32);
}
