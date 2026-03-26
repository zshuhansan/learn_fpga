#!/usr/bin/env bash
set -euo pipefail

ROOT="/home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV"
FW="$ROOT/FIRMWARE"
TOOLS_WORDS="/home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/TOOLS/firmware_words"
RVTESTS="$ROOT/riscv-tests"
ENVH="$ROOT/ROUTE_A/riscv_test_env"
MACROS="$RVTESTS/isa/macros/scalar"
OUTDIR="$ROOT/ROUTE_A/riscv_test_build"
LOGDIR="$ROOT/ROUTE_A/riscv_test_logs"

mkdir -p "$OUTDIR" "$LOGDIR"

if [[ ! -f "$OUTDIR/start_pipeline_dummy.S" ]]; then
cat >"$OUTDIR/start_pipeline_dummy.S" <<'EOF'
    .section .text
    .globl __routea_dummy_start
__routea_dummy_start:
    nop
EOF
fi

if [[ ! -d "$RVTESTS" ]]; then
  echo "riscv-tests not found at $RVTESTS"
  exit 1
fi

RVGCC=$(find /home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/TOOLCHAIN -name riscv64-unknown-elf-gcc | head -n 1)
RVLD=$(find /home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/TOOLCHAIN -name riscv64-unknown-elf-ld | head -n 1)
if [[ -z "${RVGCC:-}" || -z "${RVLD:-}" ]]; then
  echo "RISC-V toolchain not found, run: make -C $FW get_riscv_toolchain"
  exit 1
fi

"$RVGCC" -march=rv32im -mabi=ilp32 -c "$OUTDIR/start_pipeline_dummy.S" -o "$OUTDIR/start_pipeline.o"

TESTS=(
  rv32ui/add
  rv32ui/addi
  rv32ui/and
  rv32ui/andi
  rv32ui/or
  rv32ui/ori
  rv32ui/xor
  rv32ui/xori
  rv32ui/sll
  rv32ui/slli
  rv32ui/srl
  rv32ui/srli
  rv32ui/sra
  rv32ui/srai
  rv32ui/slt
  rv32ui/slti
  rv32ui/sltiu
  rv32ui/sltu
  rv32ui/lui
  rv32ui/auipc
  rv32ui/beq
  rv32ui/bne
  rv32ui/blt
  rv32ui/bge
  rv32ui/bltu
  rv32ui/bgeu
  rv32ui/jal
  rv32ui/jalr
  rv32ui/lb
  rv32ui/lh
  rv32ui/lw
  rv32ui/lbu
  rv32ui/lhu
  rv32ui/sb
  rv32ui/sh
  rv32ui/sw
  rv32um/mul
  rv32um/mulh
  rv32um/mulhsu
  rv32um/mulhu
  rv32um/div
  rv32um/divu
  rv32um/rem
  rv32um/remu
)

pass=0
fail=0

for t in "${TESTS[@]}"; do
  name="${t//\//_}"
  src="$RVTESTS/isa/$t.S"
  obj="$OUTDIR/$name.o"
  elf="$OUTDIR/$name.elf"
  log="$LOGDIR/$name.log"

  echo "==> $t"
  "$RVGCC" -march=rv32im -mabi=ilp32 -nostdlib -ffreestanding -fno-pic \
    -I"$ENVH" -I"$MACROS" -I"$RVTESTS/env" \
    -c "$src" -o "$obj"

  (cd "$OUTDIR" && "$RVLD" -T "$FW/pipeline.ld" -m elf32lriscv -nostdlib -norelax start_pipeline.o "$name.o" -o "$name.elf")

  "$TOOLS_WORDS" "$elf" -ram 0x20000 -max_addr 0x20000 -out "$OUTDIR/$name.PROGROM.hex" -from_addr 0 -to_addr 0xFFFF >/dev/null
  "$TOOLS_WORDS" "$elf" -ram 0x20000 -max_addr 0x20000 -out "$OUTDIR/$name.DATARAM.hex" -from_addr 0x10000 -to_addr 0x1FFFF >/dev/null

  cp "$OUTDIR/$name.PROGROM.hex" "$ROOT/PROGROM.hex"
  cp "$OUTDIR/$name.DATARAM.hex" "$ROOT/DATARAM.hex"

  (cd "$ROOT" && ./run_verilator.sh Soc.v) >"$log" 2>&1 || true

  if grep -q "P" "$log"; then
    echo "PASS $t"
    pass=$((pass+1))
  else
    echo "FAIL $t"
    fail=$((fail+1))
  fi
done

echo "=============================="
echo "riscv-tests summary: PASS=$pass FAIL=$fail"
echo "logs: $LOGDIR"

if [[ $fail -ne 0 ]]; then
  exit 2
fi
