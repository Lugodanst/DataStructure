cmake_minimum_required(VERSION 2.8)
project(data)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_BUILD_TYPE Debug)

set(DEP_LIB ocdata)
aux_source_directory(./src LIB_SRC)
add_library(${DEP_LIB} SHARED ${LIB_SRC})

add_executable(test_slist ./test/test_slist.c)
target_link_libraries(test_slist ${DEP_LIB})

add_executable(test_dlist ./test/test_dlist.c)
target_link_libraries(test_dlist ${DEP_LIB})
