
program main

  use SRO

  type(problem) :: p

  character (len=1024) output_file

  real*8, allocatable :: intensity(:,:,:)

  call scan_command_line(p%l, p%m, p%n, output_file)

  p%h10 = -10
  p%h20 = -10
  p%h30 = -10

  p%h11 = 10
  p%h21 = 10
  p%h31 = 10

  p%h1n = 100
  p%h2n = 100
  p%h3n = 100

  allocate(intensity(p%h1n, p%h2n, p%h3n))

  p%a_o1v1 = 1
  p%a_o2v2 = 1
  p%a_o1v2 = 1

  p%mu1 = 1
  p%mu2 = 1
  p%mu3 = 1

  call compute_I(p, intensity)

  call write_intensity(p, intensity, output_file)

  deallocate(intensity)

end program

subroutine scan_command_line(l, m, n, output_file)

  real*8, intent(out) :: l, m, n
  character (len=*), intent(out) :: output_file

  integer status, required_arguments
  character (len=32) s
  character (len=32) message

  required_arguments = 4
  if (command_argument_count() /= required_arguments) then
     write(message, '(A,I2)') &
          'Required arguments:', required_arguments
     call crash(message)
  endif

  call get_command_argument(1, s)
  read(s, *, iostat=status) l
  if (status > 0) then
     call crash("Bad argument for l: " // s)
  endif
  call get_command_argument(2, s)
  read(s, *, iostat=status) m
  if (status > 0) then
     call crash("Bad argument for m: " // s)
  endif
  call get_command_argument(3, s)
  read(s, *, iostat=status) n
  if (status > 0) then
     call crash("Bad argument for n: " // s)
  endif

  call get_command_argument(4, output_file)

  print '(A,3F6.1)', 'l,m,n = ', l, m, n

end subroutine

subroutine write_intensity(p, intensity, output_file)

  use HDF5
  use SRO

  type(problem), intent(in) :: p
  character (len=*), intent(in) :: output_file
  real*8 :: intensity(p%h1n,p%h2n,p%h3n)

  integer file_id,space_id,dset_id
  character (len=1024) :: hdf_path
  integer hdferr             ! Error code for HDF5
  integer(hsize_t), dimension(1:3) :: dims

  print *, "writing to: ", trim(output_file)
  print *, p%h1n, p%h2n, p%h3n

  hdf_path = "intensity"
  dims(1) = p%h1n
  dims(2) = p%h2n
  dims(3) = p%h3n

  print *, intensity(1,1,1)

  ! Open output file
  call h5open_f(hdferr)
  call h5_error_check(hdferr)
  call h5fcreate_f(output_file, H5F_ACC_TRUNC_F, file_id, hdferr, &
       H5P_DEFAULT_F, H5P_DEFAULT_F)
  call h5_error_check(hdferr)
  ! Create HDF path
  call h5screate_simple_f(3, dims, space_id, hdferr)
  call h5_error_check(hdferr)
  call h5dcreate_f(file_id, hdf_path, H5T_IEEE_F64LE, space_id, &
       dset_id, hdferr)
  call h5_error_check(hdferr)

  call h5dwrite_f(dset_id, H5T_IEEE_F64LE, intensity, dims, &
       hdferr)
  call h5_error_check(hdferr)
  print *, 'write done.'

end subroutine

subroutine h5_error_check(hdferr)
  integer hdferr
  if (hdferr < 0) then
     write (*,*) 'Some HDF operation failed'
     call exit(1)
  end if
end subroutine

subroutine crash(message)
  character(len=*) :: message
  write (*,*) trim(message)
  call exit(1)
end subroutine
