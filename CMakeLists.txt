cmake_minimum_required(VERSION 3.20)

#project name
PROJECT(BasicTools)


#c11 standard
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Require C++17 support

SET(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libgcc -O3 -flto -g") 

find_package(Threads)

#src lists
SET(
ThreadTest
src/thread/thread.cpp
test/thread_test.cpp
)


#head file path
INCLUDE_DIRECTORIES(
/include/
)

#lib file path
LINK_DIRECTORIES(
/libs/
)

#set extern libraries
SET(LIBRARIES

)

# set output binary path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#add executable file
ADD_EXECUTABLE(threadtest ${ThreadTest})

#add link library
TARGET_LINK_LIBRARIES(threadtest ${LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})