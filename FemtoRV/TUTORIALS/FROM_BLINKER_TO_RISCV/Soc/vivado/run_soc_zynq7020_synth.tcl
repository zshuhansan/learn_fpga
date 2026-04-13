set script_dir [file normalize [file dirname [info script]]]
set soc_dir [file normalize [file join $script_dir ..]]
set repo_dir [file normalize [file join $soc_dir ..]]
set src_dir [file normalize [file join $soc_dir src]]
set tb_dir [file normalize [file join $soc_dir tb]]
set variant_name [expr {[info exists ::env(VIVADO_VARIANT)] && $::env(VIVADO_VARIANT) ne "" ? $::env(VIVADO_VARIANT) : ""}]
set variant_suffix [expr {$variant_name eq "" ? "" : "_$variant_name"}]
set project_name [format "soc_zynq7020_clg400_2%s" $variant_suffix]
set reports_dir [file normalize [file join $script_dir [format "reports_zynq7020_clg400_2%s" $variant_suffix]]]
set xdc_file [file normalize [file join $script_dir zynq7020_clg400_2_synth_only.xdc]]
set progrom_mem [file normalize [file join $repo_dir PROGROM.mem]]
set dataram_mem [file normalize [file join $repo_dir DATARAM.mem]]
set jobs [expr {[info exists ::env(VIVADO_JOBS)] ? $::env(VIVADO_JOBS) : 8}]
set enable_rv32m [expr {![info exists ::env(ENABLE_RV32M)] || $::env(ENABLE_RV32M) ne "0"}]

set_param general.maxThreads $jobs

file mkdir $reports_dir

set source_files [list \
    [file join $src_dir alu.v] \
    [file join $src_dir branch_predictor.v] \
    [file join $src_dir csr_unit.v] \
    [file join $src_dir decoder.v] \
    [file join $src_dir div_unit.v] \
    [file join $src_dir hazard_unit.v] \
    [file join $src_dir imm_gen.v] \
    [file join $src_dir lsu_align.v] \
    [file join $src_dir regfile.v] \
    [file join $src_dir Soc.v]]

set verilog_defines [list \
    BYPASS_ICACHE \
    VIVADO_XPM_MEMORY \
    CONFIG_PC_PREDICT \
    CONFIG_RAS \
    CONFIG_GSHARE \
    CONFIG_INITIALIZE \
    FAST_UART_STATUS \
    CPU_FREQ=100]

if {$enable_rv32m} {
    lappend verilog_defines CONFIG_RV32M
} else {
    lappend verilog_defines DISABLE_RV32M
}

create_project -in_memory -part xc7z020clg400-2
set_property target_language Verilog [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property XPM_LIBRARIES {XPM_MEMORY} [current_project]
set_property include_dirs [list $src_dir $repo_dir $tb_dir] [current_fileset]
set_property verilog_define $verilog_defines [current_fileset]

set mem_files [list]
if {[file exists $progrom_mem]} {
    lappend mem_files $progrom_mem
}
if {[file exists $dataram_mem]} {
    lappend mem_files $dataram_mem
}

if {[llength $mem_files] > 0} {
    read_mem $mem_files
}
cd $script_dir
read_verilog -library xil_defaultlib $source_files
read_xdc $xdc_file
set_param ips.enableIPCacheLiteLoad 1

synth_design -top Soc -part xc7z020clg400-2 -directive PerformanceOptimized -fsm_extraction one_hot -keep_equivalent_registers -resource_sharing off -no_lc -shreg_min_size 5

report_timing_summary -delay_type max -max_paths 20 -input_pins -file [file join $reports_dir post_synth_timing_summary.rpt]
report_utilization -hierarchical -file [file join $reports_dir post_synth_utilization_hier.rpt]
report_utilization -file [file join $reports_dir post_synth_utilization.rpt]
report_high_fanout_nets -max_nets 50 -fanout_greater_than 16 -file [file join $reports_dir post_synth_high_fanout.rpt]
report_control_sets -verbose -file [file join $reports_dir post_synth_control_sets.rpt]
write_checkpoint -force [file join $reports_dir [format "%s_post_synth.dcp" $project_name]]

set worst_setup_path [lindex [get_timing_paths -max_paths 1 -setup] 0]
set worst_hold_path [lindex [get_timing_paths -max_paths 1 -hold] 0]
set wns [expr {$worst_setup_path eq "" ? "" : [get_property SLACK $worst_setup_path]}]
set whs [expr {$worst_hold_path eq "" ? "" : [get_property SLACK $worst_hold_path]}]
puts "SYNTH_STATUS=complete"
puts "VIVADO_JOBS=$jobs"
puts "VIVADO_VARIANT=$variant_name"
puts "ENABLE_RV32M=$enable_rv32m"
puts "SYNTH_WNS=$wns"
puts "SYNTH_WHS=$whs"
puts "REPORT_DIR=$reports_dir"

close_project
exit
