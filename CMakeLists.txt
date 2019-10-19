cmake_minimum_required(VERSION 3.5)
project(FloatFormat VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(FATAL_ERROR "Compiler not tested yet")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
     message(FATAL_ERROR "Compiler not supported")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   message(FATAL_ERROR "Compiler not tested yet")
else ()
  message(FATAL_ERROR "Unknown Compiler")
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Werror")
add_library(FloatFormat SHARED FF.cpp)
set_target_properties(FloatFormat PROPERTIES VERSION 1.0 SOVERSION 1)

add_executable(ff driver.cpp FF.cpp)
target_link_libraries(ff FloatFormat)

add_subdirectory(test)

install(TARGETS ff RUNTIME DESTINATION bin)
install(TARGETS FloatFormat LIBRARY DESTINATION lib)
install(FILES FF.h DESTINATION include)
