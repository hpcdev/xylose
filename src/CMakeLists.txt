#
# XyloseUtil is a collection of useful utilities.
#
# This project should have no external dependencies.  All that should be 
# needed to compile this project is a standards compliant C++ compiler.

xylose_library( XyloseUtil
  GLOB xylose/*.cpp
       xylose/detail/*.cpp
       xylose/random/*.cpp
       xylose/random/detail/*.cpp
  EXCLUDE xylose/util/train/Station.cpp
  INCLUDE_DIRS .
  STATIC
)

client_requirements( XyloseUtil INCLUDE_DIRS . )


# add xylose directory to get the unit tests added recursively
add_subdirectory( xylose )

