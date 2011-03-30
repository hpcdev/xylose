
#include <iostream>
#include <fstream>

#include <xylose/power.h>
#include <xylose/Vector.h>
#include <xylose/integrate/rk.h>

#define X 0
#define Y 1
#define Z 2
#define VX 3
#define VY 4
#define VZ 5

using xylose::Vector;
using xylose::make_vector;
using xylose::SQR;

typedef Vector<double,6u> Particle;

const double omega[] = {
  2*M_PI*500. /* 2 pi  500 Hz */,
  2*M_PI*500. /* 2 pi  500 Hz */,
  2*M_PI*500. /* 2 pi  500 Hz */
};

const double k_m[] = {
  SQR(omega[X]) /*(rad/s)^2 = (2 pi 500  Hz)^2 */,
  SQR(omega[Y]) /*(rad/s)^2 = (2 pi 500  Hz)^2 */,
  SQR(omega[Z]) /*(rad/s)^2 = (2 pi 500  Hz)^2 */
};

const double omega_prime[] = {
  0.25*omega[X] /*rad/s */,
  0.25*omega[Y] /*rad/s */,
  0.25*omega[Z] /*rad/s */
};

/** Iinitial x-position */
const double x0[] = { 0.1, 0.1, 0.1 };

/** Acceleration factor. (units : m ) */
const double a[] = { + 0.1, - 0.1, + 0.0 };


/** ADL function to obtain reference to dt_step from the last argument of
 * rk5::operator() (which, in this case, is just dt_step itsself. */
inline double & localDt( double & dt ) {
  return dt;
}

struct GetDerivs {
  int num_calls;
  GetDerivs() : num_calls(0) { }

  void operator() ( const Particle & p,
                    const double & time,
                    const double & dt,
                          Vector<double,6u> & F,
                    const double & dt_step ) {
    ++num_calls;
    /* copy over the dx/dt, dy/dt, and dz/dt */
    F[X] = p[VX];
    F[Y] = p[VY];
    F[Z] = p[VZ];

    /* and accelerations for a simple harmonic oscillator (kind of simple) */
    F[VX] = - k_m[X]*p[X] - a[X]*k_m[X] * sin( omega_prime[X] * time );
    F[VY] = - k_m[Y]*p[Y] - a[Y]*k_m[Y] * sin( omega_prime[Y] * time );
    F[VZ] = - k_m[Z]*p[Z] - a[Z]*k_m[Z] * sin( omega_prime[Z] * time );
  }
};

struct Solution {
  Vector<double,3> A,B,phi,phip;

  Solution() {
    phip = 0.0;
    using std::atan;
    using std::cos;
    using std::sin;

    phi[X] = atan(-( a[X]/x0[X] )/( omega[X]/omega_prime[X] - omega_prime[X]/omega[X] ));
    phi[Y] = atan(-( a[Y]/x0[Y] )/( omega[Y]/omega_prime[Y] - omega_prime[Y]/omega[Y] ));
    phi[Z] = atan(-( a[Z]/x0[Z] )/( omega[Z]/omega_prime[Z] - omega_prime[Z]/omega[Z] ));

    A[X] = x0[X] / cos(phi[X]);
    A[Y] = x0[Y] / cos(phi[Y]);
    A[Z] = x0[Z] / cos(phi[Z]);

    B[X] = (omega[X]/omega_prime[X]) * sin(phi[X]);
    B[Y] = (omega[Y]/omega_prime[Y]) * sin(phi[Y]);
    B[Z] = (omega[Z]/omega_prime[Z]) * sin(phi[Z]);
  }

  Vector<double,6u> operator() ( const double & t ) {
    return make_vector<double,6u>() (
      A[X]*( cos(omega[X]*t +phi[X]) + B[X]*sin(omega_prime[X]*t +phip[X]) ),
      A[Y]*( cos(omega[Y]*t +phi[Y]) + B[Y]*sin(omega_prime[Y]*t +phip[Y]) ),
      A[Z]*( cos(omega[Z]*t +phi[Z]) + B[Z]*sin(omega_prime[Z]*t +phip[Z]) ),

      A[X]*(omega_prime[X]*B[X]*cos(omega_prime[X]*t + phip[X]) - omega[X]*sin(omega[X]*t + phi[X])),
      A[Y]*(omega_prime[Y]*B[Y]*cos(omega_prime[Y]*t + phip[Y]) - omega[Y]*sin(omega[Y]*t + phi[Y])),
      A[Z]*(omega_prime[Z]*B[Z]*cos(omega_prime[Z]*t + phip[Z]) - omega[Z]*sin(omega[Z]*t + phi[Z]))
    );
  }

  Vector<double,6u> d_dt( const double & t ) {
    return make_vector<double,6u>() (
      A[X]*(omega_prime[X]*B[X]*cos(omega_prime[X]*t + phip[X]) - omega[X]*sin(omega[X]*t + phi[X])),
      A[Y]*(omega_prime[Y]*B[Y]*cos(omega_prime[Y]*t + phip[Y]) - omega[Y]*sin(omega[Y]*t + phi[Y])),
      A[Z]*(omega_prime[Z]*B[Z]*cos(omega_prime[Z]*t + phip[Z]) - omega[Z]*sin(omega[Z]*t + phi[Z])),

      A[X]*(-SQR(omega_prime[X])*B[X]*sin(omega_prime[X] + phip[X]) - SQR(omega[X])*cos(omega[X]*t + phi[X])),
      A[Y]*(-SQR(omega_prime[Y])*B[Y]*sin(omega_prime[Y] + phip[Y]) - SQR(omega[Y])*cos(omega[Y]*t + phi[Y])),
      A[Z]*(-SQR(omega_prime[Z])*B[Z]*sin(omega_prime[Z] + phip[Z]) - SQR(omega[Z])*cos(omega[Z]*t + phi[Z]))
    );
  }
};

int main() {
  Particle p1;

  p1[X]  = x0[X];
  p1[Y]  = x0[Y];
  p1[Z]  = x0[Z];
  p1[VX] = 0.0;
  p1[VY] = 0.0;
  p1[VZ] = 0.0;

  const double T = 0.03L;
  double dt = 1e-4L,
         dt_step = 1e-5L;
  #ifdef WRITE_SOLN
    std::ofstream fout ("out.dat");
  #endif


  Solution soln;

  #ifdef WRITE_SOLN
    fout << 0 << '\t' << p1 << '\t' << dt_step << '\t' << soln(0) << '\n';
  #endif

  xylose::integrate::RK<GetDerivs, 5u> rk;
  rk.errmax = 0.28e-6L;

  Vector<double,6> err_avg = 0.0, err2_avg = 0.0;
  int n = 0;
  double t = 0;
  while ( t <= T ) {
    rk(p1, t, dt, dt_step);
    t+= dt;
    Vector<double,6u> s = soln(t);
    #ifdef WRITE_SOLN
      fout << t << '\t' << p1 << '\t' << dt_step << '\t' << s << '\n';
    #endif

    Vector<double,6u> cal = p1, dx = dt_step * soln.d_dt(t);
    dx.save_fabs(), cal.save_fabs();
    cal += dx + 1e-30;

    Vector<double,6u> frac = compDiv(s - p1, cal);
    Vector<double,6u> err = frac.save_fabs();
    err_avg += err;
    err2_avg += compMult(err,err);
    ++n;
  }

  err_avg /= n;
  err2_avg /= n;
  Vector<double,6u> err_sigma = err2_avg - compMult(err_avg, err_avg);
  err_sigma.save_sqrt();
  Vector<double,6u> err_rms = err2_avg;
  err_rms.save_sqrt();

  std::cout << "Number derivative evaluations:  " << rk.derivs.num_calls << "\n"
       "In units of the target error (" << rk.errmax << ")\n"
       "                  X\t  Y\t  Z\t  VX\t  VY\t  VZ\n"
       "average error:  " << err_avg/rk.errmax << "\n"
       "rms error    :  " << err_rms/rk.errmax << "\n"
       "std(error)   :  " << err_sigma/rk.errmax << std::endl;

  #ifdef WRITE_SOLN
    fout.close();
  #endif

  return 0;
}

