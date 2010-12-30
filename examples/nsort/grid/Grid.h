#ifndef test_Grid_h
#define test_Grid_h

#include "Particle.h"
#include "Walker.h"
#include "Cell.h"

#include <xylose/Vector.h>
#include <xylose/nsort/map/w_species.h>

#include <ostream>
#include <vector>
#include <cassert>

namespace test {
  using xylose::Vector;

  template < unsigned int n_species = 1u,
             unsigned int ndims = 3u > struct Grid;

  template < unsigned int n_species >
  struct Grid<n_species, 1u> {
    /* TYPEDEFS */
    typedef test::Cell<n_species> Cell;
    typedef typename std::vector< Cell >::iterator iterator;
    typedef typename std::vector< Cell >::const_iterator const_iterator;


    /* MEMBER STORAGE */
    std::vector< Cell > cells;
    Vector<double,3u> dx;
    Vector<double,3u> x0;


    /* MEMBER FUNCTIONS */
    Grid( const Vector<double,3u> & dx,
          const unsigned int & N = 0,
          const Vector<double,3u> & x0 = 0.0 )
      : cells( N, Cell() ), dx(dx), x0(x0) {
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


  template < unsigned int N, unsigned int D >
  std::ostream & operator<< ( std::ostream & out, const Grid<N,D> & grid ) {
    walk( grid, CellPrinter(out) );
    return out;
  }



  namespace map {
    template < unsigned int n_species,
               unsigned int dir0,
               unsigned int dir1 = dir0,
               unsigned int dir2 = dir0 >
    struct grid_location;

    template < unsigned int n_species,
               unsigned int dir0 >
    struct grid_location<n_species, dir0,dir0,dir0> {
      typedef void super;

      const Grid<n_species,1u> & g;
      const int max_cell;

      grid_location( const Grid<n_species,1u> & g )
        : g(g), max_cell( g.getNumberCells() - 1 ) {
        assert( max_cell >= 0 );
      }

      int getNumberValues() const {
        return max_cell + 1;
      }

      template <class _Particle>
      int operator()(const _Particle & p) const {
        int L = static_cast<int>((position(p)[dir0] - g.x0[dir0]) / g.dx[dir0]);
        return std::max(0, std::min( max_cell, L ) );
      }
    };

    template < unsigned int n_species,
               unsigned int dir0,
               unsigned int dir1 = dir0,
               unsigned int dir2 = dir0 >
    struct make_grid_map {
      typedef xylose::nsort::map::w_species<
        grid_location<n_species,dir0,dir1,dir2>
      > type;
    };
  }

}

#endif // test_Grid_h
