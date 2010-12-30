#ifndef test_Cell_h
#define test_Cell_h

#include "Particle.h"

#include <xylose/Vector.h>
#include <xylose/IteratorRange.h>
#include <xylose/nsort/utility/w_species_fields.h>

#include <ostream>
#include <vector>
#include <iterator>
#include <algorithm>

namespace test {
  using xylose::nsort::utility::w_species_fields;

  typedef std::vector<Particle>::iterator ParticleIterator;

  template < unsigned int n_species >
  struct Cell : w_species_fields<ParticleIterator, n_species> {
    typedef w_species_fields<ParticleIterator, n_species> super;
    typedef xylose::IteratorRange< ParticleIterator > Range;
    Range particles;
    /* This information is redundant, but I want it for now to print out to the
     * user easily. */
    xylose::Vector<double,3u> min;
    xylose::Vector<double,3u> max;
    bool last;
    Cell() : super(), particles(), min(0.0), max(0.0), last(false) { }
  };

  template < unsigned int N >
  std::ostream & operator<< ( std::ostream & out, const Cell<N> & cell ) {
    std::copy( cell.particles.begin(),
               cell.particles.end(),
               std::ostream_iterator<Particle>(out, "\n"));

    return out;
  }


  struct CellPrinter {
    std::ostream & out;

    CellPrinter( std::ostream & out ) : out(out) { }

    template < typename Cell >
    void operator() ( const Cell & cell ) {
      out << "**************************CELL EDGE:  "
          << cell.min
          << "**************************\n"
          << cell;
      if ( cell.last )
        out << "**************************CELL EDGE:  "
            << cell.max
            << "**************************\n";
    }
  };

}

#endif // test_Cell_h
