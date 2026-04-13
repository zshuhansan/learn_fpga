#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
VIVADO_SETTINGS=${VIVADO_SETTINGS:-$HOME/Xilinx/Vivado/2023.2/settings64.sh}
VIVADO_JOBS=${VIVADO_JOBS:-$(nproc)}
ENABLE_RV32M=${ENABLE_RV32M:-1}
variant_suffix=${VIVADO_VARIANT:+_$VIVADO_VARIANT}
REPORTS_DIR=$SCRIPT_DIR/reports_zynq7020_clg400_2${variant_suffix}
SYNTH_DCP=$REPORTS_DIR/soc_zynq7020_clg400_2${variant_suffix}_post_synth.dcp

if [[ ! -f "$VIVADO_SETTINGS" ]]; then
  echo "Vivado settings64.sh not found: $VIVADO_SETTINGS" >&2
  exit 1
fi

source "$VIVADO_SETTINGS"
export VIVADO_JOBS
export ENABLE_RV32M

if [[ ! -f "$SYNTH_DCP" || "${FORCE_SYNTH:-0}" == "1" ]]; then
  "$SCRIPT_DIR/run_soc_zynq7020_synth.sh"
fi

vivado -mode batch -source "$SCRIPT_DIR/run_soc_zynq7020_impl.tcl" | tee "$SCRIPT_DIR/run_soc_zynq7020_impl.log"