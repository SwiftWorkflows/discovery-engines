
program test_read

  use IO
  use SRO

  integer status, required_arguments
  character (len=32) s
  character (len=32) message

  type(problem) :: p
  character (len=128) input_file

  REAL, allocatable :: intensity(:,:,:)

  print *, "test-read"

  required_arguments = 4
  if (command_argument_count() /= required_arguments) then
     write(message, '(A,I2)') &
           'Required arguments:', required_arguments
     call crash(message)
  endif

  call get_command_argument(1, s)
  read(s, *, iostat=status) p%l
  if (status > 0) then
     call crash("Bad argument for l: " // s)
  endif
  call get_command_argument(2, s)
  read(s, *, iostat=status) p%m
  if (status > 0) then
     call crash("Bad argument for m: " // s)
  endif
  call get_command_argument(3, s)
  read(s, *, iostat=status) p%n
  if (status > 0) then
     call crash("Bad argument for n: " // s)
  endif

  call get_command_argument(4, input_file)

  print *, "reading: ", input_file

  allocate(intensity(p%h1n, p%h2n, p%h3n))

  call intensity_hdf_read(p, input_file, intensity)


end program
