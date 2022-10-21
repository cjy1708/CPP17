cmake_minimum_required(VERSION 3.16)
project(C17)

set(CMAKE_CXX_STANDARD 17)

MESSAGE(STATUS "src_dir = ${CMAKE_SOURCE_DIR}.")
MESSAGE(STATUS "current_src_dir = ${CMAKE_CURRENT_SOURCE_DIR}.")

add_executable(client Client.cpp)
add_executable(backtrace BackTrace.cpp)
add_executable(string string.cpp)
add_executable(thread thread/ThreadBase.cpp)
add_executable(readwritelock thread/ReadWriteLock.cpp)
add_executable(semaphore thread/semaphore.cpp)
add_executable(chain dp/chain_of_responsibility.c dp/list.h)
add_executable(process process/process.cpp)



FIND_PACKAGE(Threads)
MESSAGE("===thread====${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(thread ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(readwritelock ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(semaphore ${CMAKE_THREAD_LIBS_INIT})

add_executable(image stb/image_test.cpp stb/stb_image.h stb/stb_image_write.h stb/stb_image_resize.h stb/fast_gaussian_blur.h)


find_path(MYSQL_ROOT_DIR mysql)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mysql REQUIRED_VARS MYSQL_ROOT_DIR)
add_executable(C17 main.cpp deep_shallow.cpp BaseClass.cpp StructPointer.cpp template.cpp Server.cpp string.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE /usr/include/mysql)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include/mysql++)


target_link_directories(${PROJECT_NAME} PRIVATE ${MYSQL_ROOT_DIR}/lib)
target_link_libraries(${PROJECT_NAME} mysqlpp)


add_library( # Sets the name of the library.
        myJNI

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        jsoncpp/json_reader.cpp
        jsoncpp/json_valueiterator.inl
        jsoncpp/json_value.cpp
        jsoncpp/json_writer.cpp
        jsoncpp/config.h
        jsoncpp/forwards.h
        jsoncpp/json.h
        jsoncpp/reader.h
        jsoncpp/value.h
        jsoncpp/writer.h
        )

link_libraries(myJNI)
add_executable(json jsoncpp/main.cpp)

