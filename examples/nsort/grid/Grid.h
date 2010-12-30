#ifndef test_Grid_h
#define test_Grid_h

#include "Particle.h"
#include "Walker.h"
#include "Cell.h"

#include <xylose/Vector.h>
#include <xylose/Dimensions.hpp>
#include <xylose/nsort/map/w_species.h>
#include <xylose/nsort/map/uniform_grid.h>

#include <ostream>
#include <vector>
#include <cassert>

namespace test {
  using xylose::Vector;
  using xylose::multiply;

  template < unsigned int n_species, typename dims >
  struct Grid {
    /* TYPEDEFS */
    typedef test::Cell<n_species> Cell;
    typedef typename std::vector< Cell >::iterator iterator;
    typedef typename std::vector< Cell >::const_iterator const_iterator;


    /* MEMBER STORAGE */
    std::vector< Cell > cells;
    Vector<double,3u> dx;
    Vector<double,3u> x0;
    Vector<unsigned int,3u> N;


    /* MEMBER FUNCTIONS */
    Grid( const Vector<double,3u> & dx,
          const Vector<unsigned int,3u> & N = 0u,
          const Vector<double,3u> & x0 = 0.0 )
      : cells( multiply<dims>(N), Cell() ),
        dx(dx),
        x0(x0),
        N(N) {
      for ( int i = cells.size() - 1u; i >= 0; --i ) {
        Cell & c = cells[i];
        c.min = x0 + (i+0.0)*dx;
        c.max = x0 + (i+1.0)*dx;
      }
      cells.back().last = true;
    }

    iterator begin() { return cells.begin(); }
    iterator end() { return cells.end(); }

    const_iterator begin() const { return cells.begin(); }
    const_iterator end() const { return cells.end(); }

    const Vector<unsigned int,3u> size() const { return N; }

    int getNumberCells() const { return cells.size(); }

    template < typename NSort >
    void setParticleRanges( const typename Cell::Range::iterator & begin,
                            const NSort & s ) {
      for ( int i = cells.size() - 1u; i >= 0; --i ) {
        Cell & c = cells[i];
        const int si = i * n_species; /**< Beginning nsort bin for ith cell */
        c.particles = typename Cell::Range( begin + s.begin(si),
                                            begin + s.end(si + n_species - 1) );
        c.setSpeciesRanges( begin, s, si );
      }
    }

  };


  template < unsigned int N, typename dims >
  std::ostream & operator<< ( std::ostream & out, const Grid<N,dims> & grid ) {
    walk( grid, CellPrinter(out) );
    return out;
  }

}

#endif // test_Grid_h
