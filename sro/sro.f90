
module SRO
  type problem
     real*8  :: a_o1v1, a_o2v2, a_o1v2
     real*8  :: mu1, mu2, mu3
     real*8  :: l, m, n
     real*8  :: h10, h20, h30 ! Lower boundary of h1,2,3
     real*8  :: h11, h21, h31 ! Upper boundary of h1,2,3
     integer :: h1n, h2n, h3n ! Discretization of h1,2,3
  end type

  contains

    subroutine compute_I(p, intensity)

      type(problem) :: p
      real*8 :: intensity(p%h1n,p%h2n,p%h3n)
      ! Locals
      real*8  :: I1, I2, I12
      integer :: i, j, k
      real*8  :: h1step, h2step, h3step, h1, h2, h3, a

      h1step = (p%h11 - p%h10) / p%h1n
      h2step = (p%h21 - p%h20) / p%h2n
      h3step = (p%h31 - p%h10) / p%h3n

      ! print *, h1step, h2step, h3step

      do k = 1,p%h3n
         h3 = p%h30 + k * h3step
         ! print *,  h3
         do j = 1, p%h2n
            h2 = p%h20 + j * h2step
            do i = 1, p%h1n
               h1 = p%h10 + i * h1step
               a = h1*p%l + h2*p%m + h3*p%n
               I1  = p%a_o1v1 * cos(a)
               I2  = p%a_o2v2 * cos(a)
               I12 = p%a_o1v2 * cos(a)
               intensity(i,j,k) = p%mu1*I1 + p%mu2*I2 + p%mu3*I12
            end do
         end do
      end do
    end subroutine

  end module
