
lappend auto_path $env(PWD)

package require tclnfhedm

tclfop::fop_sanity

set z [ placebo_1 1.5 2.5 ]
puts "test-placebo.tcl: z=$z"


