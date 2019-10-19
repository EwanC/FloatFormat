find_program(LIT NAMES llvm-lit lit)
find_program(FILECHECK NAMES FileCheck)

if (NOT LIT)
    message(FATAL_ERROR "Could not find lit testing tool.")
endif()

if (NOT FILECHECK)
    message(FATAL_ERROR "Could not find FileCheck testing tool.")
endif()

configure_file(lit.cfg.in lit.cfg @ONLY)
set(LIT_SOURCES help.lit)
file(COPY ${LIT_SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
