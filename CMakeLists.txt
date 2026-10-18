cmake_minimum_required(VERSION 3.20)
project(eMule-kad LANGUAGES C CXX)

# Same dependencies as eMule (avwarez/eMule-build), as vanilla submodules at
# the repository root, so the sources' own #include "cryptopp/..." and
# #include "zlib/zlib.h" resolve unchanged.

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---------------------------------------------------------------------------
# zlib (madler/zlib v1.3.2) - has its own CMakeLists.txt, only the static
# library is needed.
# ---------------------------------------------------------------------------
set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ZLIB_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(zlib EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------------
# Crypto++ (weidai11/cryptopp CRYPTOPP_8_9_0) - no CMakeLists.txt of its own.
# The source list is read from cryptlib.vcxproj, as eMule-build does.
# CRYPTOPP_DISABLE_ASM keeps every compiler on the portable C++ code paths,
# so no per-file SIMD flags or MASM sources are needed: Kad only uses MD4,
# MD5 and AutoSeededRandomPool.
# ---------------------------------------------------------------------------
set(CRYPTOPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cryptopp")
file(STRINGS "${CRYPTOPP_DIR}/cryptlib.vcxproj" _cryptopp_lines
     REGEX "<ClCompile Include=\"[^\"]+\\.cpp\"")
set(CRYPTOPP_SOURCES "")
foreach(_line IN LISTS _cryptopp_lines)
    string(REGEX REPLACE ".*<ClCompile Include=\"([^\"]+)\".*" "\\1" _src "${_line}")
    list(APPEND CRYPTOPP_SOURCES "${_src}")
endforeach()
list(REMOVE_ITEM CRYPTOPP_SOURCES eccrypto.cpp eprecomp.cpp pch.cpp)
list(TRANSFORM CRYPTOPP_SOURCES PREPEND "${CRYPTOPP_DIR}/")

add_library(cryptopp STATIC ${CRYPTOPP_SOURCES})
target_include_directories(cryptopp PUBLIC "${CRYPTOPP_DIR}")
target_compile_definitions(cryptopp PUBLIC CRYPTOPP_DISABLE_ASM)
target_compile_features(cryptopp PRIVATE cxx_std_23)
set_target_properties(cryptopp PROPERTIES OUTPUT_NAME cryptlib)

# ---------------------------------------------------------------------------
# Kad
# ---------------------------------------------------------------------------
# srchybrid/ holds the Kad sources; compat/ will provide the eMule
# headers they include, reduced to what Kad uses, and comes first so that
# its stand-ins are the ones found.
file(GLOB_RECURSE KAD_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/srchybrid/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/compat/*.cpp"
)

add_library(emulekad STATIC ${KAD_SOURCES})
target_compile_features(emulekad PRIVATE cxx_std_23)
target_include_directories(emulekad
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/compat"
        "${CMAKE_CURRENT_SOURCE_DIR}/srchybrid"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_definitions(emulekad PRIVATE UNICODE _UNICODE)
target_link_libraries(emulekad PUBLIC cryptopp zlibstatic)
