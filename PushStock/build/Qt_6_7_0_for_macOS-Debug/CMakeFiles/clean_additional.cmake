# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PushStock_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PushStock_autogen.dir/ParseCache.txt"
  "PushStock_autogen"
  )
endif()
