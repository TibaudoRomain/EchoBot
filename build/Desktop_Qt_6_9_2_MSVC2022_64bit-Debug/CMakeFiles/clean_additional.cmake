# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ServerArm_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ServerArm_autogen.dir\\ParseCache.txt"
  "ServerArm_autogen"
  )
endif()
