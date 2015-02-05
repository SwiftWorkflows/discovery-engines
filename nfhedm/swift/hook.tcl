
set root /home/wozniak/data-root/2
set comm [ adlb::comm_leaders ]
if { [ adlb::rank $comm ] == 0 } {
    set top_files [ ::glob -type f $root/* ]
    set bin_files [ ::glob -type f $root/Au_Reduced2/* ]
}

turbine::c::bcast $comm 0 top_files
turbine::c::bcast $comm 0 bin_files

foreach f $top_files {
    if { [ adlb::rank $comm ] == 0 } {
        puts "copying: $f"
    }
    turbine::c::copy_to $comm $f /tmp
}

file mkdir /tmp/Au_Reduced2
foreach f $bin_files {
    turbine::c::copy_to $comm $f /tmp/Au_Reduced2
}

# puts [ ::glob /tmp/* ]
