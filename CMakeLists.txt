CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(ClienServerEcn)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_EXPORT_COMPILE_COMMANDS $(CMAKE_BINARY_DIR)/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FIND_PACKAGE(Boost 1.74 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

include(cmake/CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#7.1.3")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:jtv/libpqxx#7.9.2")

OPTION(ENABLE_GTEST "Enable Google Test" ON)
IF(ENABLE_GTEST)
    CPMAddPackage("gh:google/googletest#v1.15.2")
ENDIF()

ADD_LIBRARY(
    ${PROJECT_NAME}_objs OBJECT 
    src/components/stock_exchange/component.hpp
    src/components/stock_exchange/component.cpp
    src/components/session/component.hpp
    src/components/session/component.cpp
    src/server/server.hpp
    src/server/server.cpp
    src/client/client.hpp
    src/client/client.cpp
    src/utils/json.hpp
    src/utils/json.cpp
    src/utils/logging.hpp
    src/utils/logging.cpp
    )

TARGET_LINK_LIBRARIES(${PROJECT_NAME}_objs PUBLIC nlohmann_json::nlohmann_json fmt::fmt libpqxx::pqxx ${Boost_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_objs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src  ${Boost_INCLUDE_DIRS})


ADD_EXECUTABLE(Server src/server.cpp)
TARGET_LINK_LIBRARIES(Server PRIVATE ${PROJECT_NAME}_objs)
ADD_EXECUTABLE(Client src/client.cpp)
TARGET_LINK_LIBRARIES(Client PRIVATE ${PROJECT_NAME}_objs)

IF(ENABLE_GTEST)
    ADD_EXECUTABLE(tests src/tests/test_main.cpp)
    TARGET_LINK_LIBRARIES(tests PRIVATE ${PROJECT_NAME}_objs gtest gtest_main)
    TARGET_INCLUDE_DIRECTORIES(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
ENDIF()