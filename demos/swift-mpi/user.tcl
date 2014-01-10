
package require turbine 0.4.0

namespace eval user {

    proc mpi_merge_tcl { args } {

        set result [ lindex $args 0 ]
        set inputs [ lindex $args 1 ]
        set opts   [ lrange $args 2 end ]
        turbine::rule $inputs "user::mpi_merge_tcl_body $result $inputs" \
            type $turbine::WORK {*}$opts
    }

    proc mpi_merge_tcl_body { result filename } {

        set filename_value [ turbine::retrieve_string $filename ]

        if { [ turbine::c::task_rank ] == 0 } {
            set size [ turbine::c::task_size ]
        }
        set result_value [ mpi_merge [turbine::c::task_comm] $filename_value ]

        if { [ turbine::c::task_rank ] == 0 } {
            turbine::store_float $result $result_value
        }
    }
}
