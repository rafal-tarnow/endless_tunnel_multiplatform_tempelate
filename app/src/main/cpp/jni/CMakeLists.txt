cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(library_api)

add_library(hello-jni SHARED
        hello-jni.cpp
		main_2.h
		main_2.cpp)

# Include libraries needed for hello-jni lib
target_link_libraries(	hello-jni
                      	android
                      	log
						library_api
		              )
