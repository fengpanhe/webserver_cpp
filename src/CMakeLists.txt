aux_source_directory(. DIR_SRCS_)

# 设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_package(Threads)
add_executable(webserver ${DIR_SRCS_})
#add_executable(webserver epollFunctions.h epollFunctions.cpp)
target_link_libraries(webserver ${CMAKE_THREAD_LIBS_INIT})