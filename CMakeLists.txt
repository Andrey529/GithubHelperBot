cmake_minimum_required(VERSION 3.24)
project(GithubHelperBot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()
link_directories(/usr/local/lib/libpqxx/lib)
include_directories(/usr/local/lib/libpqxx/include)

#add_subdirectory(external/lib/cpp-dotenv)
#include_directories(external/lib/cpp-dotenv/include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp src/GithubHelperBot.cpp src/GithubHelperBot.h src/CurlClient.h)

target_link_libraries(${CMAKE_PROJECT_NAME} pqxx #[[cpp-dotenv ]] pq /usr/local/lib/libpqxx/lib/libpqxx.a
        /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

