/*  This software is property of the United States Air Force.  */
#ifndef xylose_util_Singleton_hpp
#define xylose_util_Singleton_hpp

#include <cassert>

namespace xylose {
  namespace util {
    
    /**
     * The Singleton mixin ensures that a class has only one instance and
     * provides a global point of access to it.
     * 
     * To use this class, inherit Singleton< T >, declare the constructor and
     * destructor for your class private, and provide friendship to the 
     * singleton template:
     * 
     * @code
     * class MyClass : public Singleton< MyClass > {
     * public:
     *   void do();
     * 
     * private:
     *   MyClass();
     *   ~MyClass();
     * 
     *   friend class Singleton< MyClass >
     * @endcode
     * 
     * The singleton is constructed upon the first call to instance() and 
     * destroyed upon application exit.
     */
    template< typename T >
    class Singleton {
    public:
      
      /** 
       * Get the single instance of the class.  The return value should not be
       * cached.
       */
      static T* instance() {
        if ( !s_instance().get() ) {
          s_instance().init();
        }
        return s_instance().get();
      }
      
      /**
       * Destroy the single instance of the class.  In the event that this 
       * function is not called, the instance will be cleaned up upon 
       * application exit.
       */
      static void destroyInstance() {
        if ( s_instance().get() ) {
          s_instance().finalize();
        }
      }
      
    protected:
      /** Only sublcass can construct. Only default constructors are allowed. */
      Singleton() {}
      /** Only subclasses can destruct. */
      ~Singleton() {}
      
    private: 
      
      /** 
       * The code that actually instantiates the object must live in Singleton
       * since it has been granted friendship with the class.  This is a 
       * workaround for compilers that do not grant nested classes the same
       * friendship as the enclosing class.
       */
      static T* createObject() {
        return new T;
      }
      
      /** Deletion needs to live in singleton.  Set the note on createObject. */
      static void deleteObject( T* const ptr ) {
        delete ptr;
      }
      
      /** 
       * Container to hold the instance.  We use this instead of auto_ptr
       * because only Singleton (and therefore its inner classes) have the 
       * ability to destroy the instance.  This lets us call the destructor
       * on application exit to clean up the instance.
       */
      class SingletonPtrContainer {
        T* m_ptr;
      public:        
        SingletonPtrContainer() : m_ptr( 0 ) {}
        ~SingletonPtrContainer() {
          if ( m_ptr ) {
            finalize();
          }
        }        
        void init() {
          assert( !m_ptr );
          m_ptr = Singleton::createObject();
        }        
        void finalize() {
          // if this assertion fails, you are using the singleton object after
          // it has been destroyed.
          assert( m_ptr );
          Singleton::deleteObject( m_ptr );
          m_ptr = 0;
        }
        T* get() {
          return m_ptr;
        }
      };
      
      static SingletonPtrContainer& s_instance() {
        static SingletonPtrContainer instance;
        return instance;
      }
      
    };
    
  } // namespace util
} // namespace xylose

#endif //xylose_util_Singleton_hpp
