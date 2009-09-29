#ifndef xylose_pool_allocator_h
#define xylose_pool_allocator_h

#include <xylose/segmented_vector.hpp>

#include <xylose/logger.h>
#include <xylose/SyncLock.h>

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
             typename ContainerAlloc = std::allocator<void> >
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
      typedef pool_allocator<T2, Container, ContainerAlloc> other;
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
    /* MEMBER STORAGE */
    private:
      ///** oh, the evil of using std::vector<bool> ! */
      std::vector<bool> used;
      Pool pool;
      PoolIter next;
      xylose::SyncLock memLock;

      /* MEMBER FUNCTIONS */
    public:
      /** Constructor; sets next allocation to beginning of pool. */
      Impl() : pool(), next(pool.begin()) { }

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
          used.push_back(false);
        }

        PoolIter pi = next;
        next += next->next_offset;
        /* mark the item as used and give the memory to the caller */
        used[pi - pool.begin()] = true;

        return reinterpret_cast<pointer>(pi->bytes);
      }

      void deallocate(pointer p, size_type n) {
        if (n != 1) {
          logger::log_severe("pool allocator can only be used with "
                             "single object (de-)allocations");
          throw std::bad_alloc();
        }

        MemKey memKey( memLock );/* RAII type synchronization */
        PoolIter pi = pool.getIterator(reinterpret_cast<PoolItem*>(p));

        if ( pi < next ) {
          if (pi < pool.begin()) {
            logger::log_severe("tried to free an item not in pool allocator");
            throw std::bad_alloc();
          }

          /* We're in luck.  This deallocation is really easy. */
          pi->next_offset = (next - pi);
          next = pi;
          used[pi - pool.begin()] = false;
        } else if ( pi < pool.end() ) {
          /* we have to search for unused items lower in index. */
          std::vector<bool>::reverse_iterator bi = used.rbegin() + (pool.end()-pi - 1);

          if (!(*bi)) {
            logger::log_severe("tried to free an item that was already freed");
            throw std::bad_alloc();
          }

          /* now search for the next item not allocated. */
          for (++bi; bi != used.rend() && (*bi); ++bi);

          if ( bi == used.rend() ) {
            logger::log_severe("could not find previous freed item in allocator pool");
            throw std::bad_alloc();
          }

          PoolIter i = pool.begin() + (used.rend() - bi - 1);
          int pi_minus_i = (pi - i);
          pi->next_offset = i->next_offset - pi_minus_i;
          i->next_offset = pi_minus_i;
          used[pi - pool.begin()] = false;
        } else {
          logger::log_severe("tried to free an item not in pool allocator");
          throw std::bad_alloc();
        }
      }

    };/* struct Impl */


    /* MEMBER STORAGE */
  private:
    Impl & impl;


    /* MEMBER FUNCTIONS */
  private:
    static Impl & impl_instance() {
      /* Is this initialization thread-safe?  Need to do some research
       * about the standard... */
      static Impl impl;
      return impl;
    }

  public:
    /** Constructor; sets next allocation to beginning of pool. */
    pool_allocator() : impl(impl_instance()) {}

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

    pointer allocate(size_type n, const_pointer p = NULL) {
      return impl.allocate(n,p);
    }

    void deallocate(pointer p, size_type n = 1) {
      return impl.deallocate(p,n);
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
}

#endif // xylose_pool_allocator_h
