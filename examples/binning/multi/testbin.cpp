#include <xylose/Vector.h>
#include <xylose/binning/MultiValued.h>
#include <xylose/distribution/Uniform.h>
#include <xylose/distribution/Inverter.h>

#include <iostream>
#include <fstream>

int main() {
  xylose::binning::MultiValued<double,double,3,201> bin(-0.5,0.5);

  int iter = 0;
  std::cout << "Enter the nubmer of samples:  "
            << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  namespace dist = xylose::distribution;

  dist::Inverter<> distro(dist::Uniform(), -0.5, 0.5, 1000);
  for (int i = 0; i < iter; i++) {
    using xylose::Vector;
    using xylose::V3;
    double x = distro();
    Vector<double,3> v =
      V3( std::log(std::abs(x)), std::exp(x), std::pow(2.0,x) );
    bin.bin(distro(),v);
  }

  std::ofstream outf("bin.dat");
  bin.print(outf,"");
  outf.close();
  return 0;
}

