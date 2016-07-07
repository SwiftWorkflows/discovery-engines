
program main

  use SRO

  type(problem) :: p

  character (len=1024) output_file

  real*8, allocatable :: intensity(:,:,:)
  real*8, allocatable :: mu1(:,:), mu2(:,:), mu12(:,:)

  call scan_command_line(p%l, p%m, p%n, output_file)

  p%h10 = -5
  p%h20 = -5
  p%h30 = -5

  p%h11 = 5
  p%h21 = 5
  p%h31 = 5

  p%h1n = 1000
  p%h2n = 1000
  p%h3n = 10

  allocate(intensity(p%h1n, p%h2n, p%h3n))
  allocate(mu1 (p%h1n, p%h2n))
  allocate(mu2 (p%h1n, p%h2n))
  allocate(mu12(p%h1n, p%h2n))

  p%a_o1v1 = -0.25
  p%a_o2v2 = -0.25
  p%a_o1v2 = -0.2260175

  call compute_mu(p, mu1, mu2, mu12)
  ! call compute_I( p, mu1, mu2, mu12, intensity)

  call write_mu(p, mu1,  "mu1.txt")
  call write_mu(p, mu2,  "mu2.txt")
  call write_mu(p, mu12, "mu12.txt")

  call write_mu_hdf(p, mu1,  "mu1.h5")
  call write_mu_hdf(p, mu2,  "mu2.h5")
  call write_mu_hdf(p, mu12, "mu12.h5")

  ! call write_intensity_hdf(p, intensity, output_file)

  deallocate(intensity)
  deallocate(mu1)
  deallocate(mu2)
  deallocate(mu12)

end program

subroutine scan_command_line(l, m, n, output_file)

  real*8,            intent(out) :: l, m, n
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

subroutine write_mu_hdf(p, mu, output_file)

  use HDF5
  use SRO

  type(problem),     intent(in) :: p
  character (len=*), intent(in) :: output_file
  real*8,            intent(in) :: mu(p%h1n,p%h2n)

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
  call h5dcreate_f(file_id, hdf_path, H5T_IEEE_F64LE, space_id, &
                   dset_id, hdferr)
  call h5_error_check(hdferr)

  ! Write
  call h5dwrite_f(dset_id, H5T_IEEE_F64LE, mu, dims, hdferr)
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

subroutine write_intensity_hdf(p, intensity, output_file)

  use HDF5
  use SRO

  type(problem),     intent(in) :: p
  character (len=*), intent(in) :: output_file
  real*8,            intent(in) :: intensity(p%h1n,p%h2n,p%h3n)

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
  call h5dcreate_f(file_id, hdf_path, H5T_IEEE_F64LE, space_id, &
       dset_id, hdferr)
  call h5_error_check(hdferr)

  ! Write
  call h5dwrite_f(dset_id, H5T_IEEE_F64LE, intensity, dims, &
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

subroutine write_mu(p, mu, output_file)
  use SRO

  type(problem),     intent(in) :: p
  character (len=*), intent(in) :: output_file
  real*8,            intent(in) :: mu(p%h1n,p%h2n)

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

subroutine h5_error_check(hdferr)
  integer, intent(in) :: hdferr
  if (hdferr < 0) then
     write (*,*) 'Some HDF operation failed'
     call exit(1)
  end if
end subroutine

subroutine crash(message)
  character(len=*), intent(in) :: message
  write (*,*) trim(message)
  call exit(1)
end subroutine
