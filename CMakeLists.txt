cmake_minimum_required(VERSION 3.7)

project(msexpr
    VERSION 0.0.3.0
    LANGUAGES C
    )

configure_file(src/version.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/msexpr/version.h" @ONLY
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
    include/msexpr/sexpr.h
    include/msexpr/serializers.h
    include/msexpr/deserializers.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/msexpr/version.h
    )

set(msexpr_SOURCES
    src/sexpr.c
    src/sexpr_ops.c
    src/sexpr_stack.c
    src/serializers.c
    src/deserializers.c
    src/deserializer/token.c
    src/deserializer/input.c
    src/deserializer/rivest_canonical_lexer.c
    )

set(msexpr_HEADERS
    src/sexpr_ops.h
    src/sexpr_stack.h
    src/deserializer/token.h
    src/deserializer/input.h
    src/deserializer/rivest_canonical_lexer.h
    )

set(msexpr_RE2C
    src/deserializer/rivest_canonical_lexer.c.re2c
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
    PUBLIC_HEADER "${msexpr_INCLUDES}"
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

install(TARGETS msexpr
    EXPORT msexprTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )

install(
    FILES ${msexpr_INCLUDES}
    DESTINATION "include/msexpr"
    COMPONENT Devel
    )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/msexpr/msexpr-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

export(EXPORT msexprTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/msexpr/msexpr-targets.cmake"
    )

set(ConfigPackageLocation lib/cmake/msexpr)

install(EXPORT msexprTargets
    FILE "msexpr-targets.cmake"
    DESTINATION ${ConfigPackageLocation}
    )

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/msexpr/msexpr-config-version.cmake"
    DESTINATION ${ConfigPackageLocation}
    COMPONENT Devel
)



#Setup CPACK to generate installer
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "DEB;TGZ;ZIP")
set(CPACK_DEBIAN_PACKAGE_NAME "msexpr")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rui Maciel")
include(CPack)
