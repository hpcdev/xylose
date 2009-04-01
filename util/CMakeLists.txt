#
# XyloseUtil is a collection of useful utilities.
#
# This project should have no external dependencies.  All that should be 
# needed to compile this project is a standards compliant C++ compiler.

xylose_library( XyloseUtil *.cpp STATIC )

add_subdirectory( test )

