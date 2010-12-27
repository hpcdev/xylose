
#include <xylose/Vector.h>
#include <xylose/binning/SingleValued.h>
#include <xylose/distribution/Inverter.h>
#include <xylose/distribution/Poissonian.h>
#include <xylose/random/PoissonianDeviate.hpp>
#include <xylose/compat/math.hpp>

#include <iostream>
#include <fstream>

#include <cstdlib>


namespace dist = xylose::distribution;
using xylose::Vector;
using xylose::binning::SingleValued;

typedef dist::Inverter<> Inverter;

/* The deviate returns integers.  We set the key for BinD to an integer so that
 * the bin location will be correct when it is printed out (instead of off by
 * +0.5). */
typedef xylose::binning::SingleValued<int,10000> BinD;

/* The inverted poissonian returns double values that are not necessarily whole
 * numbers.  The key here is therefore a double. */
typedef xylose::binning::SingleValued<double,10000> BinI;

int main() {
  int iter = 0;
  std::cout << "Enter the number of samples:  " << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;

  double lambda = 0;
  std::cout << "Enter lambda (note: the histogram range is 0-9999):  " << std::flush;
  std::cin >> lambda;

  std::cout << iter << " samples requested with lambda=" << lambda << std::endl;

  const double max_bin = lambda + std::sqrt(lambda) * 10.0;
  Inverter poi = Inverter(dist::Poissonian(lambda), 0.0, max_bin, 10000);
  xylose::random::PoissonianDeviate<> poi_deviate;


  BinD binD(0.0, 9999);
  BinI binI(0.0, 9999);

  for ( int i = 0; i < iter; ++i ) {
    binD.bin( poi_deviate(lambda) );
    binI.bin( poi() );
  }

  std::ofstream outf("poissonian-deviate.dat");
  binD.print(outf,"") << "\n\n";
  outf.close();

  outf.open("poissonian-inverted.dat");
  binI.print(outf,"") << "\n\n";
  outf.close();

  return 0;
}

