#!/usr/bin/env tclsh

# Extract bandwidth or error rates from Globus Online events log

if { $argc < 1 } {
    puts "requires events file!"
    exit 1
}

set filename [ lindex $argv 0 ]

set fd [ open $filename "r" ]

# Unix time
set timestamp -1
set MBPS "0"
set code "none"

while { [ gets $fd line ] >= 0 } {

    if { [ string length $line ] == 0 } {
        if { [ string equal $code "SUCCEEDED" ] &&
             $MBPS > 0 } {
            puts "$timestamp $MBPS"
            continue
        }
        # Use this section if extracting error events:
        # if { [ string equal $code "ENDPOINT_TOO_BUSY" ] ||
        #      [ string equal $code "UNKNOWN" ] } {
        #     puts "$timestamp 100"
        # }
    }

    set token [ lindex $line 0 ]
    switch $token {
        "Time:" {
            set time_string [ lrange $line 1 2 ]
            set time_string [ string range $time_string 0 18 ]
            set timestamp [ clock scan $time_string \
                                -format {%Y-%m-%d %H:%M:%S} ]
        }
        "MBPS:" {
            set MBPS [ lindex $line 1 ]
            if [ string equal $MBPS "n/a" ] {
                set MBPS 0
            }
        }
        "Code:" {
            set code [ lindex $line 1 ]
        }
    }
}
