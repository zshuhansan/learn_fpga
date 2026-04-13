set script_dir [file normalize [file dirname [info script]]]
set variant_name [expr {[info exists ::env(VIVADO_VARIANT)] && $::env(VIVADO_VARIANT) ne "" ? $::env(VIVADO_VARIANT) : ""}]
set variant_suffix [expr {$variant_name eq "" ? "" : "_$variant_name"}]
set project_name [format "soc_zynq7020_clg400_2%s" $variant_suffix]
set reports_dir [file normalize [file join $script_dir [format "reports_zynq7020_clg400_2%s" $variant_suffix]]]
set synth_dcp [file normalize [file join $reports_dir [format "%s_post_synth.dcp" $project_name]]]
set jobs [expr {[info exists ::env(VIVADO_JOBS)] ? $::env(VIVADO_JOBS) : 8}]
set enable_post_route_physopt [expr {[info exists ::env(ENABLE_POST_ROUTE_PHYSOPT)] && $::env(ENABLE_POST_ROUTE_PHYSOPT) ne "0"}]

if {![file exists $synth_dcp]} {
    puts stderr "Synthesized checkpoint not found: $synth_dcp"
    exit 1
}

set_param general.maxThreads $jobs

file mkdir $reports_dir

open_checkpoint $synth_dcp
opt_design
place_design
phys_opt_design -directive AggressiveFanoutOpt
route_design -directive Explore
set post_route_physopt_status skipped
if {$enable_post_route_physopt} {
    phys_opt_design -directive AggressiveExplore
    set post_route_physopt_status complete
}

report_timing_summary -delay_type max -max_paths 20 -input_pins -file [file join $reports_dir post_route_timing_summary.rpt]
report_utilization -hierarchical -file [file join $reports_dir post_route_utilization_hier.rpt]
report_utilization -file [file join $reports_dir post_route_utilization.rpt]
report_high_fanout_nets -max_nets 50 -fanout_greater_than 16 -file [file join $reports_dir post_route_high_fanout.rpt]
report_clock_utilization -file [file join $reports_dir post_route_clock_utilization.rpt]
report_route_status -file [file join $reports_dir post_route_route_status.rpt]
report_design_analysis -congestion -file [file join $reports_dir post_route_congestion.rpt]
report_drc -file [file join $reports_dir post_route_drc.rpt]
write_checkpoint -force [file join $reports_dir [format "%s_post_route.dcp" $project_name]]

set worst_setup_path [lindex [get_timing_paths -max_paths 1 -setup] 0]
set worst_hold_path [lindex [get_timing_paths -max_paths 1 -hold] 0]
set wns [expr {$worst_setup_path eq "" ? "" : [get_property SLACK $worst_setup_path]}]
set whs [expr {$worst_hold_path eq "" ? "" : [get_property SLACK $worst_hold_path]}]

puts "IMPL_STATUS=complete"
puts "VIVADO_JOBS=$jobs"
puts "VIVADO_VARIANT=$variant_name"
puts "POST_ROUTE_PHYSOPT_STATUS=$post_route_physopt_status"
puts "IMPL_WNS=$wns"
puts "IMPL_WHS=$whs"
puts "REPORT_DIR=$reports_dir"

close_design
exit