
#include <xylose/binning/SingleValued.h>
#include <xylose/distribution/DiscreteInverter.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>

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

  /* The probabiltiy distribution (doesn't have to be normalized or uniformly
   * spaced). */
  std::istringstream Pv(
    //v,  P(v)
    " 1   0.5\n"
    " 2   0.5\n"
    " 3   0.1\n"
    " 6   0.7\n"
    " 7   0.4\n"
    " 8   0.3\n"
    "10   0.5\n");

  Inverter dist( Pv );
  Bin bin( 0, 12 );

  for ( int i = 0; i < iter; ++i ) {
    bin.bin( dist() );
  }

  std::ofstream outf("dist.dat");
  bin.print(outf);
  outf.close();

  return 0;
}

