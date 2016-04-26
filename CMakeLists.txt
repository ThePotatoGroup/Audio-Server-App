#cmake_minimum_required(VERSION 3.3)
#project(Audio_Server_App)
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
#set(SOURCE_FILES main.cpp)
#add_executable(Audio_Server_App ${SOURCE_FILES})


cmake_minimum_required(VERSION 3.3)
project(Audio-Server-App)

# DEBUG FLAGS
#========================
set(CMAKE_VERBOSE_MAKEFILE OFF)
#========================

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "/Users/brycecater/Qt/5.5/clang_64")
# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Multimedia)


include_directories("easyloggingpp/src")
include_directories("/usr/local/include")
include_directories("Embedded-Audio-Shared")
link_directories("/usr/local/lib")

configure_file(easylogging++.conf easylogging++.conf COPYONLY)

add_executable(Audio-Server-App audioServerApp.cpp audioServerApp.h NetworkInterface.cpp NetworkInterface.h AudioSource.cpp AudioSource.h HostInputWindow.cpp HostInputWindow.h)

target_link_libraries(Audio-Server-App Qt5::Widgets)
target_link_libraries(Audio-Server-App Qt5::Network)
target_link_libraries(Audio-Server-App Qt5::Multimedia)
target_link_libraries(Audio-Server-App audiodecoder)


#SET(noParenSourcePath "")
#SET(noParenBinPath "")
#STRING(REPLACE "(" "\\(adfasdf" noParenSourcePath ${CMAKE_CURRENT_SOURCE_DIR})
#STRING(REPLACE ")" "\\)" noParenSourcePath ${CMAKE_CURRENT_SOURCE_DIR})
#STRING(REPLACE "(" "\\(" noParenBinPath ${CMAKE_CURRENT_BINARY_DIR})
#STRING(REPLACE ")" "\\)" noParenBinPath ${CMAKE_CURRENT_BINARY_DIR})
#
#if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
#    # list of files for which we add a copy rule
#    set(config_files easylogging++.conf)
#
#    foreach(item IN LISTS config_files)
#        message(STATUS ${item})
#        add_custom_command(
#                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
#                COMMAND ${CMAKE_COMMAND} -E copy "\"${noParenSourcePath}/${item}\"" "\"${noParenBinPath}/${item}\""
#                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
#        )
#    endforeach()
#endif()
#
## files are only copied if a target depends on them
#add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/easylogging++.conf")
#ADD_DEPENDENCIES(Audio-Server-App data-target)