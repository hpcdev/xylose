#include "Particle.h"
#include "Grid.h"

#include <xylose/nsort/NSort.h>
#include <xylose/nsort/map/args.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


int main() {
  const int N = 50;
  const unsigned int n_types = 4;

  using xylose::V3;
  using xylose::nsort::NSort;
  using test::Particle;
  using xylose::nsort::map::make_arg;

  typedef xylose::Dimensions<0u>                dims;
  typedef test::Grid<n_types,dims>              Grid;
  Grid                                          grid( 5.0, V3(20u,0u,0u), -50.0 );

  typedef xylose::nsort::map::w_species<
    xylose::nsort::map::uniform_grid<
      Grid,
      dims
    >
  >                                             grid_map;

  std::vector< Particle >                       pv;
  grid_map                                      map(make_arg(n_types, grid));
  NSort< grid_map >                             ps( map.getNumberValues() );

  /* test for sorting particles. */

  initPVector(pv, N, n_types);
  std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));

  ps.sort(pv.begin(), pv.end(), map);
  std::cout << "\nsorted by NSort:\n";
  std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
  std::cout << std::endl;

  grid.setParticleRanges( pv.begin(), ps );

  std::cout << grid << std::endl;

  return 0;
}
