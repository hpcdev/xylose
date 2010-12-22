xylose_unit_test( pool_allocator pool_allocator.cpp )
xylose_unit_test( TestIndex TestIndex.cpp )
xylose_unit_test( TestStack TestStack.cpp )
xylose_unit_test( Test_segmented_vector Test_segmented_vector.cpp )
xylose_unit_test( Time_segmented_vector Time_segmented_vector.cpp )
xylose_unit_test( TestSingleton TestSingleton.cpp )
xylose_unit_test( SyncLock_nothreads SyncLock.cpp )
xylose_unit_test( Vector Vector.cpp )
xylose_unit_test( TestTypedFactory TestTypedFactory.cpp )
xylose_unit_test( bits bits.cpp )


find_package( Threads )
if ( THREADS_FOUND AND CMAKE_USE_PTHREADS_INIT )
    xylose_unit_test( SyncLock_pthreads SyncLock.cpp )
    set_target_properties( xylose.SyncLock_pthreads.test
        PROPERTIES
        LINK_FLAGS ${CMAKE_THREAD_LIBS_INIT}
        COMPILE_FLAGS ${CMAKE_THREAD_LIBS_INIT}
    )
endif()

find_package( OpenMP )
if ( OPENMP_FOUND )
    xylose_unit_test( SyncLock_omp SyncLock.cpp )
    set_target_properties( xylose.SyncLock_omp.test
        PROPERTIES
        LINK_FLAGS ${OpenMP_CXX_FLAGS}
        COMPILE_FLAGS ${OpenMP_CXX_FLAGS}
    )
endif()
