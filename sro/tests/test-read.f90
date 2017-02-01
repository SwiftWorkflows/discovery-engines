
program test_read

  use IO
  use SRO

  integer required_arguments
  character (len=32) message

  type(problem) :: p
  character (len=128) input_file

  REAL, allocatable :: intensity(:,:,:)

  print *, "test-read"

  required_arguments = 1
  if (command_argument_count() /= required_arguments) then
     write(message, '(A,I2)') &
           'Required arguments:', required_arguments
     call crash(message)
  endif

  p%h1n = 801
  p%h2n = 901
  p%h3n = 100

  call get_command_argument(1, input_file)

  print *, "allocating: ", p%h1n*p%h2n*p%h3n*8/(1024*1024)

  allocate(intensity(p%h1n, p%h2n, p%h3n))

  call intensity_hdf_read(p, input_file, intensity)

end program
