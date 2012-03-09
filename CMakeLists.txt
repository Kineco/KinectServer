PROJECT(kineco)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
UNSET(LIBS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

INCLUDE(cmake_modules/FindMHD.cmake)
IF(HAVE_MHD)
    INCLUDE_DIRECTORIES(${MHD_INCLUDE_DIR})
    SET(LIBS ${LIBS} ${MHD_LIBRARY})
ELSE()
    MESSAGE(FATAL_ERROR "microhttpd not found or installed.")
ENDIF()

INCLUDE(cmake_modules/FindOpenNI.cmake)
IF(HAVE_NI)
    INCLUDE_DIRECTORIES(${NI_INCLUDE_DIR})
ELSE()
    MESSAGE(FATAL_ERROR "OpenNI not found or installed.")
ENDIF()

INCLUDE(cmake_modules/FindCV.cmake)
IF(HAVE_OPENCV)
    INCLUDE_DIRECTORIES(${CV_INCLUDE_DIR})
ELSE()
    MESSAGE(FATAL_ERROR "OpenCV not found or installed.")
ENDIF()

FIND_PACKAGE(Boost REQUIRED system thread)
IF(Boost_FOUND)
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ELSE()
    MESSAGE("Boost Not Installed")
ENDIF()

SET(CMAKE_CXX_FLAGS_RELEASE "-w -O3")

ADD_SUBDIRECTORY(WebSocket)
ADD_SUBDIRECTORY(PolarSSL)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/json/include/)
ADD_SUBDIRECTORY(json)

ADD_EXECUTABLE(KinectServer
    AbstractUserDetector.cpp
    CalibratedUsers.cpp
    DepthImage.cpp
    DepthMap.cpp
    DepthParam.cpp
    DetectedUsers.cpp
    HandDetector.cpp
    InterfacePoseDetector.cpp
    KinectHttpServer.cpp
    PointCloud.cpp
    PoseDetector.cpp
    RequestHandler.cpp
    RgbImage.cpp
    UserDetector.cpp
    main.cpp
)

TARGET_LINK_LIBRARIES( KinectServer WebSocket PolarSSL json-cpp ${NI_LIBRARY} ${MHD_LIBRARY} ${CV_LIBS})
