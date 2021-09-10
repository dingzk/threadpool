#CMake最低版本号要求
cmake_minimum_required(VERSION 3.12)
#项目信息,工程顶层目录名称
project(threadpool)

#使用C++ 11标准
#set(CMAKE_CXX_COMPILER "g++" )
set(CMAKE_CXX_STANDARD 11)

# C99 标准
set(CMAKE_C_STANDARD 99)

# 启用调试/发版模式
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)
# 显示编译过程
set(CMAKE_VERBOSE_MAKEFILE ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SOURCE)


set(THIRDINCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        )

include_directories(${THIRDINCLUDE_DIR})

set(LDFLAGS " -lpthread ")

set(CXX_FLAGS -g  -fpermissive -Wall -Wno-deprecated -Wunused-function  -lpthread)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 指定生成的类库
#add_library(threadpool SHARED ${STRATEGY_DIR} ${CATGORY_DIR})

add_executable(threadpool test.cpp)
#指定生成目标
TARGET_LINK_LIBRARIES(threadpool  pthread)

