
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

/** SOME DISTRIBUTION FUNCTIONS. **/

typedef xylose::binning::SingleValued<double,10000> Bin;

int main() {
  int iter = 0;
  std::cout << "Enter the number of samples:  " << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;

  double lambda = 0;
  std::cout << "Enter lambda:  " << std::flush;
  std::cin >> lambda;

  std::cout << iter << " samples requested with lambda=" << lambda << std::endl;

  const double max_bin = std::sqrt(lambda) * 10.0;
  Inverter poi = Inverter(dist::Poissonian(lambda), 0.0, max_bin, 10000);
  xylose::random::PoissonianDeviate<> poi_deviate;


  Bin bin0(0.0, 9999);
  Bin bin1(0.0, 9999);

  for ( int i = 0; i < iter; ++i ) {
    bin0.bin( poi_deviate(lambda) );
    bin1.bin( poi() );
  }

  std::ofstream outf("poissonian-deviate.dat");
  bin0.print(outf,"") << "\n\n";
  outf.close();

  outf.open("poissonian-inverted.dat");
  bin1.print(outf,"") << "\n\n";
  outf.close();

  return 0;
}

