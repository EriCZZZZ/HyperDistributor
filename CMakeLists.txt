cmake_minimum_required(VERSION 3.2)

PROJECT(hyper-distributor-demo)

set(DEMO_VERSION_MAJOR 0)
set(DEMO_VERSION_MINOR 1)
set(DEMO_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)

configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/config.h")

OPTION(DEMO_DEBUG "Debug mode" ON)
OPTION(DISTRIBUTOR_LOCK_FREE "Distributor multithread policy" OFF)
OPTION(TASK_LOCK_FREE "Task multithread policy" ON)
OPTION(TASK_LF_WRONG_GEN "使用非线程安全的无锁代码" OFF)
OPTION(LOG_LIB_LOG4CPP "Use log4cpp as log freamwork" ON)

# dependencies
INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR})

# source code
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

# hyper distributor
add_subdirectory(src/HyperDistributor)

# lib
if (LOG_LIB_LOG4CPP)
    FIND_LIBRARY(LOG4CPP_LIB_PATH log4cpp /usr/lib /usr/local/lib NO_DEFAULT_PATH)
    LINK_LIBRARIES(${LOG4CPP_LIB_PATH} -lpthread)
endif (LOG_LIB_LOG4CPP)

LINK_LIBRARIES()

ADD_EXECUTABLE(hyper-distributor-demo ${DIR_SRCS})

# link hyper distributor
target_link_libraries(hyper-distributor-demo HyperDistributor)
