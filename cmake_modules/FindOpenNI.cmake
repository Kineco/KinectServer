
if(NOT "${NI_LIB_DIR}" STREQUAL "${NI_LIB_DIR_INTERNAL}")
    unset(NI_LIBRARY CACHE)
    unset(NI_LIB_DIR CACHE)
endif()

if(NOT "${NI_INCLUDE_DIR}" STREQUAL "${NI_INCLUDE_DIR_INTERNAL}")
    unset(NI_INCLUDES CACHE)
    unset(NI_INCLUDE_DIR CACHE)
endif()

if(NOT "${PRIME_SENSOR_MODULE_BIN_DIR}" STREQUAL "${PRIME_SENSOR_MODULE_BIN_DIR_INTERNAL}")
    unset(PRIME_SENSOR_MODULE CACHE)
    unset(PRIME_SENSOR_MODULE_BIN_DIR CACHE)
endif()

find_file(NI_INCLUDES "XnCppWrapper.h" PATHS $ENV{NI_INCLUDE} "/usr/include/ni" "/usr/include/openni" "c:/Program Files/OpenNI/Include" DOC "OpenNI c++ interface header")
find_library(NI_LIBRARY "OpenNI" PATHS $ENV{NI_LIB} "/usr/lib" "c:/Program Files/OpenNI/Lib" DOC "OpenNI library")

if(NI_LIBRARY AND NI_INCLUDES)
    set(HAVE_NI TRUE)
    # the check: are PrimeSensor Modules for OpenNI installed
    if(WIN32)
        find_file(PRIME_SENSOR_MODULE "XnCore.dll" PATHS "c:/Program Files/Prime Sense/Sensor/Bin" DOC "Core library of PrimeSensor Modules for OpenNI")
    elseif(UNIX OR APPLE)
        find_library(PRIME_SENSOR_MODULE "XnCore" PATHS "/usr/lib" DOC "Core library of PrimeSensor Modules for OpenNI")
    endif()
	
    if(PRIME_SENSOR_MODULE)
        set(HAVE_PRIME_SENSOR_MODULE TRUE)
    endif()
endif() #if(OPENNI_LIBRARY AND OPENNI_INCLUDES)

get_filename_component(NI_LIB_DIR "${NI_LIBRARY}" PATH CACHE)
get_filename_component(NI_INCLUDE_DIR ${NI_INCLUDES} PATH CACHE)
get_filename_component(PRIME_SENSOR_MODULE_BIN_DIR "${PRIME_SENSOR_MODULE}" PATH CACHE)

if(NI_LIBRARY)
    set(NI_LIB_DIR_INTERNAL "${NI_LIB_DIR}" CACHE INTERNAL "This is the value of the last time OPENNI_LIB_DIR was set successfully." FORCE)
else()
    message( WARNING, " OpenNI library directory (set by OPENNI_LIB_DIR variable) is not found or does not have OpenNI libraries." )
endif()

if(NI_INCLUDES)
    set(NI_INCLUDE_DIR_INTERNAL "${NI_INCLUDE_DIR}" CACHE INTERNAL "This is the value of the last time OPENNI_INCLUDE_DIR was set successfully." FORCE)
else()
    message( WARNING, " OpenNI include directory (set by OPENNI_INCLUDE_DIR variable) is not found or does not have OpenNI include files." )
endif()

if(PRIME_SENSOR_MODULE)
    set(PRIME_SENSOR_MODULE_BIN_DIR_INTERNAL "${PRIME_SENSOR_MODULE_BIN_DIR}" CACHE INTERNAL "This is the value of the last time OPENNI_PRIME_SENSOR_MODULE_BIN_DIR was set successfully." FORCE)
else()
    message( WARNING, " PrimeSensor Module binaries directory (set by OPENNI_PRIME_SENSOR_MODULE_BIN_DIR variable) is not found or does not have PrimeSensor Module binaries." )
endif()

mark_as_advanced(FORCE PRIME_SENSOR_MODULE)
mark_as_advanced(FORCE NI_LIBRARY)
mark_as_advanced(FORCE NI_INCLUDES)
