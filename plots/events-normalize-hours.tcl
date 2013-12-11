#!/usr/bin/env tclsh

# Normalizes events to nearest hour starting with 0

if { $argc < 1 } {
    puts "requires events file!"
    exit 1
}

set filename [ lindex $argv 0 ]

set fd [ open $filename "r" ]

set hour 0
set run_start 0
set hour_start 0
set hour_previous 0

while { [ gets $fd line ] >= 0 } {

    set timestamp [ lindex $line 0 ]
    set payload   [ lreplace $line 0 0 ]

    if { $run_start == 0 } {
        set run_start $timestamp
    }
    set hour [ expr ( $timestamp-$run_start ) / 3600 ]
    puts "$hour $payload"
}
