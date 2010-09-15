
#include <xylose/binning/SingleValued.h>
#include <xylose/distribution/DiscreteInverter.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

namespace dist = xylose::distribution;
using xylose::binning::SingleValued;
typedef dist::DiscreteInverter<xylose::random::Kiss,true> Inverter;

/** SOME DISTRIBUTION FUNCTIONS. **/

typedef xylose::binning::SingleValued<double,13> Bin;

int main() {
  int iter = 0;
  std::cout << "Enter the number of samples:  " << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  Inverter dist( "Pv.dat" );
  Bin bin( 0, 12 );

  for ( int i = 0; i < iter; ++i ) {
    bin.bin( dist() );
  }

  std::ofstream outf("dist.dat");
  bin.print(outf);
  outf.close();

  return 0;
}

