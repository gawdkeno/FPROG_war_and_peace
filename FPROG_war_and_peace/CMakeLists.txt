# CMakeList.txt : CMake project for FPROG_war_and_peace, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (FPROG_war_and_peace "FPROG_war_and_peace.cpp" "FPROG_war_and_peace.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FPROG_war_and_peace PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
