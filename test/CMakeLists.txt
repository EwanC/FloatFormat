find_program(LIT NAMES llvm-lit lit)
find_program(FILECHECK NAMES FileCheck)

if (NOT LIT)
    message(STATUS "Could not find lit testing tool.")
elseif (NOT FILECHECK)
    message(STATUS "Could not find FileCheck testing tool.")
else()
configure_file(lit.cfg.in lit.cfg @ONLY)
set(LIT_SOURCES
    help.lit
    ELF_SO.lit
    simple_half.lit
    max_half.lit
    min_normal_half.lit
    max_subnormal_half.lit
    min_subnormal_half.lit
    hex_simple_half.lit
    hexf_simple_half.lit
    sci_simple_half.lit
    round_down_half.lit
    round_even_down_half.lit
    round_even_up_half.lit
    round_up_half.lit
    denorm_round_half.lit
    simple_float.lit
    max_float.lit
    min_float.lit
    hex_simple_float.lit
    hexf_simple_float.lit
    sci_simple_float.lit
    round_float.lit
    simple_double.lit
    max_double.lit
    min_double.lit
    hex_simple_double.lit
    hexf_simple_double.lit
    sci_simple_double.lit
    zero_half.lit
    zero_float.lit
    zero_double.lit
    signed_zero_half.lit
    signed_zero_float.lit
    signed_zero_double.lit
    inf_half.lit
    inf_float.lit
    inf_double.lit
    signed_inf_half.lit
    signed_inf_float.lit
    signed_inf_double.lit
    nan_half.lit
    nan_float.lit
    nan_double.lit
    repro_half_1.lit)
file(COPY ${LIT_SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
