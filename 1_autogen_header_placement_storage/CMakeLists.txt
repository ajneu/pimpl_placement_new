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
#
# During development, you can set use      cmake -DNO_HEADER_GEN=1 ..   and set a real large Len value to satisfy the static_assert!
# Afterwards: clean the build dir and use  cmake ..
#

if(NO_HEADER_GEN   AND NOT ${NO_HEADER_GEN} EQUAL 0) # cmake -DNO_HEADER_GEN=1 ..
  # no header-generation
  add_executable(${target} ${src})
else()
  add_executable(${target} ${src}             ${CMAKE_CURRENT_BINARY_DIR}/ab_impl_aligninfo.h) ## header-to-generate is a dependency!
endif()
target_link_libraries(${target} ${libs})


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "Because the header ab_impl_aligninfo.h will be GENERATED (with proper values, unlike that dummy-header with the same name in the source-dir), the binary-build-dir must be different from the source dir!")
endif()



#################
# Generate Header
#################


add_executable(gen_aligninfo gen_aligninfo.cpp) # executable gen_aligninfo is used in function genalign (below)


function(genalign scoped_class generated_header_name)
  string(REGEX MATCH "[^:]*$" classname "${scoped_class}") # Get last component:
  #                                                        # If ${scoped_class} STREQUAL "AA::BB::CC";   then ${classname} is set to "CC"

  string(CONCAT gen_prog     "gen_" "${generated_header_name}" ".exe")
  string(CONCAT gen_prog_cpp "${gen_prog}" ".cpp")

  #message("###### \${scoped_class} == ${scoped_class}")
  #message("###### \${classname}    == ${classname}")
  #message("###### \${gen_prog}     == ${gen_prog}")
  #message("###### \${gen_prog_cpp} == ${gen_prog_cpp}")
  #message("###### \${ARGN}         == ${ARGN}")



  add_custom_command (
    OUTPUT                                          ${CMAKE_CURRENT_BINARY_DIR}/${gen_prog_cpp}
    COMMAND gen_aligninfo ${scoped_class} ${ARGN} > ${CMAKE_CURRENT_BINARY_DIR}/${gen_prog_cpp}
    #                                     ^^^^^^^
    #                                     found in . (i.e. ${CMAKE_CURRENT_SOURCE_DIR})
    DEPENDS gen_aligninfo
    )


  add_executable(${gen_prog}                        ${CMAKE_CURRENT_BINARY_DIR}/${gen_prog_cpp})

  add_custom_command (
    OUTPUT                                          ${CMAKE_CURRENT_BINARY_DIR}/${generated_header_name}
    COMMAND ${gen_prog} >                           ${CMAKE_CURRENT_BINARY_DIR}/${generated_header_name}
    DEPENDS ${gen_prog}
    )
  
endfunction()


if(NO_HEADER_GEN   AND NOT ${NO_HEADER_GEN} EQUAL 0) # cmake -DNO_HEADER_GEN=1 ..
  # generate no header
else()
  genalign(AB::AB_impl   ab_impl_aligninfo.h  ab.h ab.cpp)
  #        ^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^  ^^^^^^^^^^^
  #            |                  |                |
  #     For this class            |                |
  #                      Generate this header      |
  #                                          The class can be analyzed, by including these files into a cpp!
endif()
