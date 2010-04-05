#ifndef xylose_pool_allocator_h
#define xylose_pool_allocator_h

#include <xylose/segmented_vector.hpp>
#include <xylose/logger.h>
#include <xylose/SyncLock.h>
#include <xylose/bits.hpp>

#include <vector>
#include <cassert>

namespace xylose {

  namespace detail {
    /** A simple typedef to build an stl container of a certain segment size out
     * of the segmented vector.  I had to use this typedef here instead of
     * directly using make_stl_container in the pool_allocator default args
     * because the IBM xlC compiler is lame.
     */
    typedef xylose::make_stl_container<10000u> DefaultPoolContainerT;
  }

  /** Pooled memory allocator for allocating one item at a time. 
   * By default, the segmented_vector is used for the source of memory to
   * allocate.  Using a segmented_vector helps this class allocate mostly
   * contiguous chunks in memory.  */
  template < typename T,
             template <typename,typename> class Container =
               detail::DefaultPoolContainerT::type,
             typename ContainerAlloc = std::allocator<void>,
             bool noOPDealloc = false >
  class pool_allocator {
    /* TYPEDEFS */
  public:
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    /** Meta function to calculate the size of an item in the pool.  This
     * calculation ensures that at least sizeof(int) number of bytes exist so
     * that the union between 'int next_offset' and 'long bytes[]' will always
     * have enough memory to support the 'int next_offset' union member. 
     */
    struct chunk_size {
      static const unsigned int value =
        sizeof(T) + sizeof(long) -
        (sizeof(T) % sizeof(long));
    };

    /** Required rebind template creates a new pool_allocator that supports a
     * different type. */
    template < typename T2 >
    struct rebind {
      typedef pool_allocator<
        T2,
        Container,
        ContainerAlloc,
        noOPDealloc
      > other;
    };

    /** Meta function to create a new allocator enabling/disabling the
     * dealloc function. */
    template < bool B >
    struct setDealloc {
      typedef pool_allocator< T, Container, ContainerAlloc, B > type;
    };

  private:
    union PoolItem {
      int next_offset;
      long bytes[chunk_size::value / sizeof(long)];

      /** Constructor sets the next_offset to default to this+1 */
      PoolItem() : next_offset(1) {}
    };

    typedef Container<PoolItem,ContainerAlloc> Pool;
    typedef typename Pool::iterator PoolIter;
    typedef xylose::SyncKey MemKey;


    struct Impl {
    /* TYPEDEFS */
      /** oh, the evil of using std::vector<bool> !  */
      typedef std::vector<unsigned char> UsedBits;

    /* MEMBER STORAGE */
    private:
      /** The used vector should be the same length as the Pool container
       * divided by 8. */
      UsedBits used;
      Pool pool;
      PoolIter next;
      /** A counter to tell how many items are currently allocated. */
      int number_allocated;
      /** A resource lock for the memory pool. */
      xylose::SyncLock memLock;

      /* MEMBER FUNCTIONS */
    public:
      /** Constructor; sets next allocation to beginning of pool. */
      Impl() : used(), pool(), next(pool.begin()), number_allocated(0) { }

      pointer allocate(size_type n, const_pointer p = NULL) {
        if (n != 1) {
          logger::log_severe("pool allocator can only be used with "
                             "single object (de-)allocations");
          throw std::bad_alloc();
        }

        MemKey memKey( memLock );/* RAII type synchronization */

        if (next == pool.end()) {
          /* need to expand the allocation pool */
          pool.push_back( PoolItem() );
          next = pool.end();
          --next;

          /* push a new blank byte if necessary. */
          bits::resize( used, pool.size() );
        }

        PoolIter pi = next;
        next += next->next_offset;

        /* mark the item as used and give the memory to the caller */
        bits::set( used, pi - pool.begin() );

        ++number_allocated;
        std::allocator<void>::pointer retval = pi->bytes;
        return reinterpret_cast<pointer>(retval);
      }

      /** Resets the allocator so that 1) all used[] items are false, and 2)
       * only rsz number of pool items are left in the pool.  This reserve
       * operation essentially resizes all appropriate vectors/containers so
       * that only rsz items are left.
       *
       * @param rsz
       *    Target capacity of the pool after all items are freed.  If
       *    \f$ rsz < 0 \f$, then no attempts will be made to decrease the
       *    underlying capacity (real memory taken by pool_allocator).<br>
       *    [Default -1]
       */
      void reset( int rsz = -1 ) {
        MemKey memKey( memLock );/* RAII type synchronization */
        if ( number_allocated != 0 ) {
          logger::log_severe("pool allocator reset only valid if no "
                             "objects are allocated ");
          throw std::bad_alloc();
        }

        if ( rsz < 0 ) {
          /* no efforts to release memory */
          used.clear();
          pool.clear();
        } else {
          /* trying to release memory */
          UsedBits tmpU;
          Pool tmpP;
          used.swap( tmpU );
          pool.swap( tmpP );

        }

        /* we'll go ahead and initialize each of the pool items right now so
         * that allocate doesn't have to. */
        rsz = pool.capacity();
        bits::resize( used, rsz );
        pool.resize( rsz );

        next = pool.begin();
      }

      void deallocate(pointer p, size_type n) {
        if (n != 1) {
          logger::log_severe("pool allocator can only be used with "
                             "single object (de-)allocations");
          throw std::bad_alloc();
        }

        MemKey memKey( memLock );/* RAII type synchronization */
        --number_allocated;

        if ( noOPDealloc )
          /* This should be dead code for when noOPDealloc is false */
          return;

        PoolIter pi = pool.getIterator(reinterpret_cast<PoolItem*>(p));

        if ( pi < next ) {
          if (pi < pool.begin()) {
            logger::log_severe("tried to free an item not in pool allocator");
            ++number_allocated; /* Need to correct the number allocated. */
            throw std::bad_alloc();
          }

          /* We're in luck.  This deallocation is really easy. */
          pi->next_offset = (next - pi);
          next = pi;

          bits::unset( used, pi - pool.begin() );
        } else if ( pi < pool.end() ) {
          int pi_idx = pi - pool.begin();
          int used_byte_idx  = pi_idx / 8u;
          int used_bit_idx   = pi_idx % 8u;

          assert( used.size() == ( (pool.size() + 7u) / 8u ) );
          int used_byte_ridx = ((pool.size()-1u) / 8u) - used_byte_idx;

          /* we have to search for unused items lower in index. */
          UsedBits::reverse_iterator bi = used.rbegin() + used_byte_ridx;

          if (!(*bi & ( 1 << used_bit_idx ))) {
            logger::log_severe(
              "tried to free an item that was already freed:  "
              "%lu of %lu items", pi - pool.begin(), pool.size()
            );
            ++number_allocated; /* Need to correct the number allocated. */
            throw std::bad_alloc();
          }

          /* first search for any unused items within the same byte. */
          int unused_bit = used_bit_idx-1;
          {
            const UsedBits::value_type & mixed_byte = *bi;
            while ( unused_bit >= 0 && mixed_byte & (1 << unused_bit) )
              --unused_bit;
          }

          if ( unused_bit < 0 ) {
            /* now search for the next byte with an item not allocated. */
            for (++bi; bi != used.rend() && (*bi) == 0xff; ++bi);

            if ( bi == used.rend() ) {
              logger::log_severe("could not find previous freed item in allocator pool");
              ++number_allocated; /* Need to correct the number allocated. */
              throw std::bad_alloc();
            }

            /* now search for the particular unused bit. */
            unused_bit = 7;
            {
              const UsedBits::value_type & mixed_byte = *bi;
              while ( unused_bit >= 0 && mixed_byte & (1 << unused_bit) )
                --unused_bit;
            }
          }

          assert( unused_bit >= 0 );

          PoolIter i = pool.begin() + (used.rend() - bi - 1 + unused_bit);
          int pi_minus_i = (pi - i);
          pi->next_offset = i->next_offset - pi_minus_i;
          i->next_offset = pi_minus_i;
          bits::unset( used, pi - pool.begin() );
        } else {
          logger::log_severe("tried to free an item not in pool allocator");
          ++number_allocated; /* Need to correct the number allocated. */
          throw std::bad_alloc();
        }
      }

    };/* struct Impl */


    /* STATIC STORAGE */
  private:
    static Impl impl;


    /* MEMBER FUNCTIONS */
  public:
    /** Constructor; sets next allocation to beginning of pool.
     * Since this allocator is stateless (state is stored in static member) this
     * constructor does nothing.  */
    pool_allocator() {}

    /** Copy constructor--required by stl containers.
     * Since this allocator is stateless (state is stored in static member) this
     * constructor does nothing.  */
    pool_allocator( const pool_allocator & other ) {}

    /** Copy constructor from allocator of different type--required by stl containers.
     * Since this allocator is stateless (state is stored in static member) this
     * constructor does nothing. */
    template < typename T1 >
    pool_allocator( const pool_allocator<T1,Container,ContainerAlloc,noOPDealloc> & other ) {}

    /** convert a reference to an address.  What is the purpose of this function
     * in an allocator?  I'm including it only because it appears that it is
     * standard. */
    pointer address(reference x) const {
      return &x;
    }

    /** convert a reference to an (const) address.  What is the purpose of this function
     * in an allocator?  I'm including it only because it appears that it is
     * standard. */
    const_pointer address(const_reference x) const {
      return &x; 
    }

    pointer allocate(size_type n, std::allocator<void>::const_pointer p = 0) {
      return impl.allocate(n, reinterpret_cast<const_pointer>(p));
    }

    /** Resets the allocator so that 1) all used[] items are false, and 2)
     * only rsz number of pool items are left in the pool.  This reserve
     * operation essentially resizes all appropriate vectors/containers so
     * that only rsz items are left.
     * NOTE:  this operation will fail unless deallocate has been called for all
     * items in the pool.  Further note that this operation is only really
     * helpful for when the noOPDealloc template parameter is true (Default is
     * false).
     *
     * @param rsz
     *    Target capacity of the pool after all items are freed.  If
     *    \f$ rsz < 0 \f$, then no attempts will be made to decrease the
     *    underlying capacity (real memory taken by pool_allocator).<br>
     *    [Default -1]
     */
    void reset( const int & rsz = -1 ) {
      impl.reset( rsz );
    }

    /** Deallocate an object. */
    void deallocate(pointer p, size_type n = 1) {
      impl.deallocate(p,n);
    }

    void construct(pointer p, const T& t = T()) {
      new(p) T(t);
    }

    void destroy(pointer p) {
      p->~T();
    }


    size_type max_size() const {
      return static_cast<size_type>(-1) / sizeof(value_type);
    }
  };


  /* Declare the initialization of the static pool implementation. */
  template < typename T,
             template <typename,typename> class C,
             typename CA,
             bool D >
  typename pool_allocator<T,C,CA,D>::Impl pool_allocator<T,C,CA,D>::impl = Impl();

}

#endif // xylose_pool_allocator_h
