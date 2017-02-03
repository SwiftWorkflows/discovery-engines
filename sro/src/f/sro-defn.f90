
module SRO_DEFN

  type problem
     REAL    :: a_o1v1, a_o2v2, a_o1v2
     REAL    :: l, m, n
     REAL    :: h10, h20, h30 ! Lower boundary of h1,2,3
     REAL    :: h11, h21, h31 ! Upper boundary of h1,2,3
     integer :: h1n, h2n, h3n ! Discretization of h1,2,3
  end type

  REAL, parameter :: pi = 3.14159265359

contains

  subroutine crash(message)
    character(len=*), intent(in) :: message
    write (*,*) trim(message)
    call exit(1)
  end subroutine

end module
