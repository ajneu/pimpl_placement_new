cmake_minimum_required(VERSION 3.2)
project(cmake_demo)


###########
# Toolchain
###########
set(toolchain   "$ENV{HOME}/toolchain_desktop.cmake"      CACHE FILEPATH     "")  # override with e.g. -D toolchain=~/toolchain_embedded.cmake
                                                                                  # e.g. cmake -D toolchain="" ..

if (toolchain AND EXISTS ${toolchain})
  message("==> Including toolchain_file ${toolchain}")
  include(${toolchain})
endif()



###########
# Debug or Release
###########
if (NOT CMAKE_BUILD_TYPE)
  ## set default to Debug
  set(CMAKE_BUILD_TYPE Debug)  # override with -DCMAKE_BUILD_TYPE=Release
  message("==> CMAKE_BUILD_TYPE empty. Changing it to Debug.")
else()
  message("==> CMAKE_BUILD_TYPE == ${CMAKE_BUILD_TYPE}.")
endif()

## https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html
## During Debug, use debug version of libstdc++ (asserts on access to invalid iterators, etc!)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")


###########
# Suppress certain warnings
###########
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")


###########
# clean-all (removes cmake's cache)
###########

# http://stackoverflow.com/a/13714219
add_custom_target(clean-all
  COMMAND ${CMAKE_BUILD_TOOL} clean
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/clean-all.cmake
  )


##############
# C++ Standard
##############
set(CMAKE_CXX_STANDARD   11)
set(CMAKE_CXX_EXTENSIONS OFF)




##############
# Sources and Target Name
##############
include_directories( ${CMAKE_CURRENT_BINARY_DIR} . ) ## include_directories 
#                                                    ##  - Has ${CMAKE_CURRENT_BINARY_DIR}
#                                                    ##    so that ${CMAKE_CURRENT_BINARY_DIR} is preferred as search-path for header <ab_impl_aligninfo.h>
#                                                    ##
#                                                    ##  - Has . (i.e. ${CMAKE_CURRENT_SOURCE_DIR})
#                                                    ##    so that ${CMAKE_CURRENT_BINARY_DIR}/gen_ab_impl_aligninfo.cpp finds it's includes: ab.h ab.cpp
set(src    main.cpp ab.cpp)
set(target go)



##############
# Build and Link
##############
add_executable(${target} ${src}             ${CMAKE_CURRENT_BINARY_DIR}/ab_impl_aligninfo.h) ## header-to-generate is a dependency!
target_link_libraries(${target} ${libs})


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "Because the header ab_impl_aligninfo.h will be GENERATED (with proper values, unlike that dummy-header with the same name in the source-dir), the binary-build-dir must be different from the source dir!")
endif()

##############
# Generate Header (todo: should perhaps write a generic cmake-function for the following lines)
##############
add_executable(gen_aligninfo gen_aligninfo.cpp)

add_custom_command (
  OUTPUT                                          ${CMAKE_CURRENT_BINARY_DIR}/gen_ab_impl_aligninfo.cpp
  COMMAND gen_aligninfo AB::AB_impl ab.h ab.cpp > ${CMAKE_CURRENT_BINARY_DIR}/gen_ab_impl_aligninfo.cpp
  #                                 ^^^^^^^^^^^
  #                                 found in . (i.e. ${CMAKE_CURRENT_SOURCE_DIR})
  DEPENDS gen_aligninfo
  )


add_executable(gen_ab_impl_aligninfo              ${CMAKE_CURRENT_BINARY_DIR}/gen_ab_impl_aligninfo.cpp)

add_custom_command (
  OUTPUT                                   ${CMAKE_CURRENT_BINARY_DIR}/ab_impl_aligninfo.h
  COMMAND gen_ab_impl_aligninfo >          ${CMAKE_CURRENT_BINARY_DIR}/ab_impl_aligninfo.h
  DEPENDS gen_ab_impl_aligninfo
  )
