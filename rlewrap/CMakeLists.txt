CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
CMAKE_POLICY(VERSION 2.6)

FILE(GLOB src *.cc *.h)
FILE(GLOB luasrc *.lua)
SET(luasrc ${luasrc} rlewrap.inl)

MESSAGE(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "TORCH_INSTALL_PREFIX ${TORCH_INSTALL_PREFIX}")
MESSAGE(STATUS "LUAROCKS_INSTALL_PREFIX ${LUAROCKS_INSTALL_PREFIX}")

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    MESSAGE(STATUS "Using Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")        ## Optimize
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=1024")
    MESSAGE(STATUS "FLAGS ${CMAKE_CXX_FLAGS}")
# Using GCC.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    MESSAGE(STATUS "Using GCC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")        ## Optimize
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")        ## Optimize
    MESSAGE(STATUS "FLAGS ${CMAKE_CXX_FLAGS}")
# Using Intel C++.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    MESSAGE(STATUS "Using Intel C++")
# Using Visual Studio C++.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    MESSAGE(STATUS "Using Visual Studio C++")
ENDIF()

# We are building rlewraplib.
# We specify the paths to RLE includes and librle.
 INCLUDE_DIRECTORIES(${Torch_SOURCE_INCLUDES})
find_path(RLE_INCLUDE_DIR rle/rle_interface.hpp
          HINTS 
          ${TORCH_INSTALL_PREFIX}/include
          ${CMAKE_INSTALL_PREFIX}/include
          ${CMAKE_INSTALL_PREFIX}/../../../../../include
          ${LUAROCKS_INSTALL_PREFIX}/../../../../../include
          PATH_SUFFIXES include)

find_path(RLE_LIBRARY_DIR NAMES rle librle.so
          HINTS 
          ${TORCH_INSTALL_PREFIX}/lib 
          ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/../../../../../lib 
          ${LUAROCKS_INSTALL_PREFIX}/../../../../../lib)

find_library(RLE_LIBRARY NAMES rle librle.so
          HINTS 
          ${TORCH_INSTALL_PREFIX}/lib 
          ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/../../../../../lib 
          ${LUAROCKS_INSTALL_PREFIX}/../../../../../lib)

IF(${RLE_INCLUDE_DIR} STREQUAL "RLE_INCLUDE_DIR-NOTFOUND")
  MESSAGE(ERROR " Could not find rle include location")
ENDIF()

IF(${RLE_LIBRARY_DIR} STREQUAL "RLE_LIBRARY_DIR-NOTFOUND")
  MESSAGE(ERROR " Could not find rle library")
ENDIF()

IF(${RLE_LIBRARY} STREQUAL "RLE_LIBRARY-NOTFOUND")
  MESSAGE(ERROR " Could not find rle library")
ENDIF()

SET(INCDIR "${RLE_INCLUDE_DIR}")
SET(LIBDIR "${RLE_LIBRARY_DIR}")

INCLUDE_DIRECTORIES("${INCDIR}")
LINK_DIRECTORIES("${LIBDIR}")

ADD_TORCH_PACKAGE(rlewrap "${src}" "${luasrc}" "RLE Wrapper")
TARGET_LINK_LIBRARIES(rlewrap ${RLE_LIBRARY})

