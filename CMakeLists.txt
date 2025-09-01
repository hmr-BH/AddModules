cmake_minimum_required(VERSION 4.0)
project(AddModules)

if(WIN32)
    # MSVC
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.38)
            message(FATAL_ERROR
                    "MSVC >= 19.38 (Visual Studio 2022 17.8+) is required.\n"
                    "Please upgrade Visual Studio or switch to a newer toolchain.")
        endif()
        # MinGW / GNU / Clang
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
            message(FATAL_ERROR
                    "MinGW GCC >= 14.0 is required.\n"
                    "Current MinGW does not yet provide it; please use MSVC or MSYS2 Clang 17+.")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.0)
            message(FATAL_ERROR
                    "Clang >= 17.0 is required.\n"
                    "Please upgrade to MSYS2 UCRT64-clang or another Clang 17+ distribution.")
        endif()
    else()
        message(FATAL_ERROR "Unsupported compiler on Windows.")
    endif()
else() # Linux / macOS / *BSD / etc.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
            message(FATAL_ERROR
                    "GCC >= 14.0 is required. "
                    "Please install gcc-14 or newer.")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.0)
            message(FATAL_ERROR
                    "Clang >= 17.0 is required. "
                    "Please install clang-17 or newer.")
        endif()
    else()
        message(FATAL_ERROR "Unsupported compiler.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(add-module
        src/main.cpp
        src/cli/parser.cpp
        src/cli/parser.hpp
        src/utils/executable_path.hpp
        src/commands/commands.hpp
        src/commands/install.cpp
        src/commands/uninstall.cpp
        src/commands/list.cpp
        src/commands/show.cpp
        src/commands/help.cpp
        src/commands/about.cpp
        src/execute/install_module.cpp
        src/execute/execute.h
)

add_compile_options(
        $<$<CXX_COMPILER_ID:GNU,Clang>:-finput-charset=UTF-8 -fexec-charset=UTF-8>
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
file(MAKE_DIRECTORY "${OUTPUT_DIR}")
set(TARGET_FILE "$<TARGET_FILE:add-module>")

add_custom_command(TARGET add-module POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${TARGET_FILE}"
        "${OUTPUT_DIR}/"
        COMMENT "Copying add-module to ${OUTPUT_DIR}"
)

if(MSVC)
    target_compile_options(add-module PRIVATE
            $<$<CONFIG:Release>:/MT /O2 /Oi /GL /Gy>
            $<$<CONFIG:Debug>:/MTd>
    )
    target_link_options(add-module PRIVATE
            $<$<CONFIG:Release>:/INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG>
    )
endif()

if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_options(add-module PRIVATE -static -s)
endif()