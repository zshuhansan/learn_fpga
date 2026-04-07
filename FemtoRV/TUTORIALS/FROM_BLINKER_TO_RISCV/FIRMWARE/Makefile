include ../../../FIRMWARE/makefile.inc

EXTERNAL_RV_PREFIX ?=
ifneq ($(EXTERNAL_RV_PREFIX),)
RVTOOLCHAIN_BIN_PREFIX=$(EXTERNAL_RV_PREFIX)
RVAS=$(RVTOOLCHAIN_BIN_PREFIX)-as
RVLD=$(RVTOOLCHAIN_BIN_PREFIX)-ld
RVOBJCOPY=$(RVTOOLCHAIN_BIN_PREFIX)-objcopy
RVOBJDUMP=$(RVTOOLCHAIN_BIN_PREFIX)-objdump
RVGCC=$(RVTOOLCHAIN_BIN_PREFIX)-gcc
RVGPP=$(RVTOOLCHAIN_BIN_PREFIX)-g++
RVAR=$(RVTOOLCHAIN_BIN_PREFIX)-ar
RVRANLIB=$(RVTOOLCHAIN_BIN_PREFIX)-ranlib
RVTOOLCHAIN_LIB_DIR=$(dir $(shell $(RVGCC) -print-file-name=libc.a))
RVTOOLCHAIN_GCC_LIB_DIR=$(dir $(shell $(RVGCC) -print-libgcc-file-name))
RV_BINARIES=$(RVAS) $(RVLD) $(RVOBJCOPY) $(RVOBJDUMP) $(RVGCC) $(RVAR) $(RVRANLIB)
endif

ifeq ($(filter get_riscv_toolchain,$(MAKECMDGOALS)),get_riscv_toolchain)
.PHONY: get_riscv_toolchain
get_riscv_toolchain:
	@mkdir -p $(FIRMWARE_DIR)/TOOLCHAIN
	@if [ -x "$(RVGCC)" ]; then \
		echo "Using existing toolchain $(RVGCC)"; \
	else \
		echo "Installing RISC-V toolchain for $(uname) $(uname_m) in $(FIRMWARE_DIR)/TOOLCHAIN"; \
		cd $(FIRMWARE_DIR)/TOOLCHAIN; \
		if [ ! -f "$(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX).tar.gz" ]; then \
			wget $(TOOLCHAIN_WEB)/$(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX).tar.gz -O $(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX).tar.gz; \
		fi; \
		if [ ! -d "$(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX)" ]; then \
			tar xfz $(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX).tar.gz; \
			if [ -d $(TOOLCHAIN_VER) ]; then mv $(TOOLCHAIN_VER) $(TOOLCHAIN_VER)$(TOOLCHAIN_DL_SUFFIX); fi; \
		fi; \
	fi
endif
RVASFLAGS=-march=$(ARCH) -mabi=$(ABI) 
RVCFLAGS=-I. -O2 -fno-pic -march=$(ARCH) -mabi=$(ABI) -fno-stack-protector -w -Wl,--no-relax

RAM_SIZE=6144

LIBOBJECTS=putchar.o wait.o print.o memcpy.o errno.o perf.o

COREMARK_OBJECTS=core_main.o core_list_join.o core_matrix.o core_state.o core_util.o core_portme.o ee_printf.o

$(COREMARK_OBJECTS): %.o: COREMARK/%.c $(RV_BINARIES)
	$(RVGCC) $(RVCFLAGS) -c $< -o $@

%.bram.elf: %.o start.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T bram.ld -m elf32lriscv -nostdlib -norelax $< $(LIBOBJECTS) $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a -o $@

coremark.spiflash1.elf: $(COREMARK_OBJECTS) start_spiflash1.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T spiflash1.ld -m elf32lriscv -nostdlib -norelax $(COREMARK_OBJECTS) start_spiflash1.o $(LIBOBJECTS) $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a -o $@

coremark.pipeline.elf: $(COREMARK_OBJECTS) start_pipeline.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T pipeline.ld -m elf32lriscv -nostdlib -norelax $(COREMARK_OBJECTS) start_pipeline.o $(LIBOBJECTS) -L$(RVTOOLCHAIN_LIB_DIR) -lm $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a  -o $@
	$(RVOBJDUMP) -Mnumeric -D $@ > $@.list

coremark.pipeline.hex: coremark.PROGROM.hex coremark.DATARAM.hex
	echo $@ > ../firmware.txt


%.hex: %.elf $(FIRMWARE_DIR)/TOOLS/firmware_words 
	$(FIRMWARE_DIR)/TOOLS/firmware_words $< -ram $(RAM_SIZE) -max_addr $(RAM_SIZE) -out $@
	cp $@ ../firmware.hex
	mkdir -p ../obj_dir
	cp $@ ../obj_dir/firmware.hex
	echo $@ > ../firmware.txt


# SPI FLASH 0 (sends everything to SPI flash)

%.spiflash0.elf: %.o start.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T spiflash0.ld -m elf32lriscv -nostdlib -norelax $< $(LIBOBJECTS) $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a -o $@

%.spiflash0.bin: %.spiflash0.elf
	$(RVOBJCOPY) $< $@ -O binary

%.spiflash0.prog: %.spiflash0.bin
	iceprog -o 128k $<

# SPI FLASH 1 (sends code and variables initialization to SPI flash, variables to RAM)

%.spiflash1.elf: %.o start_spiflash1.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T spiflash1.ld -m elf32lriscv -nostdlib -norelax $< $(LIBOBJECTS) $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a -o $@


%.spiflash1.bin: %.spiflash1.elf
	$(RVOBJCOPY) $< $@ -O binary

%.spiflash1.prog: %.spiflash1.bin
	iceprog -o 128k $<


# SPI FLASH 2 (sends code and variables initialization to SPI flash, variables and fastcode to RAM)

%.spiflash2.elf: %.o start_spiflash1.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T spiflash2.ld -m elf32lriscv -nostdlib -norelax $< $(LIBOBJECTS) -L$(RVTOOLCHAIN_LIB_DIR) -lm  $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a -o $@


%.spiflash2.bin: %.spiflash2.elf
	$(RVOBJCOPY) $< $@ -O binary

%.spiflash2.prog: %.spiflash2.bin
	iceprog -o 128k $<

%.spiflash2.list: %.spiflash2.elf
	$(RVOBJDUMP) -Mnumeric -D $< > $@

# DUAL MEMORY (64 kb program ROM, 64 kb data RAM)

%.pipeline.elf: %.o start_pipeline.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T pipeline.ld -m elf32lriscv -nostdlib -norelax $< $(LIBOBJECTS) -L$(RVTOOLCHAIN_LIB_DIR) -lm $(RVTOOLCHAIN_GCC_LIB_DIR)/libgcc.a  -o $@
	$(RVOBJDUMP) -Mnumeric -D $@ > $@.list

%.PROGROM.hex: %.pipeline.elf $(FIRMWARE_DIR)/TOOLS/firmware_words 
	$(FIRMWARE_DIR)/TOOLS/firmware_words $< -ram 0x20000 -max_addr 0x20000 -out $@ -from_addr 0 -to_addr 0xFFFF
	cp $@ ../PROGROM.hex
	mkdir -p ../obj_dir
	cp $@ ../obj_dir/PROGROM.hex

%.DATARAM.hex: %.pipeline.elf $(FIRMWARE_DIR)/TOOLS/firmware_words 
	$(FIRMWARE_DIR)/TOOLS/firmware_words $< -ram 0x20000 -max_addr 0x20000 -out $@ -from_addr 0x10000 -to_addr 0x1FFFF
	cp $@ ../DATARAM.hex
	mkdir -p ../obj_dir
	cp $@ ../obj_dir/DATARAM.hex

%.pipeline.hex: %.PROGROM.hex %.DATARAM.hex
	echo $@ > ../firmware.txt
