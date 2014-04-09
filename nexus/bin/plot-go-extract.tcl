#!/usr/bin/env tclsh

# Extract file stats after GO xfer

exec find . > ~/.files.txt

set timestamps [ list ]
set bytes [ dict create ]

set fd [ open "~/.files.txt" "r" ]
while { [ gets $fd name ] >= 0 } {
    file stat $name S
    if { ! [ string equal $S(type) "file" ] } continue
    lappend timestamps $S(mtime)
    dict incr bytes $S(mtime) $S(size)
}
close $fd

set timestamps [ lsort -unique -integer $timestamps ]

# puts $timestamps

foreach t $timestamps {
    set s [ dict get $bytes $t ]
    puts "$t $s"
}


