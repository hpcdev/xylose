
#include <xylose/Vector.h>
#include <xylose/binning/SingleValued.h>
#include <xylose/distribution/Uniform.h>
#include <xylose/distribution/Inverter.h>
#include <xylose/distribution/Gaussian.h>
#include <xylose/distribution/Maxwellian2D.h>
#include <xylose/distribution/Maxwellian3D.h>

#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>


namespace dist = xylose::distribution;
using xylose::Vector;
using xylose::binning::SingleValued;

typedef dist::Inverter<> Inverter;

/** SOME DISTRIBUTION FUNCTIONS. **/

static const double amu = 1.66054e-27 /* kg */;
static const double K_B = 1.38065e-23;
static const double uK = 1e-6 /* Tesla */;
static double beta = 0.5 * (87.0 * amu) / (K_B * 500.0 * uK);
static double sigma = std::sqrt( 0.5 / beta);

static Inverter t2d = Inverter(dist::Maxwellian2D(beta), 0.0, 5*sigma, 10000);

static Inverter t3d = Inverter(dist::Maxwellian3D(beta), 0.0, 5*sigma, 10000);

static Inverter gau = Inverter(dist::Gaussian(beta), -4*sigma, 4*sigma, 10000);

static Inverter flt = Inverter(dist::Uniform(),-4*sigma, 4*sigma, 10000);

void nextvalues(Vector<double,4u> & p) {
  p[0] = gau();
  p[1] = t2d();
  p[2] = t3d();
  p[3] = flt();
}

typedef xylose::binning::SingleValued<double,1000> bin;

bin bins[4u] = {
  bin(-4*sigma, 4*sigma),     /* Gaussian */
  bin(0.0, 5*sigma),          /* Maxwellian2D */
  bin(0.0, 5*sigma),          /* Maxwellian3D */
  bin(-4*sigma, 4*sigma)      /* Uniform */
};

int main() {
  int iter = 0;
  std::cout << "Enter the number of samples:  " << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  for ( int i = 0; i < iter; ++i ) {
    Vector<double,4u> p;
    nextvalues(p);

    for ( int j = 0; j < 4u; ++j )
      bins[j].bin(p[j]);

  }

  std::ofstream outf("dist.dat");
  for ( int j = 0; j < 4u; ++j )
    bins[j].print(outf,"") << "\n\n";
  outf.close();

  return 0;
}

