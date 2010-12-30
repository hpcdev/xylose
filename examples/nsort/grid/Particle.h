#ifndef test_Particle_h
#define test_Particle_h

#include <xylose/Vector.h>
#include <xylose/random/Kiss.hpp>

#include <cassert>

namespace test {

  using xylose::Vector;

  struct Particle {
    Particle( const Vector<double,3> & x = 0.0,
              const Vector<double,3> & v = 0.0,
              const int & species = 0 )
      : x(x), v(v), species(species) {}

    Vector<double, 3> x;
    Vector<double, 3> v;
    int species;
  };

  const Vector<double,3u> & position( const Particle & p ) {
    return p.x;
  }

  Vector<double,3u> & position( Particle & p ) {
    return p.x;
  }

  const Vector<double,3u> & velocity( const Particle & p ) {
    return p.x;
  }

  Vector<double,3u> & velocity( Particle & p ) {
    return p.v;
  }

  const int & species( const Particle & p ) {
    return p.species;
  }

  int & species( Particle & p ) {
    return p.species;
  }

  std::ostream & operator<< (std::ostream & out, const Particle & p) {
    return out << p.x << '\t' << p.v << '\t' << p.species;
  }

  template < typename Container >
  void initPVector(Container & pv, const unsigned int & n, const int & n_type) {
    xylose::random::Kiss rng;
    pv.clear();
    pv.reserve(n);
    for (unsigned int i = 0; i < n; ++i) {
      Vector<double,3> x = xylose::V3(
                          100.0*rng.rand() - 50.0,
                          100.0*rng.rand() - 50.0,
                          100.0*rng.rand() - 50.0
                       ),
                       v = xylose::V3(
                          100.0*rng.rand() - 50.0,
                          100.0*rng.rand() - 50.0,
                          100.0*rng.rand() - 50.0
                       );
      int species = static_cast<int>( rint( (n_type-1) * rng.rand() ) );
      pv.push_back( Particle(x,v,species) );
    }

    assert( pv.size() == n );
  }

}

#endif // test_Particle_h
