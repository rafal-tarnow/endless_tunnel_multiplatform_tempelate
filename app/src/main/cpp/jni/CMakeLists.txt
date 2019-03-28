cmake_minimum_required(VERSION 3.4.1)

add_library(hello-jni SHARED
        hello-jni.cpp
        cdelegate.h
        ctimer.h
        ctimer.cpp
        cudpsocket.h
        cudpsocket.cpp
        cunixdatagramsocket.h
        cunixdatagramsocket.cpp
	aunixdatagramsocket.h
        aunixdatagramsocket.cpp
        epoll.hpp
        epoll.cpp
	main_2.h
	main_2.cpp)

# Include libraries needed for hello-jni lib
target_link_libraries(hello-jni
                      android
                      log
		              )
