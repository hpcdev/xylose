
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

  /* The probabiltiy distribution (doesn't have to be normalized or uniformly
   * spaced). */
  std::vector< std::pair<double,double> > Pv;
                            //  v, P(v)
  Pv.push_back( std::make_pair( 1, 0.5 ) );
  Pv.push_back( std::make_pair( 2, 0.5 ) );
  Pv.push_back( std::make_pair( 3, 0.1 ) );
  Pv.push_back( std::make_pair( 6, 0.7 ) );
  Pv.push_back( std::make_pair( 7, 0.4 ) );
  Pv.push_back( std::make_pair( 8, 0.3 ) );
  Pv.push_back( std::make_pair(10, 0.5 ) );

  Inverter dist( Pv.begin(), Pv.end() );
  Bin bin( 0, 12 );

  for ( int i = 0; i < iter; ++i ) {
    bin.bin( dist() );
  }

  std::ofstream outf("dist.dat");
  bin.print(outf);
  outf.close();

  return 0;
}

