
#ifndef example_nsort_basic_Particle_h
#define example_nsort_basic_Particle_h

#include <xylose/Vector.h>
#include <vector>


struct Particle {
    typedef std::vector<Particle> list;

    Particle(const xylose::Vector<double,3> & x = 0.0,
             const xylose::Vector<double,3> & v = 0.0,
             const int & species = 0) : x(x), v(v), species(species) {}
    xylose::Vector<double, 3> x;
    xylose::Vector<double, 3> v;
    int species;
};

const int & species( const Particle & p ) {
  return p.species;
}

const xylose::Vector<double,3u> & position( const Particle & p ) {
  return p.x;
}


std::ostream & operator<< (std::ostream & out, const Particle & p) {
  return out << "{x: (" << p.x[0] << ", " << p.x[1] << ", " << p.x[2] << "), "
                 "v: (" << p.v[0] << ", " << p.v[1] << ", " << p.v[2] << "), "
                 "t: " << p.species   << '}';
}

void initPVector(Particle::list & pv, const int & n, const int & n_type) {
  static const unsigned int SEED = time(NULL);
  using xylose::Vector;
  srand(SEED);
  pv.resize(n);
  for (int i = 0; i < n; i++) {
    Vector<double,3> x = xylose::V3(
                        100.0*rand()/((double)RAND_MAX) - 50.0,
                        100.0*rand()/((double)RAND_MAX) - 50.0,
                        100.0*rand()/((double)RAND_MAX) - 50.0
                     ),
                     v = xylose::V3(
                        100.0*rand()/((double)RAND_MAX) - 50.0,
                        100.0*rand()/((double)RAND_MAX) - 50.0,
                        100.0*rand()/((double)RAND_MAX) - 50.0
                     );
    int species = (int)rint( ((double)(n_type-1)*rand())/((double)RAND_MAX) );
    pv[i] = Particle(x,v,species);
  }
}

void initPtrVector( Particle::list  & pv,
                    std::vector<Particle*> & ptrv,
                    const int & n, const int & n_type) {
    /* with ptr array */
    initPVector(pv, n, n_type);
    ptrv.resize(n);
    for (unsigned int i = 0; i < pv.size(); ++i)
        ptrv[i] = & pv[i];
}









/* comparators for std::sort and similar routines. */
template <unsigned int dir>
struct position_comp {
    const double pivot;
    position_comp(const double & pivot = 0.0) : pivot(pivot) {}
    bool operator() (const Particle & lhs, const Particle & rhs) {
        return lhs.x[dir] < rhs.x[dir];
    }
};

struct species_comp {
    bool operator() (const Particle & lhs, const Particle & rhs) {
        return lhs.species < rhs.species;
    }
};


#endif // example_nsort_basic_Particle_h
