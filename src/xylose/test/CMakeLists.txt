xylose_unit_test( pool_allocator pool_allocator.cpp )
xylose_unit_test( TestIndex TestIndex.cpp )
xylose_unit_test( TestStack TestStack.cpp )
xylose_unit_test( Test_segmented_vector Test_segmented_vector.cpp )
xylose_unit_test( Time_segmented_vector Time_segmented_vector.cpp )
xylose_unit_test( TestSingleton TestSingleton.cpp )
xylose_unit_test( SyncLock_nothreads SyncLock.cpp )
xylose_unit_test( Vector Vector.cpp )

# TODO: Remains of Jamfile based testing.  These need to be ported to CMake.
# xylose_unit_test( SyncLock_pthreads SyncLock.cpp )
# xylose_unit_test( SyncLock_omp SyncLock.cpp )
# 
# unit-test SyncLock_pthreads
#     : SyncLock_pthreads_obj
#     : <cflags>-pthread <linkflags>-pthread
#     ;
# unit-test SyncLock_omp
#     : SyncLock_omp_obj
#     : <toolset>gcc:<cflags>-fopenmp
#       <toolset>gcc:<linkflags>-fopenmp
#       <toolset>intel:<cflags>-openmp
#       <toolset>intel:<linkflags>-openmp
#     ;
# 
# obj SyncLock_nothreads_obj : SyncLock.cpp ;
# obj SyncLock_pthreads_obj : SyncLock.cpp : <define>USE_PTHREAD <cflags>-pthread  ;
# obj SyncLock_omp_obj
#     : SyncLock.cpp
#     : <toolset>gcc:<cflags>-fopenmp
#       <toolset>intel:<cflags>-openmp
#     ;
