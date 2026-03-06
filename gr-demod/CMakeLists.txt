cmake_minimum_required(VERSION 3.16)
project(gr-demod VERSION 1.0.0)

# ── GNU Radio ─────────────────────────────────────────────────────────────────
find_package(Gnuradio "3.10" REQUIRED COMPONENTS runtime pmt)
include(GrVersion)

# ── Python blocks (no C++ required) ──────────────────────────────────────────
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(GR_PYTHON_DIR "${CMAKE_INSTALL_PREFIX}/lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/dist-packages"
    CACHE PATH "Python dist-packages install path")

install(
    FILES
        python/demod/__init__.py
        python/demod/dcf_frame_sink.py
        python/demod/dcf_frame_source.py
    DESTINATION ${GR_PYTHON_DIR}/demod
)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()
add_test(
    NAME qa_dcf_frame
    COMMAND ${Python3_EXECUTABLE} -m pytest
            ${CMAKE_SOURCE_DIR}/python/demod/qa_dcf_frame.py -v
)

# ── GRC block YAML (for GNU Radio Companion) ─────────────────────────────────
install(
    DIRECTORY grc/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks
    FILES_MATCHING PATTERN "*.yml"
)
