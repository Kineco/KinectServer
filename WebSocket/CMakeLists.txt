cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(WebSocket 
	Connection.cpp 
	ConnectionFactory.cpp 
    Connection_draft76.cpp 
    Connection_ver10.cpp 
    Frame.cpp 
    Header.cpp 
    Server.cpp 
)

TARGET_LINK_LIBRARIES(WebSocket ${Boost_LIBRARIES})
