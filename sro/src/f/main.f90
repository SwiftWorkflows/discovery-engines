
#include "cfg.h"

program main

  use IO
  use SRO_CALC
  use SRO_DEFN

  type(problem) :: p

  character (len=1024) output_file

  REAL, allocatable :: mu1(:,:), mu2(:,:), mu12(:,:)

  call scan_command_line(p%l, p%m, p%n, output_file)

  p%h10 = -5
  p%h20 = -5
  p%h30 = -5

  p%h11 = 5
  p%h21 = 5
  p%h31 = 5

  p%h1n = 10
  p%h2n = 10
  p%h3n = 10

  allocate(mu1 (p%h1n, p%h2n))
  allocate(mu2 (p%h1n, p%h2n))
  allocate(mu12(p%h1n, p%h2n))

  p%a_o1v1 = -0.25
  p%a_o2v2 = -0.25
  p%a_o1v2 = -0.2260175

  call compute_mu(p, mu1, mu2, mu12)
  call allocate_theory(p)
  call compute_theory(p, mu1, mu2, mu12)

  ! call mu_write(p, mu1,  "mu1.txt")
  ! call mu_write(p, mu2,  "mu2.txt")
  ! call mu_write(p, mu12, "mu12.txt")

  ! call mu_hdf_write(p, mu1,  "mu1.h5")
  ! call mu_hdf_write(p, mu2,  "mu2.h5")
  ! call mu_hdf_write(p, mu12, "mu12.h5")

  call intensity_hdf_write(p, theory, output_file)

  deallocate(mu1)
  deallocate(mu2)
  deallocate(mu12)

end program

subroutine scan_command_line(l, m, n, output_file)

  use SRO_DEFN

  REAL,              intent(out) :: l, m, n
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
