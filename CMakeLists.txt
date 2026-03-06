cmake_minimum_required(VERSION 3.22)
project(demod-faust-sdr CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Compiler flags ─────────────────────────────────────────────────────────────
add_compile_options(
    -O3 -march=native -ffast-math
    -Wall -Wextra -Wno-unused-parameter
)

# ── Dependencies ───────────────────────────────────────────────────────────────
find_package(PkgConfig REQUIRED)

pkg_check_modules(SOAPY  REQUIRED SoapySDR)
pkg_check_modules(LIQUID REQUIRED liquid)
pkg_check_modules(FFTW   REQUIRED fftw3f)

# ── Helper: compile one .dsp file → .cpp via Faust ────────────────────────────
#
# Usage:
#   faust_compile(<dsp_stem> <arch_file>)
#
# Input:  dsp/<dsp_stem>.dsp
# Output: ${CMAKE_BINARY_DIR}/<dsp_stem>_gen.cpp
# Arch:   arch/<arch_file>  (e.g. soapy-sdr.cpp or soapy-sdr-lib.cpp)
#
# The generated target name is faust_<dsp_stem>.
function(faust_compile DSP_STEM ARCH_FILE)
    set(DSP_SRC  "${CMAKE_SOURCE_DIR}/dsp/${DSP_STEM}.dsp")
    set(DSP_OUT  "${CMAKE_BINARY_DIR}/${DSP_STEM}_gen.cpp")
    set(ARCH_SRC "${CMAKE_SOURCE_DIR}/arch/${ARCH_FILE}")

    add_custom_command(
        OUTPUT  "${DSP_OUT}"
        COMMAND faust
                -a   "${ARCH_SRC}"
                -lang c++
                -vec -vs 256
                -o   "${DSP_OUT}"
                "${DSP_SRC}"
        DEPENDS "${DSP_SRC}" "${ARCH_SRC}"
        COMMENT "Faust: ${DSP_STEM}.dsp → ${DSP_STEM}_gen.cpp"
        VERBATIM
    )
    add_custom_target("faust_${DSP_STEM}" DEPENDS "${DSP_OUT}")
endfunction()

# ── Compile all 10 DSP files ──────────────────────────────────────────────────
# Modulator path (standalone standalone arch: soapy-sdr.cpp  — includes main())
faust_compile(modulator      soapy-sdr.cpp)

# Modem library path (soapy-sdr-lib.cpp — library arch, no main())
faust_compile(qpsk_mod       soapy-sdr-lib.cpp)
faust_compile(gmsk_mod       soapy-sdr-lib.cpp)
faust_compile(fsk_mod        soapy-sdr-lib.cpp)
faust_compile(ask_mod        soapy-sdr-lib.cpp)
faust_compile(bpsk_demod     soapy-sdr-lib.cpp)
faust_compile(qpsk_demod     soapy-sdr-lib.cpp)
faust_compile(gmsk_demod     soapy-sdr-lib.cpp)
faust_compile(fsk_demod      soapy-sdr-lib.cpp)
faust_compile(ask_demod      soapy-sdr-lib.cpp)

# ── Meta-target: compile all DSP files in one step ───────────────────────────
# cmake --build build --target faust-compile
add_custom_target(faust-compile
    DEPENDS
        faust_modulator
        faust_qpsk_mod
        faust_gmsk_mod
        faust_fsk_mod
        faust_ask_mod
        faust_bpsk_demod
        faust_qpsk_demod
        faust_gmsk_demod
        faust_fsk_demod
        faust_ask_demod
)

# ── Standalone TX binary (modulator.dsp with main()) ─────────────────────────
add_executable(demod-sdr
    "${CMAKE_BINARY_DIR}/modulator_gen.cpp"
)
add_dependencies(demod-sdr faust_modulator)

target_include_directories(demod-sdr PRIVATE
    "${CMAKE_SOURCE_DIR}/transport"
    ${SOAPY_INCLUDE_DIRS}
    ${LIQUID_INCLUDE_DIRS}
    ${FFTW_INCLUDE_DIRS}
)
target_link_libraries(demod-sdr
    ${SOAPY_LIBRARIES}
    ${LIQUID_LIBRARIES}
    ${FFTW_LIBRARIES}
    m
)
target_link_options(demod-sdr PRIVATE ${SOAPY_LDFLAGS_OTHER})

# ── Modem shared library (all 9 modem DSP files, no main()) ──────────────────
#
# Linked as a shared library so both the Haskell path (via faust_bridge.cpp)
# and the standalone C++ test harness can load any scheme at runtime.
add_library(demod-modem SHARED
    "${CMAKE_BINARY_DIR}/qpsk_mod_gen.cpp"
    "${CMAKE_BINARY_DIR}/gmsk_mod_gen.cpp"
    "${CMAKE_BINARY_DIR}/fsk_mod_gen.cpp"
    "${CMAKE_BINARY_DIR}/ask_mod_gen.cpp"
    "${CMAKE_BINARY_DIR}/bpsk_demod_gen.cpp"
    "${CMAKE_BINARY_DIR}/qpsk_demod_gen.cpp"
    "${CMAKE_BINARY_DIR}/gmsk_demod_gen.cpp"
    "${CMAKE_BINARY_DIR}/fsk_demod_gen.cpp"
    "${CMAKE_BINARY_DIR}/ask_demod_gen.cpp"
)
add_dependencies(demod-modem
    faust_qpsk_mod faust_gmsk_mod faust_fsk_mod faust_ask_mod
    faust_bpsk_demod faust_qpsk_demod faust_gmsk_demod
    faust_fsk_demod faust_ask_demod
)
target_include_directories(demod-modem PRIVATE
    ${SOAPY_INCLUDE_DIRS}
    ${LIQUID_INCLUDE_DIRS}
    ${FFTW_INCLUDE_DIRS}
)
target_link_libraries(demod-modem
    ${SOAPY_LIBRARIES}
    ${LIQUID_LIBRARIES}
    ${FFTW_LIBRARIES}
    m
)

# ── Frame unit test ────────────────────────────────────────────────────────────
add_executable(test-frame src/test_frame.cpp)
target_include_directories(test-frame PRIVATE "${CMAKE_SOURCE_DIR}/transport")
target_compile_options(test-frame PRIVATE -g -O0)

# ── Conformance test harness ───────────────────────────────────────────────────
# Feeds the reference test vector to C codec and asserts 0x42DD CRC pin.
# Run: ctest --test-dir build -V
add_executable(test-conformance src/test_conformance.cpp)
target_include_directories(test-conformance PRIVATE "${CMAKE_SOURCE_DIR}/transport")
target_compile_options(test-conformance PRIVATE -g -O0)

enable_testing()
add_test(NAME frame_roundtrip  COMMAND test-frame)
add_test(NAME conformance      COMMAND test-conformance)

# ── Install ────────────────────────────────────────────────────────────────────
install(TARGETS demod-sdr demod-modem
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
install(DIRECTORY transport/ DESTINATION include/demod
        FILES_MATCHING PATTERN "*.h")
