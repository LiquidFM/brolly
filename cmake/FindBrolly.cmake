# - Find brolly
# Find the brolly library
#
#  This module defines the following variables:
#     BROLLY_FOUND   - True if BROLLY_INCLUDE is found
#     BROLLY_INCLUDE - where to find header files
#     BROLLY_LIB     - the library files


if (NOT BROLLY_FOUND)
    find_path (BROLLY_INCLUDE
	       NAMES "brolly/assert.h"
	       PATH_SUFFIXES include
	       DOC "Brolly include directory")

    find_library (BROLLY_LIB
		  NAMES brolly
		  DOC "Brolly library file")

    # handle the QUIETLY and REQUIRED arguments and set BROLLY_FOUND to TRUE if all listed variables are TRUE
    include (FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS (Brolly DEFAULT_MSG BROLLY_INCLUDE BROLLY_LIB)

    find_package (Platform REQUIRED)
    list (APPEND BROLLY_INCLUDE ${PLATFORM_INCLUDE})

    list (REMOVE_DUPLICATES BROLLY_INCLUDE)

    mark_as_advanced (BROLLY_INCLUDE BROLLY_LIB)
endif ()
