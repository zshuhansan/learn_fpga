#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
VIVADO_SETTINGS=${VIVADO_SETTINGS:-$HOME/Xilinx/Vivado/2023.2/settings64.sh}
VIVADO_JOBS=${VIVADO_JOBS:-$(nproc)}
ENABLE_RV32M=${ENABLE_RV32M:-1}

if [[ ! -f "$VIVADO_SETTINGS" ]]; then
  echo "Vivado settings64.sh not found: $VIVADO_SETTINGS" >&2
  exit 1
fi

source "$VIVADO_SETTINGS"
export VIVADO_JOBS
export ENABLE_RV32M

vivado -mode batch -source "$SCRIPT_DIR/run_soc_zynq7020_synth.tcl" | tee "$SCRIPT_DIR/run_soc_zynq7020_synth.log"
