cmake_minimum_required(VERSION 3.7)

project(msexpr
    VERSION 0.0.1.0
    LANGUAGES C
    )

configure_file(version.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY
  )
option(USES_RE2C "Uses re2c to regenerate the lexer files" OFF)
if(USES_RE2C)
    #TODO check if re2c is installed
    find_program(RE2C re2c REQUIRED)

    add_custom_command( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c
        MAIN_DEPENDENCY	${CMAKE_CURRENT_SOURCE_DIR}/src/deserializers.c
        DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c.re2c ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.h
        COMMAND ${RE2C}
        ARGS -T -o ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c.re2c
        COMMENT "Generating re2c files"
        )
endif()

set(msexpr_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msexpr/sexpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msexpr/serializers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msexpr/deserializers.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    )

set(msexpr_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_ops.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_stack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serializers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/token.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c
    )

set(msexpr_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_ops.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_stack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.h
    )

set(msexpr_RE2C
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c.re2c
    )

add_library(${PROJECT_NAME} SHARED
    ${msexpr_INCLUDES}
    ${msexpr_SOURCES}
    ${msexpr_HEADERS}
    ${msexpr_RE2C}
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    )

option(ENABLE_TESTS "Enable tests" ON)
if (${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
