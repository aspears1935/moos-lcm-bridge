cmake_minimum_required(VERSION 2.4.6)
#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

find_package(MOOS REQUIRED)
include_directories(${MOOS_INCLUDE_DIRS})
link_directories(${MOOS_LIBRARY_DIRS})
set(LIBS ${LIBS} ${MOOS_LIBRARIES})

# find_package(lcm REQUIRED)

INCLUDE(FindPackageMessage) 

message("----------------------------------------------")
if(MOOS_FOUND)
  message("MOOS FOUND!")
else(MOOS_FOUND)
  message("MOOS NOT FOUND")
endif(MOOS_FOUND)
message("----------------------------------------------")

#common commands for building c++ executables and libraries

FIND_PACKAGE( Boost 1.53 COMPONENTS thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#message(" Boost Libs: ${Boost_LIBRARIES}")
#message("----------------------------------------------")

#TARGET_LINK_LIBRARIES( Bridge ${Boost_LIBRARIES} )
#target_link_libraries( Bridge ${Boost_LIBRARIES} )
#target_link_libraries(Bridge MOOSGen)
#target_link_libraries(Bridge MOOSUtility)
#target_link_libraries(Bridge boost_thread)

INCLUDE_DIRECTORIES(moos_lcm_bridge_types)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable( MOOS_LCM_BRIDGE 
  src/MsgContainer.h 
  src/MOOSNode.cpp 
  src/Bridge.cpp
  )

target_link_libraries(MOOS_LCM_BRIDGE MOOS ${Boost_LIBRARIES} lcm)

#### BUILD THE BASIC EXAMPLES ####
#Also build the lcm test publisher for moos_t
add_executable(LcmTestPubMOOS 
  examples/lcm_publish_moos_t.cpp
  )
target_link_libraries(LcmTestPubMOOS lcm)

#Also build the lcm test publisher for moos_double_t
add_executable( LcmTestPubMOOSDouble 
  examples/lcm_publish_moos_double_t.cpp
  )
target_link_libraries(LcmTestPubMOOSDouble lcm)

#Also build the lcm test publisher for moos_string_t
add_executable( LcmTestPubMOOSString 
  examples/lcm_publish_moos_string_t.cpp
  )
target_link_libraries(LcmTestPubMOOSString lcm)

#Also build the lcm counter
add_executable( LcmCounter 
  examples/lcm_counter.cpp
  )
target_link_libraries(LcmCounter lcm)

#### BUILD THE BALEFIRE EXAMPLES ####
#Also build the lcm test nav publisher
add_executable( LcmTestPubNav 
  examples/lcm_publish_nav.cpp
  )
target_link_libraries(LcmTestPubNav lcm)

#Also build the lcm test listener
add_executable( LcmTestSubNav
  examples/lcm_subscribe_nav.cpp
  )
target_link_libraries(LcmTestSubNav lcm)

