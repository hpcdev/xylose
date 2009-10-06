# include the current directory for framework style includes
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# add xylose directory to get the unit tests added recursively
add_subdirectory( xylose )

