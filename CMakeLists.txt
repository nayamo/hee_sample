cmake_minimum_required(VERSION 2.8)
# hybrid eki engine sample 
PROJECT(hee_sample)

#option(USE_EF     "use ekispert fare liblaly ."         ON)
option(USE_M32       "use compile option -m32"    ON)
option(BUILD_SHARED_LIBRARY "build shared liblay"        ON)

SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")


SET(CMAKE_C_FLAGS " -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
if (USE_M32)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(BUILD_SHARED_LIBRARY)
  SET(LINK_TYPE "SHARED")
else()
  SET(LINK_TYPE "STATIC")
endif()

SET(OPTIMIZE "3")

find_package(Boost)

#link_directories(
#  ${Boost_LIBRARY_DIRS}
#)

set(EXP_SRC_DIR ./src)

set(PLAT_SRC_DIR ${EXP_SRC_DIR}/lib/Platform/Sources)
set(PLAT_PUBINC_DIR ${EXP_SRC_DIR}/lib/Platform/PublicHeaders)

set(EXPDIALIB_DIR ${EXP_SRC_DIR}/lib/ExpDiaLib)
set(EXPDIALIB_SRC_DIR ${EXPDIALIB_DIR}/Sources)
set(EXPDIALIB_VIEWSRC_DIR ${EXPDIALIB_DIR}/ViewSources)
set(EXPDIALIB_INC_DIR ${EXPDIALIB_DIR}/Headers)
set(EXPDIALIB_PUBINC_DIR ${EXPDIALIB_DIR}/PublicHeaders)
set(EXPDIALIB_VIEWINC_DIR ${EXPDIALIB_DIR}/ViewHeaders)

set(EXPLIB_DIR ${EXP_SRC_DIR}/lib/ExpLib)
set(EXPLIB_SRC_DIR ${EXPLIB_DIR}/Sources)
set(EXPLIB_INC_DIR ${EXPLIB_DIR}/Headers)
set(EXPLIB_PUBINC_DIR ${EXPLIB_DIR}/PublicHeaders)

set(EFIF_PUBINC_DIR "/home/centos/git/e4/lib/efif/include/efif/public")
set(EFIF_LIB_DIR "/home/centos/git/e4/build")

# ソースとヘッダーが同じディレクトリにある
set(DISPLINE_DIR ${EXP_SRC_DIR}/line_tt)



# ビルドするためにインクルードするヘッダーファイルの場所
INCLUDE_DIRECTORIES("${PLAT_PUBINC_DIR}")

INCLUDE_DIRECTORIES("${EXPDIALIB_PUBINC_DIR}")
INCLUDE_DIRECTORIES("${EXPDIALIB_INC_DIR}")
INCLUDE_DIRECTORIES("${EXPDIALIB_VIEWINC_DIR}")

INCLUDE_DIRECTORIES("${EXPLIB_INC_DIR}")
INCLUDE_DIRECTORIES("${EXPLIB_PUBINC_DIR}")

INCLUDE_DIRECTORIES("${DISPLINE_DIR}")

INCLUDE_DIRECTORIES("${EFIF_PUBINC_DIR}")


# ライブラリ別にソースファイル登録
file(GLOB PLAT_SRC ${PLAT_SRC_DIR}/*.c)
file(GLOB EXPDIALIB_SRC ${EXPDIALIB_SRC_DIR}/*.c)
file(GLOB EXPLIB_SRC ${EXPLIB_SRC_DIR}/*.c)
file(GLOB DISPLINE_SRC ${DISPLINE_DIR}/*.c)


add_library(plat ${LINK_TYPE}
  ${PLAT_SRC}
)

add_library(expdia ${LINK_TYPE}
  ${EXPDIALIB_SRC}
)

add_library(displine ${LINK_TYPE}
  ${DISPLINE_SRC}
)

add_library(exp ${LINK_TYPE}
  ${EXPLIB_SRC}
)

target_link_libraries(exp
  e4
)


add_executable(sample
  ./sample.cpp
)

target_link_libraries(sample
  plat
  expdia
  displine
  exp
  boost_system
  boost_filesystem
)