
subroutine compute_I_sro(a_o1v1, a_o2v2, a_o1v2, &
                         h10, h20, h30, h11, h21, h31, h1n, h2n, h3n, &
                         intensity)

  real*8,  intent(in) :: a_o1v1, a_o2v2, a_o1v2
  real*8,  intent(in) :: h10, h20, h30 ! Lower boundary of h1,2,3
  real*8,  intent(in) :: h11, h21, h31 ! Upper boundary of h1,2,3
  integer, intent(in) :: h1n, h2n, h3n ! Discretization of h1,2,3

  real*8, intent(out) :: intensity(h1n, h2n, h3n)

  real*8 :: I1, I2, I12
  integer :: i, j, k
  integer :: l, m, n
  real*8 :: h1step, h2step, h3step, h1, h2, h3, a

  h1step = h10 + (h11-h10)/h1n
  h2step = h20 + (h21-h20)/h2n
  h3step = h30 + (h31-h10)/h3n

  do k = 1,h3n
     h3 = h30 + k * h3step
     do j = 1, h2n
        h2 = h20 + j * h2step
        do i = 1, h1n
           h1 = h10 + i * h1step
           a = h1*l + h2*m + h3*n
           I1  = a_o1v1 * cos(a)
           I2  = a_o2v2 * cos(a)
           I12 = a_o1v2 * cos(a)
           intensity(i,j,k) = I1 + I2 + I12
        end do
     end do
  end do
end subroutine
