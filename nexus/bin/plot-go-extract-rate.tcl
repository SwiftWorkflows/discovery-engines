#!/usr/bin/env tclsh

# Extract file xfer rate per hour from plot-extract data

if { $argc < 1 } {
    puts "requires input file name!"
    exit 1
}

set data [ lindex $argv 0 ]

set hour 0
set start_run -1
set start_hour -1

set chunk_bytes 0

set fd [ open $data "r" ]
while { [ gets $fd line ] >= 0 } {
    set timestamp [ lindex $line 0 ]
    set bytes [ lindex $line 1 ]
    if { $start_run == -1 } {
        set start_run $timestamp
    }
    if { $start_hour == -1 } {
        set start_hour $timestamp
        set chunk_bytes 0
    }
    incr chunk_bytes $bytes
    if { $timestamp > $start_hour + 3600 } {

        set rate [ expr double($chunk_bytes)/3600 ]
        puts "$hour $rate"
        incr hour
        set start_hour -1
    }
}

