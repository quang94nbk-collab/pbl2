# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PBL_StudentRegistration_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PBL_StudentRegistration_autogen.dir\\ParseCache.txt"
  "PBL_StudentRegistration_autogen"
  )
endif()
