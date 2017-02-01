
#include "defns.h"

module IO
contains

  subroutine intensity_hdf_read(p, input_file, intensity)

    use HDF5
    use SRO

    type(problem),     intent(in)  :: p
    character (len=*), intent(in)  :: input_file
    REAL,              intent(out) :: intensity(p%h1n,p%h2n,p%h3n)

    integer file_id,space_id, dset_id
    character (len=1024) :: hdf_path
    integer hdferr
    integer(hsize_t), dimension(1:3) :: dims

    print *, "reading from: ", trim(input_file)

    hdf_path = "intensity"
    dims(1) = p%h1n
    dims(2) = p%h2n
    dims(3) = p%h3n

    ! Open output file
    call h5open_f(hdferr)
    call h5_error_check(hdferr)
    call h5fcreate_f(input_file, H5F_ACC_TRUNC_F, file_id, hdferr, &
         H5P_DEFAULT_F, H5P_DEFAULT_F)
    call h5_error_check(hdferr)
    ! Create HDF path
    call h5screate_simple_f(3, dims, space_id, hdferr)
    call h5_error_check(hdferr)
    call h5dcreate_f(file_id, hdf_path, REAL_HDF, space_id, &
         dset_id, hdferr)
    call h5_error_check(hdferr)

    ! Read
    call h5dread_f(dset_id, REAL_HDF, intensity, dims, &
         hdferr)
    call h5_error_check(hdferr)

    ! Clean up
    call h5dclose_f(dset_id, hdferr)
    call h5_error_check(hdferr)
    call h5sclose_f(space_id, hdferr)
    call h5_error_check(hdferr)
    call h5fclose_f(file_id, hdferr)
    call h5_error_check(hdferr)

    print *, 'read done.'

  end subroutine

  subroutine intensity_hdf_write(p, intensity, output_file)

    use HDF5
    use SRO

    type(problem),     intent(in) :: p
    character (len=*), intent(in) :: output_file
    REAL,              intent(in) :: intensity(p%h1n,p%h2n,p%h3n)

    integer file_id,space_id, dset_id
    character (len=1024) :: hdf_path
    integer hdferr
    integer(hsize_t), dimension(1:3) :: dims

    print *, "writing to: ", trim(output_file)

    hdf_path = "intensity"
    dims(1) = p%h1n
    dims(2) = p%h2n
    dims(3) = p%h3n

    ! Open output file
    call h5open_f(hdferr)
    call h5_error_check(hdferr)
    call h5fcreate_f(output_file, H5F_ACC_TRUNC_F, file_id, hdferr, &
         H5P_DEFAULT_F, H5P_DEFAULT_F)
    call h5_error_check(hdferr)
    ! Create HDF path
    call h5screate_simple_f(3, dims, space_id, hdferr)
    call h5_error_check(hdferr)
    call h5dcreate_f(file_id, hdf_path, REAL_HDF, space_id, &
         dset_id, hdferr)
    call h5_error_check(hdferr)

    ! Write
    call h5dwrite_f(dset_id, REAL_HDF, intensity, dims, &
         hdferr)
    call h5_error_check(hdferr)

    ! Clean up
    call h5dclose_f(dset_id, hdferr)
    call h5_error_check(hdferr)
    call h5sclose_f(space_id, hdferr)
    call h5_error_check(hdferr)
    call h5fclose_f(file_id, hdferr)
    call h5_error_check(hdferr)

    print *, 'write done.'

  end subroutine

  subroutine mu_write(p, mu, output_file)
    use SRO

    type(problem),     intent(in) :: p
    character (len=*), intent(in) :: output_file
    REAL,            intent(in) :: mu(p%h1n,p%h2n)

    integer i, j

    open(unit=11, file=output_file)
    do i = 1, p%h1n
       do j = 1, p%h2n
          write (11,1100,advance="no") mu(i,j)
       end do
       write (11,*) ""
    end do
1100 format(F15.8)
    close(11)

  end subroutine

  subroutine mu_hdf_write(p, mu, output_file)

    use HDF5
    use SRO

    type(problem),     intent(in) :: p
    character (len=*), intent(in) :: output_file
    REAL,              intent(in) :: mu(p%h1n,p%h2n)

    integer file_id, space_id, dset_id
    character (len=1024) :: hdf_path
    integer hdferr
    integer(hsize_t), dimension(1:2) :: dims

    print *, "writing to: ", trim(output_file)

    hdf_path = "mu"
    dims(1) = p%h1n
    dims(2) = p%h2n

    ! Open output file
    call h5open_f(hdferr)
    call h5_error_check(hdferr)
    call h5fcreate_f(output_file, H5F_ACC_TRUNC_F, file_id, hdferr, &
         H5P_DEFAULT_F, H5P_DEFAULT_F)
    call h5_error_check(hdferr)
    ! Create HDF path
    call h5screate_simple_f(2, dims, space_id, hdferr)
    call h5_error_check(hdferr)
    call h5dcreate_f(file_id, hdf_path, REAL_HDF, space_id, &
         dset_id, hdferr)
    call h5_error_check(hdferr)



    ! Write
    call h5dwrite_f(dset_id, REAL_HDF, mu, dims, hdferr)
    call h5_error_check(hdferr)

    ! Clean up
    call h5dclose_f(dset_id, hdferr)
    call h5_error_check(hdferr)
    call h5sclose_f(space_id, hdferr)
    call h5_error_check(hdferr)
    call h5fclose_f(file_id, hdferr)
    call h5_error_check(hdferr)

    print *, 'write done.'

  end subroutine

  subroutine h5_error_check(hdferr)
    integer, intent(in) :: hdferr
    write (*,*) "error check: ", hdferr
    if (hdferr < 0) then
       write (*,*) "Some HDF operation failed"
       call exit(1)
    end if
  end subroutine

end module
