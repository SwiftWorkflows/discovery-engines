#!/usr/bin/env tclsh

# Reduces the output of events-convert down to hourly averages

if { $argc < 1 } {
    puts "requires events file!"
    exit 1
}

set filename [ lindex $argv 0 ]

set fd [ open $filename "r" ]

# Unix time
set timestamp -1
set MBPS "0"

set hour 0
set run_start 0
set hour_start 0
set hour_previous 0
set values [ list ]

proc avg { L } {
    set result 0
    set N [ llength $L ]
    foreach v $L {
        set result [ expr $result + $v ]
    }
    set result [ expr double($result) / $N ]
    return $result
}

while { [ gets $fd line ] >= 0 } {

    set timestamp [ lindex $line 0 ]
    set MBPS [ lindex $line 1 ]

    if { $hour_start == 0 } {
        if { $run_start == 0 } {
            set run_start $timestamp
        }
        set hour_start $timestamp
        set hour [ expr ( $hour_start-$run_start ) / 3600 ]
        for { set i $hour_previous } { $i < $hour } { incr i } {
            puts "$i 0"
        }
    }

    lappend values $MBPS

    if { $timestamp > $hour_start + 3600 } {
        set rate [ avg $values ]
        puts "$hour $rate"
        set values [ list ]
        set hour_start 0
    }
}

set rate [ avg $values ]
puts "$hour $rate"
