
#include "cfg.h"

module SRO_CALC

  REAL, allocatable :: exprmnt(:,:,:)
  REAL, allocatable :: theory(:,:,:)

contains

  subroutine allocate_theory(p)

    use SRO_DEFN

    type(problem) :: p

    allocate(theory(p%h1n, p%h2n, p%h3n))

  end subroutine allocate_theory

  subroutine exprmnt_setup(p, exprmnt_file)

    use SRO_DEFN
    use IO

    type(problem) :: p
    character (len=FILENAME_MAX) exprmnt_file

    if (allocated(exprmnt)) then
       print *, 'already allocated!'
       return
    end if

    p%h1n = 10
    p%h2n = 10
    p%h3n = 10

    print *, "allocating: ", p%h1n*p%h2n*p%h3n*8/(1024*1024), "MB"

    allocate(exprmnt(p%h1n, p%h2n, p%h3n))
    call allocate_theory(p)

    call exprmnt_hdf_read(p, exprmnt_file, exprmnt)

    ! print *, exprmnt(1,1,1)
    ! print *, exprmnt(10,10,10)

  end subroutine

  function compute_diff(p, exprmnt_file) result(c)

    use SRO_DEFN

    type(problem), intent(in) :: p
    REAL :: mu1(p%h1n,p%h2n), &
            mu2(p%h1n,p%h2n), &
            mu12(p%h1n,p%h2n)

    integer i, j, k
    REAL :: c

    character (len=FILENAME_MAX) exprmnt_file

    call exprmnt_setup( p, exprmnt_file)
    call compute_mu(    p, mu1, mu2, mu12)
    call compute_theory(p, mu1, mu2, mu12)

    c = 0
    do k=1,p%h3n
       do j=1,p%h2n
          do i=1,p%h1n
             c = c + (theory(i,j,k) - exprmnt(i,j,k)) ** 2
          end do
       end do
    end do

  end function

  subroutine compute_theory(p, mu1, mu2, mu12)

    use SRO_DEFN

    type(problem), intent(in) :: p
    REAL, intent(in) :: mu1(p%h1n,p%h2n), &
                        mu2(p%h1n,p%h2n), &
                        mu12(p%h1n,p%h2n)

    REAL    :: I1, I2, I12
    integer :: i, j, k
    REAL    :: h1step, h2step, h3step, h1, h2, h3, a

    print *, "compute_theory()..."

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
             theory(i,j,k) = mu1(i,j)  * I1 + &
                  mu2(i,j)  * I2 + &
                  mu12(i,j) * I12
          end do
       end do
    end do

    print *, "compute_theory() done."

  end subroutine

  subroutine compute_mu(p, mu1, mu2, mu12)

    use SRO_DEFN

    type(problem), intent(in) :: p
    REAL, intent(out) :: mu1(p%h1n,p%h2n), &
                         mu2(p%h1n,p%h2n), &
                         mu12(p%h1n,p%h2n)

    REAL :: foxy, falu, xO1, yO1, xT1, yT1, xTs1, yTs1
    REAL :: xOs1, yOs1, xO2, yO2, xT2, yT2, xTs2, yTs2
    REAL :: xOs2, yOs2
    REAL :: FO1, FO2, FV1, FV2
    REAL :: h1, h2, h1step, h2step
    integer i, j

    print *, "compute_mu()..."

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

    ! fractional atomic coordinates of relevant sites
    ! relative to O2 (oxygen atom at (0.0,0.5,0.5)

    ! note that, as per Welberry and Butler's paper,
    ! I just inverted the x axis from the O1 positions

    xO2  =  xO1
    yO2  = -yO1
    xT2  =  xT1
    yT2  = -yT1
    xTs2 =  xTs1
    yTs2 = -yTs1
    xOs2 =  xOs1
    yOs2 = -yOs1

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

          FO2 = foxy +                                   &
                2 * falu * cos(2*pi*(xT2 *h1+yT2 *h2))
          FV2 = 2 * foxy * cos(2*pi*(xOs2*h1+yOs2*h2)) + &
                2 * falu * cos(2*pi*(xTs2*h1+yTs2*h2)) - &
                2 * foxy * cos(2*pi*(xO2 *h1+yO2 *h2))

          mu2 (i,j) =     0.8 * 0.2 * (FO2-FV2)**2
          mu12(i,j) = 2 * 0.8 * 0.2 * (FO1-FV1)*(FO2-FV2)

       end do
    end do

    print *, "compute_mu() done."

  end subroutine

  subroutine test_c()
    print *, "Fortran OK"
  end subroutine

  function problem_make_c() result(p)

    use SRO_DEFN

    type(problem), pointer :: p
    allocate(p)
  end function

  ! Set problem from C
  subroutine problem_set_c(p, &
       a_o1v1, a_o2v2, a_o1v2, l, m, n, &
       h10, h20, h30, h11, h21, h31, h1n, h2n, h3n)

    use SRO_DEFN

    type(problem), pointer :: p
    REAL    :: a_o1v1, a_o2v2, a_o1v2
    REAL    :: l, m, n
    REAL    :: h10, h20, h30 ! Lower boundary of h1,2,3
    REAL    :: h11, h21, h31 ! Upper boundary of h1,2,3
    integer :: h1n, h2n, h3n ! Discretization of h1,2,3
    p%a_o1v1 = a_o1v1
    p%a_o2v2 = a_o2v2
    p%a_o1v2 = a_o1v2
    p%l = l
    p%m = m
    p%n = n
    p%h10 = h10
    p%h20 = h20
    p%h30 = h30
    p%h11 = h11
    p%h21 = h21
    p%h31 = h31
    p%h1n = h1n
    p%h2n = h2n
    p%h3n = h3n
  end subroutine

  subroutine problem_set_lmn_c(p, l, m, n)
    use SRO_DEFN

    type(problem), pointer :: p
    REAL    :: l, m, n

    p%l = l
    p%m = m
    p%n = n

  end subroutine

  subroutine problem_free_c(p)

    use SRO_DEFN

    type(problem), pointer :: p
    deallocate(p)
  end subroutine

end module
