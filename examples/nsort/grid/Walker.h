#ifndef test_Walker_h
#define test_Walker_h

namespace test {

  template < typename Grid,
             class WalkFunctors >
  inline void walk( const Grid & grid, WalkFunctors & fun ) {
    for ( typename Grid::const_iterator i = grid.begin(),
                                           end = grid.end();
                                            i != end; ++i ) {
      fun( *i );
    }
  }

  template < typename Grid,
             class WalkFunctors >
  inline void walk( const Grid & grid,
                    const WalkFunctors & fun = WalkFunctors() ) {
    WalkFunctors ffun(fun);
    walk( grid, ffun );
  }

  template < typename Grid,
             class WalkFunctors >
  inline void walk( Grid & grid, WalkFunctors & fun ) {
    for ( typename Grid::iterator i = grid.begin(),
                                     end = grid.end();
                                      i != end; ++i ) {
      fun( *i );
    }
  }

  template < typename Grid,
             class WalkFunctors >
  inline void walk( Grid & grid, const WalkFunctors & fun = WalkFunctors() ) {
    WalkFunctors ffun(fun);
    walk( grid, ffun );
  }

}

#endif // test_Walker_h
