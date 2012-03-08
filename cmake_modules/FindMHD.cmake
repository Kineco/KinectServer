IF(NOT "${MHD_LIB_DIR}" STREQUAL "${MHD_LIB_DIR_INTERNAL}")
    unset(MHD_LIBRARY CACHE)
    unset(MHD_LIB_DIR CACHE)
ENDIF()

IF(NOT "${MHD_INCLUDE_DIR}" STREQUAL "${MHD_INCLUDE_DIR_INTERNAL}")
    UNSET(MHD_INCLUDES CACHE)
    UNSET(MHD_INCLUDE_DIR CACHE)
ENDIF()

FIND_FILE(MHD_INCLUDES "microhttpd.h" PATHS $ENV{MHD_INCLUDE} "/usr/local/include" "/usr/include" DOC "libmicrohttpd header")

FIND_LIBRARY(MHD_LIBRARY "microhttpd" PATHS $ENV{MHD_LIB} "/usr/lib" "/usr/local/lib" DOC "libmicrohttpd library")

GET_FILENAME_COMPONENT(MHD_INCLUDE_DIR "${MHD_INCLUDES}" PATH CACHE)
GET_FILENAME_COMPONENT(MHD_LIB_DIR "${MHD_LIBRARY}" PATH CACHE)

IF(MHD_INCLUDES AND MHD_LIBRARY)
    set(HAVE_MHD TRUE)
ENDIF()
IF(MHD_INCLUDES)
    SET(MHD_INCLUDE_DIR_INTERNAL "${MHD_INCLUDE_DIR}" CACHE INTERNAL "This is the value of the last time MHD_INCLUDE_DIR was set successfuly." FORCE)
ELSE()
    MESSAGE(WARNING ," microhttpd include directory (set by MHD_INCLUDE_DIR variable) is not found or does not have microhttpd include file")
ENDIF()

IF(MHD_LIBRARY)
    SET(MHD_LIB_DIR_INTERNAL "${MHD_LIB_DIR}" CACHE INTERNAL "This is the value of the last time MHD_LIB_DIR was set successfuly." FORCE)
ELSE()
    MESSAGE(WARNING ," microhttpd library directory (set by MHD_LIB_DIR variable) is not found or does not have microhttpd library file.")
ENDIF()

MARK_AS_ADVANCED(FORCE MHD_LIBRARY)
MARK_AS_ADVANCED(FORCE MHD_INCLUDES)
