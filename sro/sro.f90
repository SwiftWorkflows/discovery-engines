
module SRO

  type problem
     real*8  :: a_o1v1, a_o2v2, a_o1v2
     real*8  :: l, m, n
     real*8  :: h10, h20, h30 ! Lower boundary of h1,2,3
     real*8  :: h11, h21, h31 ! Upper boundary of h1,2,3
     integer :: h1n, h2n, h3n ! Discretization of h1,2,3
  end type

  real*8, parameter :: pi = 3.14159265359

  contains

    subroutine compute_I(p, mu1, mu2, mu12, intensity)

      type(problem), intent(in) :: p
      real*8, intent(in) :: mu1(p%h1n,p%h2n), &
                            mu2(p%h1n,p%h2n), &
                            mu12(p%h1n,p%h2n)
      real*8, intent(out) :: intensity(p%h1n,p%h2n,p%h3n)

      real*8  :: I1, I2, I12
      integer :: i, j, k
      real*8  :: h1step, h2step, h3step, h1, h2, h3, a

      h1step = (p%h11 - p%h10) / p%h1n
      h2step = (p%h21 - p%h20) / p%h2n
      h3step = (p%h31 - p%h10) / p%h3n

      do k = 1,p%h3n
         h3 = p%h30 + k * h3step
         do j = 1, p%h2n
            h2 = p%h20 + j * h2step
            do i = 1, p%h1n
               h1 = p%h10 + i * h1step
               a = h1*p%l + h2*p%m + h3*p%n
               I1  = p%a_o1v1 * cos(a)
               I2  = p%a_o2v2 * cos(a)
               I12 = p%a_o1v2 * cos(a)
               intensity(i,j,k) = mu1(i,j)  * I1 + &
                                  mu2(i,j)  * I2 + &
                                  mu12(i,j) * I12
            end do
         end do
      end do
    end subroutine

    subroutine compute_mu(p, mu1, mu2, mu12)

      type(problem), intent(in) :: p
      real*8, intent(out) :: mu1(p%h1n,p%h2n), &
                             mu2(p%h1n,p%h2n), &
                             mu12(p%h1n,p%h2n)

      real*8 :: foxy, falu, xO1, yO1, xT1, yT1, xTs1, yTs1
      real*8 :: xOs1, yOs1, xO2, yO2, xT2, yT2, xTs2, yTs2
      real*8 :: xOs2, yOs2
      real*8 :: FO1, FO2, FV1, FV2
      real*8  :: h1, h2, h1step, h2step
      integer i, j

      ! scattering factors for oxygen and aluminum,
      ! assuming we're sufficiently far from any absorption edges
      foxy =  8.00
      falu = 13.00

      xO1  = 0.50
      yO1  = 0.50
      xT1  = 0.1638
      yT1  = 0.1502
      xTs1 = 0.2943
      yTs1 = 0.2622
      xOs1 = 0.0502
      yOs1 = 0.0499

      h1step = (p%h11 - p%h10) / p%h1n
      h2step = (p%h21 - p%h20) / p%h2n

      do j = 1, p%h2n
         h2 = p%h20 + j * h2step
         do i = 1, p%h1n
            h1 = p%h10 + i * h1step

            FO1 = foxy +                                   &
                  2 * falu * cos(2*pi*(xT1 *h1+yT1 *h2))
            FV1 = 2 * foxy * cos(2*pi*(xOs1*h1+yOs1*h2)) + &
                  2 * falu * cos(2*pi*(xTs1*h1+yTs1*h2)) - &
                  2 * foxy * cos(2*pi*(xO1 *h1+yO1 *h2))
            mu1(i,j) = 0.8 * 0.2 * (FO1-FV1)**2
         end do
      end do

      ! fractional atomic coordinates of relevant sites
      ! relative to O2 (oxygen atom at (0.0,0.5,0.5)

      ! note that, as per Welberry and Butler's paper,
      ! I just inverted the x axis from the O1 positions

      xO2  =  xO1
      yO2  = -yO1
      xT2  =  xT2
      yT2  = -yT2
      xTs2 =  xTs1
      yTs2 = -yTs1
      xOs2 =  xOs1
      yOs2 = -yOs1

      do j = 1, p%h2n
         h2 = p%h20 + j * h2step
         do i = 1, p%h1n
            h1 = p%h10 + i * h1step

            FO2 = foxy +                                   &
                  2 * falu * cos(2*pi*(xT2 *H1+yT2 *H2))
            FV2 = 2 * foxy * cos(2*pi*(xOs2*H1+yOs2*H2)) + &
                  2 * falu * cos(2*pi*(xTs2*H1+yTs2*H2)) - &
                  2 * foxy * cos(2*pi*(xO2 *H1+yO2 *H2))

            mu2 (i,j) =     0.8 * 0.2 * (FO2-FV2)**2
            mu12(i,j) = 2 * 0.8 * 0.2 * (FO1-FV1)*(FO2-FV2)
         end do
      end do

   end subroutine compute_mu

end module
