set(CMAKE_FIND_FRAMEWORK "LAST")
FIND_PATH(CV_CHK_VER "include/opencv2" PATHS "/opt/local" "/usr/local")

MESSAGE("${CV_CHK_VER}")

IF(EXISTS ${CV_CHK_VER})
    #OpenCVのバージョンが2.2以上の場合
    #インクルード、ライブラリパスのSET
    SET(HAVE_OPENCV true)
    SET(CV_VERSION_LOWER false)
    SET(OPENCV_DIR "${CV_CHK_VER}")
    SET(CV_INCLUDE_DIR "${OPENCV_DIR}/include" CACHE STRING "" FORCE)
    SET(CV_LIB_DIR "${OPENCV_DIR}/lib" CACHE STRING "" FORCE)
    #2.2以上の場合、ライブラリ名がlibopencv_coreとかになっている
    SET(CV_LIB_COMPONENT  opencv_core opencv_highgui opencv_imgproc)
    SET(_CV_VERSION_FILE "${CV_INCLUDE_DIR}/opencv2/core/version.hpp")
ELSE()
    #OpenCVのバージョンが2.1以下の場合
    SET(CV_VERSION_LOWER true)
    FIND_PATH(CV_CHK_VER "include/opencv" PATHS "/opt/local" "/usr/local" "/usr")
        IF(EXISTS "${CV_CHK_VER}")
            SET(HAVE_OPENCV true)
            SET(OPENCV_DIR "${CV_CHK_VER}")
            SET(CV_INCLUDE_DIR "${OPENCV_DIR}/include" CACHE STRING "" FORCE)
            SET(CV_LIB_DIR "${OPENCV_DIR}/lib" CACHE STRING "" FORCE)
            SET(_CV_VERSION_FILE "${CV_INCLUDE_DIR}/opencv/cvver.h")
            SET(CV_LIB_COMPONENT cv highgui cxcore)
        ELSE()
            SET(HAVE_OPENCV false)
        ENDIF()
ENDIF()    

IF(HAVE_OPENCV)
    MESSAGE("-- OpenCV")
    #バージョン番号のチェック
    FILE(STRINGS ${_CV_VERSION_FILE} STR1 REGEX "CV_MAJOR_VERSION")
    FILE(STRINGS ${_CV_VERSION_FILE} STR2 REGEX "CV_MINOR_VERSION")
    
    STRING(REGEX MATCH "[0-9]+" _CV_MAJOR ${STR1})
    STRING(REGEX MATCH "[0-9]+" _CV_MINOR ${STR2})
    SET(CV_VERSION ${_CV_MAJOR}.${_CV_MINOR} CACHE STRING "" FORCE)
    MESSAGE("OpenCV Version : ${CV_VERSION}")
    
    #ライブラリの検索
    FOREACH(__CVLIB ${CV_LIB_COMPONENT})
        FIND_LIBRARY(CV_${__CVLIB}_LIBRARY "${__CVLIB}" PATHS "${CV_LIB_DIR}" NO_DEFAULT_PATH)
        MARK_AS_ADVANCED(CV_${__CVLIB}_LIBRARY)
        IF(CV_${__CVLIB}_LIBRARY)
            SET(_CV_LIBS ${_CV_LIBS} ${CV_${__CVLIB}_LIBRARY})
        ENDIF()
    ENDFOREACH()
    SET(CV_LIBS ${_CV_LIBS} CACHE STRING "" FORCE)
MESSAGE("${CV_LIBS}")
ENDIF()
MESSAGE("${CV_INCLUDE_DIR}")
MARK_AS_ADVANCED(_CV_LIBS)
MARK_AS_ADVANCED(CV_CHK_VER)
