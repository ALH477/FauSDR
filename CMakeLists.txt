cmake_minimum_required(VERSION 3.22)
project(dcf_c_sdk C)

set(CMAKE_C_STANDARD   11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Dependencies ───────────────────────────────────────────────────────────────
find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON   REQUIRED libcjson)
pkg_check_modules(PROTOBC REQUIRED libprotobuf-c)

# ── Library ────────────────────────────────────────────────────────────────────
set(DCF_SDK_SOURCES
    src/dcf_sdk/dcf_plugin_manager.c
    src/dcf_sdk/dcf_client.c
    src/dcf_sdk/dcf_redundancy.c
)

add_library(dcf_sdk STATIC ${DCF_SDK_SOURCES})

target_include_directories(dcf_sdk PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CJSON_INCLUDE_DIRS}
    ${PROTOBC_INCLUDE_DIRS}
)

target_link_libraries(dcf_sdk
    ${CJSON_LIBRARIES}
    ${PROTOBC_LIBRARIES}
    dl pthread m
)

target_compile_options(dcf_sdk PRIVATE -Wall -Wextra -O2)

# ── UDP transport plugin ───────────────────────────────────────────────────────
add_library(dcf_udp_transport SHARED plugins/custom_transport.c)
target_include_directories(dcf_udp_transport PRIVATE include ${CJSON_INCLUDE_DIRS})
target_link_libraries(dcf_udp_transport ${CJSON_LIBRARIES})
set_target_properties(dcf_udp_transport PROPERTIES PREFIX "lib")

# ── Tests ──────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(test_plugin tests/test_plugin.c)
target_include_directories(test_plugin PRIVATE include)
target_link_libraries(test_plugin dcf_sdk)
add_test(NAME plugin_tests COMMAND test_plugin)

add_executable(test_redundancy tests/test_redundancy.c)
target_include_directories(test_redundancy PRIVATE include)
target_link_libraries(test_redundancy dcf_sdk)
add_test(NAME redundancy_tests COMMAND test_redundancy)

# ── Install ────────────────────────────────────────────────────────────────────
install(TARGETS dcf_sdk ARCHIVE DESTINATION lib)
install(DIRECTORY include/dcf_sdk DESTINATION include)
