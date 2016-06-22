
program sro

  real*8 :: l, m, n

  call scan_command_line(l, m, n)

  print '(A,3F6.1)', 'l,m,n = ', l, m, n

end program

subroutine scan_command_line(l, m, n)

  real*8, intent(out) :: l, m, n
  integer status
  character (len=32) s

  if (command_argument_count() /= 3) then
     call crash("Requires 3 arguments!")
  endif

  call get_command_argument(1, s)
  read(s, *, iostat=status) l
  if (status > 0) then
     call crash("Bad argument for l: " // s)
  endif
  call get_command_argument(2, s)
  read(s,  *, iostat=status) m
  if (status > 0) then
     call crash("Bad argument for m: " // s)
  endif
  call get_command_argument(3, s)
  read(s, *, iostat=status) n
  if (status > 0) then
     call crash("Bad argument for n: " // s)
  endif

end subroutine

subroutine crash(message)
  character(len=*) :: message
  write (*,*) trim(message)
  call exit(1)
end subroutine
