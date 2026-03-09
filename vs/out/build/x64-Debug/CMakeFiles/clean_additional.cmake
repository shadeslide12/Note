# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "src\\CMakeFiles\\cfd_post_autogen.dir\\AutogenUsed.txt"
  "src\\CMakeFiles\\cfd_post_autogen.dir\\ParseCache.txt"
  "src\\CMakeFiles\\qt_post_autogen.dir\\AutogenUsed.txt"
  "src\\CMakeFiles\\qt_post_autogen.dir\\ParseCache.txt"
  "src\\cfd_post_autogen"
  "src\\qt_post_autogen"
  )
endif()
