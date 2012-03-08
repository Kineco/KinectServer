CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE_DIRECTORIES("include")

ADD_LIBRARY(json-cpp STATIC
    lib_json/json_reader.cpp
    lib_json/json_value.cpp
    lib_json/json_writer.cpp
)

