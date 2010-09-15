#include <xylose/binning/Extender.h>
#include <xylose/binning/SingleValued.h>
#include <xylose/distribution/Inverter.h>
#include <xylose/compat/math.hpp>

#include <iostream>
#include <fstream>

/** A sinc distribution for use.  */
struct SincDistribution {
  /** Return sinc(x).  */
  inline double operator() (const double & x) const {
    return x == 0.0 ? 2.0 : (1.0 + std::sin(x) / x);
  }
};

typedef xylose::binning::Extender<
  double,
  xylose::binning::SingleValued<double,100>,
  100u
> BinType;

int main() {
  BinType bin(-10.0,10.0, -10.0,10.0);

  int iter = 0;
  std::cout << "Enter the nubmer of samples:  "
            << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  SincDistribution sinc;

  xylose::distribution::Inverter<> distro(sinc, -10.0, 10.0, 1000);
  for (int i = 0; i < iter; i++) {
    bin.bin(distro(), distro());
  }

  std::ofstream outf("bin.dat");
  bin.print(outf,"");
  outf.close();
  return 0;
}

