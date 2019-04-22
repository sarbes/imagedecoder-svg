project(imagedecoder.svg)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)
enable_language(C)

find_package(Kodi REQUIRED)

include_directories(${KODI_INCLUDE_DIR}/..)


set(MPO_SOURCES src/SVGPicture.cpp)
#set(SVG_SOURCES src/SVGPicture.cpp)

set(DEPLIBS)

build_addon(imagedecoder.svg MPO DEPLIBS)

include(CPack)
